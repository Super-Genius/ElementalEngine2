#include "stdafx.h"

using namespace std;

#include "pydll.h"
#include "PYDLLEngine.h"
#include "AutoLock.h"
#include "RequestItem.h"
#include "process.h"
#include "CryptoLayer.h"


char g_LogScratchSpace[1024]; // optimized for speed - prevent reallocation
CRITICAL_SECTION g_loggingCriticalSection; // optimized out so it can be called from within consts

CPYDLLEngine::CPYDLLEngine() : 
#ifdef LOGGING_ENABLED
#ifdef _DEBUG
	m_logLevel(eLog_ExtraVerbose), 
#else
	m_logLevel(eLog_Warning),
#endif
#endif
	m_bInitialized(false),
	m_bShouldBeProcessing(false),
	m_bIsProcessing(false),
	m_CurrentlyWorkingItem(NULL),
	m_pCrypto(NULL)
{
	m_pCrypto = new CCryptoLayer(this);
	m_ulNextRequestID = 1;
	m_CurrentlyWorkingItem = (CRequestItem *)NULL;
	
	// Alter this to change the default logging level.
	LOGGING_LINE InitializeCriticalSection(&g_loggingCriticalSection); /// cannot log until this is initialized.
	InitializeCriticalSection(&m_flagsCriticalSection);
	InitializeCriticalSection(&m_PendingCriticalSection);
	InitializeCriticalSection(&m_QueuedCriticalSection);
	InitializeCriticalSection(&m_CurrentWorkingCriticalSection);
	InitializeCriticalSection(&m_CompletedCriticalSection);

	m_newItemEvent = CreateEvent(NULL,false,false,NULL);

	LOGGING_LINE LogString(eLog_Verbose,"PY-DLL - Startup... testing endianness");

	// discover endian-ness
	char inter[2];
	unsigned short integervalue = 0x01;
	memcpy(inter,&integervalue,2UL);
	
	if (inter[0] == 0x01)
		m_endianNess = eEndian_LittleEndian; // intel, et al.
	else
		m_endianNess = eEndian_BigEndian;

	LOGGING_LINE LogString(eLog_Verbose,"EndianNess: %s",m_endianNess==eEndian_BigEndian?"Big Endian":"Little Endian (intel).");

	// Initialize some defaults
	m_ResponseTimeoutMS = 10 * 1000; // 10 sec
	m_RequestExpirationTimeoutMS = 60 * 1000; // 60 sec
}

CPYDLLEngine::~CPYDLLEngine()
{
	LOGGING_LINE LogString(eLog_Verbose,"Shutting down the PYDLL engine...");
	
	LOGGING_LINE LogString(eLog_ExtraVerbose,"Destroying PY communications  layer - cancelling all remaining requests...");
	stopProcessing(false);
	LOGGING_LINE LogString(eLog_ExtraVerbose,"Destroying PY communications  layer - stopping socket layer...");
	WSACleanup();

#ifdef _XBOX
	XNetCleanup();
#endif

	LOGGING_LINE LogString(eLog_ExtraVerbose,"PyDLL Engine shut down.");

	LOGGING_LINE DeleteCriticalSection(&g_loggingCriticalSection);

	// lets clear the queues.
	
	if (m_bShouldBeProcessing)
	{
		stopProcessing(false); // kill / cancel all remaining processes.
		while (m_bIsProcessing)
		{
			::Sleep(10); // the only sleep we use!
		}
	}
	

#ifdef _XBOX
	printf("End of PYDLL engine object - clearing requests\r\n");
#endif

	EnterCriticalSection(&m_PendingCriticalSection);
	m_PendingRequests.clear();
	LeaveCriticalSection(&m_PendingCriticalSection);
	DeleteCriticalSection(&m_PendingCriticalSection);

	EnterCriticalSection(&m_QueuedCriticalSection);
	m_QueuedRequests.clear();
	LeaveCriticalSection(&m_QueuedCriticalSection);
	DeleteCriticalSection(&m_QueuedCriticalSection);

	EnterCriticalSection(&m_CurrentWorkingCriticalSection);
	m_CurrentlyWorkingItem = NULL;
	LeaveCriticalSection(&m_CurrentWorkingCriticalSection);
	DeleteCriticalSection(&m_CurrentWorkingCriticalSection);

	EnterCriticalSection(&m_CompletedCriticalSection);
	m_CompletedRequests.clear();
	LeaveCriticalSection(&m_CompletedCriticalSection);
	DeleteCriticalSection(&m_CompletedCriticalSection);

#ifdef _XBOX
	printf("End of PYDLL engine object - closing handle\r\n");
#endif

	CloseHandle(m_newItemEvent);
	if (m_pCrypto) delete m_pCrypto;
	m_pCrypto = NULL;
}

//  A generic logging function
// Currently uses OutputDebugStringA
// Because all logging goes through here and specifies the level
// it allows future expansion to file or whatever.
#ifdef LOGGING_ENABLED
void CPYDLLEngine::LogString(const eLogSeverity level,const char * const szText,...) const
{
	AUTOLOCK(lock,&g_loggingCriticalSection);

	if (!szText) return;
	if (m_logLevel < level) return; // discard

	va_list argptr;
	va_start( argptr, szText);
	vsprintf_s(g_LogScratchSpace,1024,szText, argptr);
	va_end(argptr);
	::OutputDebugStringA("-PYDLL-> ");
	switch (level)
	{
		case eLog_Error:
			::OutputDebugStringA("** Error ** ");
			break;
		case eLog_Warning:
			::OutputDebugStringA("* Warning * ");
			break;
		case eLog_Verbose:
			::OutputDebugStringA("            ");
			break;
		case eLog_ExtraVerbose:
			::OutputDebugStringA("   Debug    ");
			break;
	}
	::OutputDebugStringA(g_LogScratchSpace);
	::OutputDebugStringA("\r\n");
}
#endif

bool CPYDLLEngine::UTIL_EndianConvert(void *const data,const unsigned long datalength) const
{
	if (m_endianNess == eEndian_LittleEndian) return true; // no work to do

	// we have to convert
	unsigned char *dataptr = (unsigned char *)data;

	if (datalength < 2) return true; // empty and 1 byte remain the same
	if ((datalength % 2) != 0) return false;

	for (unsigned long a=0; a < (datalength / 2);a++)
	{
		unsigned char temp = dataptr[a];
		dataptr[a] = dataptr[(datalength-1) - a];
		dataptr[(datalength-1) - a] = temp;
	}
	return true;
}

#ifdef _XBOX
unsigned long CPYDLLEngine::UTIL_ConvertHostnameToADDR(const char *const hostname) const
{
	unsigned long host_addr = inet_addr(hostname);
	if(host_addr == INADDR_NONE)
	{
		LOGGING_LINE LogString(eLog_ExtraVerbose,"hostname search - %s looking up via XNET DNS...",hostname);
		host_addr = 0;
		// unfortunately, the hostname input was not a dotted ip address
		// we have to invoke the dreaded XNET
		XNDNS *dns = NULL;
		if (XNetDnsLookup(hostname,NULL,&dns)==0)
		{
			LOGGING_LINE LogString(eLog_ExtraVerbose,"hostname search - waiting for xnet lookup to finish...",hostname);
			// wait for it to end.
			// this is already on another thread so its okay to block
			while (dns->iStatus == WSAEINPROGRESS)
			{
				::Sleep(1);
			}

			if (dns->iStatus == 0) // success
			{
				LOGGING_LINE LogString(eLog_ExtraVerbose,"hostname search %i addresses returned...",(int)dns->cina);
				if (dns->cina > 0)
				{
					in_addr addr = dns->aina[0];
					host_addr = addr.S_un.S_addr;
					LOGGING_LINE LogString(eLog_ExtraVerbose,"hostname search.. using address 0: %i %i %i %i",(int)addr.S_un.S_un_b.s_b1,(int)addr.S_un.S_un_b.s_b2,(int)addr.S_un.S_un_b.s_b3,(int)addr.S_un.S_un_b.s_b4);
				}
			}
			else
			{
				// unable to lookup hostname due to xnet error
			}
			XNetDnsRelease(dns);
		}
		else
		{
			// unable to lookup hostname due to xnet error
		}
	}
	return host_addr;
}
#else // not an xbox

unsigned long CPYDLLEngine::UTIL_ConvertHostnameToADDR(const char *const hostname) const
{
	unsigned long host_addr = inet_addr(hostname);
	if(host_addr == INADDR_NONE)
	{
		host_addr = 0;
		hostent *hp = NULL;
		hp = gethostbyname(hostname);
		if (!hp)
		{
			return 0;
		}
		// intelligence occurs here:
		// the result could be multiple addresses
		// all bound to different servers
		// we want the one that looks like its not localhost
		// or a private network address (if possible)
		// although we will settle for it if no other is available.
		int whichtarget = 0;
		unsigned char *first_host = NULL;
		while (hp->h_addr_list[whichtarget]!=0)
		{
			unsigned char addr[4];
			memcpy(addr,hp->h_addr_list[whichtarget],4);
			if (first_host==NULL)
			{
				first_host = (unsigned char *)hp->h_addr_list[whichtarget];
			}
			else
			{
				if (((addr[0]==192)&&(addr[1]==168))||
					((addr[0]==127)&&(addr[1]==0))||
					((addr[0]==10)&&(addr[1]==0)&&(addr[2]==0)))
				{
					whichtarget++;
					continue;
				}
				
				first_host = (unsigned char *)hp->h_addr_list[whichtarget];
			}

			if (first_host)
			{
				if (((first_host[0]==192)&&(first_host[1]==168))||
					((first_host[0]==127)&&(addr[1]==0))||
					((first_host[0]==10)&&(first_host[1]==0)&&(first_host[2]==0)))
				{
				}
				else
				{
					break; // found a nice address.
				}
			}
			whichtarget++;
		}


		
		unsigned long value;
		memcpy(&value,first_host,4);
		host_addr = value;
	}
	return host_addr;
}
#endif

CCryptoLayer *CPYDLLEngine::getCryptoLayer()
{
	return m_pCrypto;
}


HRESULT CPYDLLEngine::initializePYConnection(const char * const serverAddress, const unsigned short serverPort)
{
	// Make sure that this function is not re-entrant.
	AUTOLOCK(lock,&m_flagsCriticalSection);
	LOGGING_LINE LogString(eLog_Verbose,"initializePYConnection being called with \"%s\" and %i",serverAddress,(int)serverPort);
	// initialize the endpoint addresses.
	// do not connect to them
	// return an error state if the endpoint address has been changed.
	if (m_bInitialized)
	{
		LOGGING_LINE LogString(eLog_Warning,"initializePYConnection is being called twice.  Only do this if you need to switch hosts...");
	}

#ifdef _XBOX
	XNetStartupParams xnsp;
	memset(&xnsp, 0, sizeof(xnsp));
	xnsp.cfgSizeOfStruct = sizeof(XNetStartupParams);
	xnsp.cfgFlags = XNET_STARTUP_BYPASS_SECURITY;
	INT err = XNetStartup(&xnsp);
#endif


	if (m_bInitialized==false)
	{
		LOGGING_LINE LogString(eLog_ExtraVerbose,"starting socket layer...");
		WORD wVersionRequested;
		WSADATA wsaData;
		int err;
		wVersionRequested = MAKEWORD( 2, 2 );
		err = WSAStartup( wVersionRequested, &wsaData );
		if (err != 0)
		{
			LOGGING_LINE LogString(eLog_Error,"Unable to start the underlying socket layer (winsock). Error: 0x%X",(long)err);
			return (HRESULT)err;
		}
	}
	m_bInitialized = false;

	LOGGING_LINE LogString(eLog_ExtraVerbose,"Searching for hostname %s...",serverAddress);
	memset(&m_ServerAddress,0,sizeof(SOCKADDR_IN));
	m_ServerAddress.sin_family = AF_INET; // Must be AF_INET
	unsigned long addr = UTIL_ConvertHostnameToADDR(serverAddress);
	m_ServerAddress.sin_addr.s_addr = addr;
	m_ServerAddress.sin_port = htons(serverPort);
	LOGGING_LINE LogString(eLog_ExtraVerbose,"hostname search returned 0x%X...",addr);

	if (addr == 0UL)
	{
		LOGGING_LINE LogString(eLog_Error,"Unable to resolve host: %s",serverAddress);
		return ERROR_INCORRECT_ADDRESS;
	}

	memset(&m_LocalSocketBind,0,sizeof(SOCKADDR_IN));
	
	m_LocalSocketBind.sin_family = AF_INET;
	m_LocalSocketBind.sin_addr.s_addr = INADDR_ANY;
	m_LocalSocketBind.sin_port = 0;

	// Right - we know the server address at this point, as well as have the other necessary stuff
	// We can immediately start sending packets at this point (encryption is not supported until
	// those functions are called).

	m_bInitialized = true;
	return S_OK;
}

HRESULT CPYDLLEngine::setResponseTimeoutMS(const unsigned long milliseconds)
{
	LOGGING_LINE LogString(eLog_Verbose,"setResponseTimeoutMS(%lu);",milliseconds);

	if ((milliseconds < 100)&&(milliseconds > 0))
	{
		LOGGING_LINE LogString(eLog_Warning,"setResponseTimeoutMS(%lu) called - 100 ms is a very short time and will probably cause issues",milliseconds);
	}
	if (milliseconds==0)
	{
		LOGGING_LINE LogString(eLog_Error,"setResponseTimeoutMS(%lu); - cannot set zero millisecond timeout.",milliseconds);
		return E_INVALIDARG;
	}
	
	m_ResponseTimeoutMS	= milliseconds;
	return S_OK;
}

HRESULT CPYDLLEngine::setRequestExpirationTimeoutMS(const unsigned long milliseconds)
{
	LOGGING_LINE LogString(eLog_Verbose,"setRequestExpirationTimeoutMS(%lu);",milliseconds);

	if ((milliseconds < 100)&&(milliseconds > 0))
	{
		LOGGING_LINE LogString(eLog_Warning,"setRequestExpirationTimeoutMS(%lu) called - 100 ms is a very short time and will probably cause issues",milliseconds);
	}
	if (milliseconds==0)
	{
		LOGGING_LINE LogString(eLog_Error,"setRequestExpirationTimeoutMS(%lu); - cannot set zero millisecond timeout.",milliseconds);
		return E_INVALIDARG;
	}
	m_RequestExpirationTimeoutMS = milliseconds;
	return S_OK;
}

HRESULT CPYDLLEngine::createNewRequest(const bool enableEncryption, const unsigned long requestType,unsigned long * const newRequestID)
{
	LOGGING_LINE LogString(eLog_Verbose,"createNewRequest(...)");
	if (m_bInitialized==false)
	{
		LOGGING_LINE LogString(eLog_Error,"Function called when not initialized.");
		return ERROR_NOT_READY;
	}
	// prevent re-entrance otherwise the same uid might be given to multiple requests
	{
		AUTOLOCK(lock,&m_PendingCriticalSection);
		*newRequestID = m_ulNextRequestID;
		CRequestItem *newrequest = new CRequestItem(enableEncryption,requestType,m_ulNextRequestID,this);
		m_PendingRequests.insert(make_pair(m_ulNextRequestID,newrequest));
		m_ulNextRequestID++;
	}

	return S_OK;
}



HRESULT CPYDLLEngine::setRequestUserData(const unsigned long requestID,const unsigned long datalength,const void * const dataBuffer)
{
	LOGGING_LINE LogString(eLog_Verbose,"setRequestUserData(...)");
	if (m_bInitialized==false)
	{
		LOGGING_LINE LogString(eLog_Error,"Function called when not initialized.");
		return ERROR_NOT_READY;
	}

	if (datalength > 0)
	{
		if (!dataBuffer)
		{
			LOGGING_LINE LogString(eLog_Error,"setRequestUserData called with nonzero length and null buffer");
			return E_POINTER;
		}
	}

	// you can unfortunately set the user data at any point during the lifetime of a request.
	SmartRequestPointer req(NULL);
	if (!FindRequest(requestID,req,true,true,true,true))
	{
		LOGGING_LINE LogString(eLog_Error,"setRequestUserData could not find the requested element");
		return ERROR_NOT_FOUND;
	}
	req->setUserData(datalength,dataBuffer);
	return S_OK;
}

HRESULT CPYDLLEngine::getRequestUserData(const unsigned long requestID,unsigned long *const datalength,void * const dataBuffer)
{
	LOGGING_LINE LogString(eLog_Verbose,"getRequestUserData(...)");
	if (m_bInitialized==false)
	{
		LOGGING_LINE LogString(eLog_Error,"Function called when not initialized.");
		return ERROR_NOT_READY;
	}

	// you can unfortunately set the user data at any point during the lifetime of a request.
	SmartRequestPointer req(NULL);
	if (!FindRequest(requestID,req,true,true,true,true))
	{
		*datalength = 0;
		LOGGING_LINE LogString(eLog_Error,"getRequestUserData could not find the requested element");
		return ERROR_NOT_FOUND;
	}

	if (*datalength==0)
	{
		// user wants to know how much data there is
		*datalength = req->getUserDataLength();
		return S_OK;
	}

	// otherwise, we want the first datalength bytes of the user data or the entire buffer if its smaller
	*datalength = min(req->getUserDataLength(),*datalength);
	req->copyUserData(*datalength,dataBuffer);

	return S_OK;
}

HRESULT CPYDLLEngine::getRequestStatus(const unsigned long requestID,eRequestStatus *status)
{
	LOGGING_LINE LogString(eLog_Verbose,"getRequestStatus(...)");
	if (m_bInitialized==false)
	{
		LOGGING_LINE LogString(eLog_Error,"Function called when not initialized.");
		return ERROR_NOT_READY;
	}

	// the status of a request is also one of those types that means searching all elements.
	SmartRequestPointer req(NULL);
	if (!FindRequest(requestID,req,true,true,true,true))
	{
		*status = ePY_UnknownRequest;
		LOGGING_LINE LogString(eLog_Warning,"getRequestStatus could not find the requested element");
		return ERROR_NOT_FOUND;
	}

	*status = req->getStatus();
	return S_OK;

}

HRESULT CPYDLLEngine::submitRequest(const unsigned long requestID)
{
	LOGGING_LINE LogString(eLog_Verbose,"submitRequest(...)");
	if (m_bInitialized==false)
	{
		LOGGING_LINE LogString(eLog_Error,"Function called when not initialized.");
		return ERROR_NOT_READY;
	}

	// You can only submit a request in the pending queue.
	SmartRequestPointer req(NULL);
	if (!FindRequest(requestID,req,true,false,false,false))
	{
		LOGGING_LINE LogString(eLog_Warning,"submitRequest() - cannot submit request - only pending requests can be submitted");
		return ERROR_NOT_FOUND;
	}

	// remove it from the queue in its own lock section and put it back.
	{
		// locking both queues ensures that the request is never in the middle state.
		AUTOLOCK(lock,&m_PendingCriticalSection);
		AUTOLOCK(lock2,&m_QueuedCriticalSection);
		m_PendingRequests.erase(requestID);

		if (req->getStatus()!=ePY_Cancelled)
			req->setStatus(ePY_Queued);

		m_QueuedRequests.insert(std::make_pair(requestID,req.GetElement()));
		// remove it from the queue
	}
	
	// Over here, we trigger the semaphore that unblocks the processing queue, since we don't like to ever 'sleep'
	SetEvent(m_newItemEvent);

	return S_OK;
}

HRESULT CPYDLLEngine::getRequestType(const unsigned long requestID,unsigned long * const requestType)
{
	LOGGING_LINE LogString(eLog_Verbose,"getRequestType(...)");
	if (m_bInitialized==false)
	{
		LOGGING_LINE LogString(eLog_Error,"Function called when not initialized.");
		return ERROR_NOT_READY;
	}

	SmartRequestPointer req(NULL);
	if (!FindRequest(requestID,req,true,true,true,true))
	{
		*requestType = 0;
		LOGGING_LINE LogString(eLog_Warning,"getRequestType() could not find the element in question.");
		return ERROR_NOT_FOUND;
	}

	*requestType = req->getRequestType();
	return S_OK;
}

// Here we need to return the id of the first element that has a finished type status.
HRESULT CPYDLLEngine::getNextFinishedRequestID(unsigned long * const requestID)
{
	LOGGING_LINE LogString(eLog_Verbose,"getNextFinishedRequestID(...)");
	if (m_bInitialized==false)
	{
		LOGGING_LINE LogString(eLog_Error,"Function called when not initialized.");
		return ERROR_NOT_READY;
	}

	*requestID = 0;

	// we don't check the currently working item here cuz its not finished by definition
	AUTOLOCK(lock,&m_CompletedCriticalSection); // since were performing a find we cant really let the structure change.
	for (RequestQueue::const_iterator pos = m_CompletedRequests.begin();pos != m_CompletedRequests.end();++pos)
	{
		eRequestStatus stat = pos->second->getStatus();
		if ((stat==ePY_Cancelled)||(stat==ePY_Failed)||(stat==ePY_Completed))
		{
			*requestID = pos->second->getRequestID();
			break;
		}
	}

	return S_OK;
}


HRESULT CPYDLLEngine::writeData(const unsigned long requestID,const unsigned long dataLength,const void * const ptrData)
{
	LOGGING_LINE LogString(eLog_Verbose,"writeData(...)");
	if (m_bInitialized==false)
	{
		LOGGING_LINE LogString(eLog_Error,"Function called when not initialized.");
		return ERROR_NOT_READY;
	}

	if (dataLength == 0)
	{
		LOGGING_LINE LogString(eLog_Error,"writeData: Zero length data cannot be written");
		return E_INVALIDARG;
	}

	if (ptrData == NULL)
	{
		LOGGING_LINE LogString(eLog_Error,"writeData: NULL DATA POINTER");
		return E_POINTER;
	}
	
	SmartRequestPointer req(NULL);
	if (!FindRequest(requestID,req,true,false,false,false))
	{
		LOGGING_LINE LogString(eLog_Error,"writeData: No such requestID: %lu - either its gone, or is no longer pending.  Submitted requests cant be written to.",requestID);
		return ERROR_NOT_FOUND;
	}

	if (!req->addData(dataLength,ptrData))
	{
		LOGGING_LINE LogString(eLog_Error,"writeData: Failed to write %lu bytes (probably data too big or already submitted)",dataLength);
		return ERROR_NOT_FOUND;
	}


	return S_OK;
}

HRESULT CPYDLLEngine::writeString(const unsigned long requestID,const char *const stringptr)
{
	LOGGING_LINE LogString(eLog_Verbose,"writeString(...)");
	if (m_bInitialized==false)
	{
		LOGGING_LINE LogString(eLog_Error,"Function called when not initialized.");
		return ERROR_NOT_READY;
	}

	if (!stringptr)
	{
		LOGGING_LINE LogString(eLog_Error,"writeString: NULL STRING POINTER");
		return E_POINTER;
	}

	size_t len = strlen(stringptr);
	if (len > 65535) 
	{
		LOGGING_LINE LogString(eLog_Error,"writeString: String too long");
		return E_FAIL;
	}

	HRESULT res = writeUINT16(requestID,(UINT16)len);
	if (res != S_OK)
	{
		LOGGING_LINE LogString(eLog_Error,"writeString: Failed to write 2 bytes (probably data too big, or request already submitted.)");
		return res;
	}
	return writeData(requestID,(unsigned long)len,stringptr);
}

HRESULT CPYDLLEngine::writeUINT8(const unsigned long requestID,const UINT8 data)
{
	LOGGING_LINE LogString(eLog_Verbose,"writeUINT8(...)");
	if (m_bInitialized==false)
	{
		LOGGING_LINE LogString(eLog_Error,"Function called when not initialized.");
		return ERROR_NOT_READY;
	}

	// everything written must be little-endian
	UINT8 outdata(data);
	if (UTIL_EndianConvert(&outdata,sizeof(outdata))==false)
	{
		LOGGING_LINE LogString(eLog_Error,"writeUINT8 failed to endian-convert");
		return E_INVALIDARG;
	}

	return writeData(requestID,sizeof(outdata),&outdata);
}

HRESULT CPYDLLEngine::writeUINT16(const unsigned long requestID,const UINT16 data)
{
	LOGGING_LINE LogString(eLog_Verbose,"writeUINT16(...)");
	if (m_bInitialized==false)
	{
		LOGGING_LINE LogString(eLog_Error,"Function called when not initialized.");
		return ERROR_NOT_READY;
	}

	// everything written must be little-endian
	UINT16 outdata(data);
	if (UTIL_EndianConvert(&outdata,sizeof(outdata))==false)
	{
		LOGGING_LINE LogString(eLog_Error,"writeUINT16 failed to endian-convert");
		return E_INVALIDARG;
	}

	return writeData(requestID,sizeof(outdata),&outdata);
}

HRESULT CPYDLLEngine::writeUINT32(const unsigned long requestID,const UINT32 data)
{
	LOGGING_LINE LogString(eLog_Verbose,"writeUINT32(...)");
	if (m_bInitialized==false)
	{
		LOGGING_LINE LogString(eLog_Error,"Function called when not initialized.");
		return ERROR_NOT_READY;
	}

	// everything written must be little-endian
	UINT32 outdata(data);
	if (UTIL_EndianConvert(&outdata,sizeof(outdata))==false)
	{
		LOGGING_LINE LogString(eLog_Error,"writeUINT32 failed to endian-convert");
		return E_INVALIDARG;
	}

	return writeData(requestID,sizeof(outdata),&outdata);
}

HRESULT CPYDLLEngine::writeFLOAT32(const unsigned long requestID,const FLOAT32 data)
{
	LOGGING_LINE LogString(eLog_Verbose,"writeFLOAT32(...)");
	if (m_bInitialized==false)
	{
		LOGGING_LINE LogString(eLog_Error,"Function called when not initialized.");
		return ERROR_NOT_READY;
	}

	// everything written must be little-endian
	FLOAT32 outdata(data);
	if (UTIL_EndianConvert(&outdata,sizeof(outdata))==false)
	{
		LOGGING_LINE LogString(eLog_Error,"writeFLOAT32 failed to endian-convert");
		return E_INVALIDARG;
	}

	return writeData(requestID,sizeof(outdata),&outdata);
}

HRESULT CPYDLLEngine::writeINT8(const unsigned long requestID,const INT8 data)
{
	LOGGING_LINE LogString(eLog_Verbose,"writeINT8(...)");
	if (m_bInitialized==false)
	{
		LOGGING_LINE LogString(eLog_Error,"Function called when not initialized.");
		return ERROR_NOT_READY;
	}

	// everything written must be little-endian
	INT8 outdata(data);
	if (UTIL_EndianConvert(&outdata,sizeof(outdata))==false)
	{
		LOGGING_LINE LogString(eLog_Error,"writeINT8 failed to endian-convert");
		return E_INVALIDARG;
	}

	return writeData(requestID,sizeof(outdata),&outdata);
}

HRESULT CPYDLLEngine::writeINT16(const unsigned long requestID,const INT16 data)
{
	LOGGING_LINE LogString(eLog_Verbose,"writeINT16(...)");
	if (m_bInitialized==false)
	{
		LOGGING_LINE LogString(eLog_Error,"Function called when not initialized.");
		return ERROR_NOT_READY;
	}

	// everything written must be little-endian
	INT16 outdata(data);
	if (UTIL_EndianConvert(&outdata,sizeof(outdata))==false)
	{
		LOGGING_LINE LogString(eLog_Error,"writeINT16 failed to endian-convert");
		return E_INVALIDARG;
	}

	return writeData(requestID,sizeof(outdata),&outdata);
}

HRESULT CPYDLLEngine::writeINT32(const unsigned long requestID,const INT32 data)
{
	LOGGING_LINE LogString(eLog_Verbose,"writeINT32(...)");
	if (m_bInitialized==false)
	{
		LOGGING_LINE LogString(eLog_Error,"Function called when not initialized.");
		return ERROR_NOT_READY;
	}

	// everything written must be little-endian
	INT32 outdata(data);
	if (UTIL_EndianConvert(&outdata,sizeof(outdata))==false)
	{
		LOGGING_LINE LogString(eLog_Error,"writeINT32 failed to endian-convert");
		return E_INVALIDARG;
	}

	return writeData(requestID,sizeof(outdata),&outdata);
}


HRESULT CPYDLLEngine::readData(const unsigned long requestID,const unsigned long dataLength, void * const ptrDestBuffer)
{
	LOGGING_LINE LogString(eLog_Verbose,"readData(...)");
	if (m_bInitialized==false)
	{
		LOGGING_LINE LogString(eLog_Error,"Function called when not initialized.");
		return ERROR_NOT_READY;
	}

	if (dataLength==0)
	{
		LOGGING_LINE LogString(eLog_Error,"readData called with zero length");
		return E_INVALIDARG;
	}

	if (!ptrDestBuffer)
	{
		LOGGING_LINE LogString(eLog_Error,"readData called with Null buffer");
		return E_POINTER;
	}

	SmartRequestPointer req(NULL);
	if (!FindRequest(requestID,req,false,false,false,true))
	{
		LOGGING_LINE LogString(eLog_Error,"readData: No such requestID is completed: %lu - its either not yet completed or it doesnt exist.  Only completed requests can be read from",requestID);
		return ERROR_NOT_FOUND;
	}

	eRequestStatus stat = req->getStatus();

	if (stat != ePY_Completed)
	{
		LOGGING_LINE LogString(eLog_Error,"readData: Cannot read from request unless it successfully completed.  Current state is %lu",(unsigned long)stat);
		return ERROR_ACCESS_DENIED;
	}

	unsigned long actual_read = 0;
	if (!req->readData(dataLength,ptrDestBuffer,&actual_read))
	{
		LOGGING_LINE LogString(eLog_Error,"readData: Not enough data remains to fulfill request for %lu bytes", dataLength);
		return E_FAIL;
	}

	return S_OK;
}

HRESULT CPYDLLEngine::readString(const unsigned long requestID,const unsigned long stringBufferLength, char * const ptrBuffer)
{
	LOGGING_LINE LogString(eLog_Verbose,"readString(...)");
	if (m_bInitialized==false)
	{
		LOGGING_LINE LogString(eLog_Error,"Function called when not initialized.");
		return ERROR_NOT_READY;
	}
	if (stringBufferLength == 0)
	{
		LOGGING_LINE LogString(eLog_Error,"readString - 0 length buffer is invalid.");
		return E_INVALIDARG;
	}

	UINT16 length = 0;
	HRESULT res = readUINT16(requestID,&length);
	if (res != S_OK)
	{
		LOGGING_LINE LogString(eLog_Error,"readString - EOF reached or other error when trying to read 2 bytes from request.");
		return E_FAIL;
	}

	if (length >= stringBufferLength)
	{
		LOGGING_LINE LogString(eLog_Error,"readString - string is %lu bytes long but only %lu bytes were given.",(unsigned long)length+1,(unsigned long)stringBufferLength);
		return E_FAIL;
	}

	res = readData(requestID,length,ptrBuffer);
	ptrBuffer[length] = 0; // cap it with a null terminator.

	if (res != S_OK)
	{
		LOGGING_LINE LogString(eLog_Error,"readString - EOF reached or other error when trying to read 2 bytes from request.");
		return E_FAIL;
	}

	return res;
}

HRESULT CPYDLLEngine::readUINT8(const unsigned long requestID,UINT8 * const data)
{
	LOGGING_LINE LogString(eLog_Verbose,"readUINT8(...)");
	if (m_bInitialized==false)
	{
		LOGGING_LINE LogString(eLog_Error,"Function called when not initialized.");
		return ERROR_NOT_READY;
	}

	if (!data)
	{
		LOGGING_LINE LogString(eLog_Error,"readFLOAT32 called with null data pointer.");
		return E_POINTER;
	}
	HRESULT hr = readData(requestID,sizeof(UINT8),data);
	if (hr != S_OK) return hr;
	UTIL_EndianConvert(data,sizeof(UINT8));
	return S_OK;
}

HRESULT CPYDLLEngine::readUINT16(const unsigned long requestID,UINT16 * const data)
{
	LOGGING_LINE LogString(eLog_Verbose,"readUINT16(...)");
	if (m_bInitialized==false)
	{
		LOGGING_LINE LogString(eLog_Error,"Function called when not initialized.");
		return ERROR_NOT_READY;
	}

	if (!data)
	{
		LOGGING_LINE LogString(eLog_Error,"readUINT16 called with null data pointer.");
		return E_POINTER;
	}
	HRESULT hr = readData(requestID,sizeof(UINT16),data);
	if (hr != S_OK) return hr;
	UTIL_EndianConvert(data,sizeof(UINT16));
	return S_OK;
}

HRESULT CPYDLLEngine::readUINT32(const unsigned long requestID,UINT32 * const data)
{
	LOGGING_LINE LogString(eLog_Verbose,"readUINT32(...)");
	if (m_bInitialized==false)
	{
		LOGGING_LINE LogString(eLog_Error,"Function called when not initialized.");
		return ERROR_NOT_READY;
	}

	if (!data)
	{
		LOGGING_LINE LogString(eLog_Error,"readUINT32 called with null data pointer.");
		return E_POINTER;
	}
	HRESULT hr = readData(requestID,sizeof(UINT32),data);
	if (hr != S_OK) return hr;
	UTIL_EndianConvert(data,sizeof(UINT32));
	return S_OK;
}

HRESULT CPYDLLEngine::readFLOAT32(const unsigned long requestID,FLOAT32 * const data)
{
	LOGGING_LINE LogString(eLog_Verbose,"readFLOAT32(...)");
	if (m_bInitialized==false)
	{
		LOGGING_LINE LogString(eLog_Error,"Function called when not initialized.");
		return ERROR_NOT_READY;
	}

	if (!data)
	{
		LOGGING_LINE LogString(eLog_Error,"readFLOAT32 called with null data pointer.");
		return E_POINTER;
	}
	HRESULT hr = readData(requestID,sizeof(FLOAT32),data);
	if (hr != S_OK) return hr;
	UTIL_EndianConvert(data,sizeof(FLOAT32));
	return S_OK;
}

HRESULT CPYDLLEngine::readINT8(const unsigned long requestID,INT8 * const data)
{
	LOGGING_LINE LogString(eLog_Verbose,"readINT8(...)");
	if (m_bInitialized==false)
	{
		LOGGING_LINE LogString(eLog_Error,"Function called when not initialized.");
		return ERROR_NOT_READY;
	}

	if (!data)
	{
		LOGGING_LINE LogString(eLog_Error,"readINT8 called with null data pointer.");
		return E_POINTER;
	}
	HRESULT hr = readData(requestID,sizeof(INT8),data);
	if (hr != S_OK) return hr;
	UTIL_EndianConvert(data,sizeof(INT8));
	return S_OK;
}

HRESULT CPYDLLEngine::readINT16(const unsigned long requestID,INT16 * const data)
{
	LOGGING_LINE LogString(eLog_Verbose,"readINT16(...)");
	if (m_bInitialized==false)
	{
		LOGGING_LINE LogString(eLog_Error,"Function called when not initialized.");
		return ERROR_NOT_READY;
	}

	if (!data)
	{
		LOGGING_LINE LogString(eLog_Error,"readINT16 called with null data pointer.");
		return E_POINTER;
	}
	HRESULT hr = readData(requestID,sizeof(INT16),data);
	if (hr != S_OK) return hr;
	UTIL_EndianConvert(data,sizeof(INT16));
	return S_OK;
}

HRESULT CPYDLLEngine::readINT32(const unsigned long requestID,INT32 * const data)
{
	LOGGING_LINE LogString(eLog_Verbose,"readINT32(...)");
	if (m_bInitialized==false)
	{
		LOGGING_LINE LogString(eLog_Error,"Function called when not initialized.");
		return ERROR_NOT_READY;
	}

	if (!data)
	{
		LOGGING_LINE LogString(eLog_Error,"readINT32 called with null data pointer.");
		return E_POINTER;
	}
	HRESULT hr = readData(requestID,sizeof(INT32),data);
	if (hr != S_OK) return hr;
	UTIL_EndianConvert(data,sizeof(INT32));
	return S_OK;
}


HRESULT CPYDLLEngine::startProcessing()
{
	LOGGING_LINE LogString(eLog_Verbose,"startProcessing(...)");
	if (m_bInitialized==false)
	{
		LOGGING_LINE LogString(eLog_Error,"Function called when not initialized.");
		return ERROR_NOT_READY;
	}

	SetEvent(m_newItemEvent);

	m_bShouldBeProcessing;
	if (!m_bShouldBeProcessing)
	{
		LOGGING_LINE LogString(eLog_Verbose,"starting the processing thread...");
		m_bShouldBeProcessing = true;
#ifdef _XBOX
		m_workerThread = CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)&CPYDLLEngine::ProcessingThreadStarter,(void *)this,0,NULL);
#else
		m_workerThread = (HANDLE)_beginthread(CPYDLLEngine::ProcessingThreadStarter,0,(void *)this);
#endif
		LOGGING_LINE LogString(eLog_Verbose,"Thread Started with handle %x",m_workerThread);
	}
	else
	{
		LOGGING_LINE LogString(eLog_Warning,"startProcessing called when thread is already processing.");
	}
	
	return S_OK;
}

HRESULT CPYDLLEngine::stopProcessing(const bool waitForRemainingItems)
{
	LOGGING_LINE LogString(eLog_Verbose,"stopProcessing(...)");
	if (m_bInitialized==false)
	{
		LOGGING_LINE LogString(eLog_Error,"Function called when not initialized.");
		return ERROR_NOT_READY;
	}
	
	if (m_bShouldBeProcessing==false)
	{
		LOGGING_LINE LogString(eLog_Warning,"Processing is already stopped or stopping");
		return S_OK;
	}

	if (waitForRemainingItems==false)
	{
		// we need to clear the queue
		// simply marking an element as cancelled will automatically kill it.
		{
			AUTOLOCK(lock,&m_QueuedCriticalSection);
			for (RequestQueue::const_iterator pos = m_QueuedRequests.begin();pos != m_QueuedRequests.end();++pos)
			{
				pos->second->Cancel();
			}
		}
		
		// we need to cancel the currently working element too
		{
			AUTOLOCK(lock,&m_CurrentWorkingCriticalSection);
			if (m_CurrentlyWorkingItem.GetElement())
			{
				m_CurrentlyWorkingItem.GetElement()->Cancel();
			}
		}
	}

	LOGGING_LINE LogString(eLog_Verbose,"stopProcessing(...) : done cancelling all items");

	m_bShouldBeProcessing = false;

	LOGGING_LINE LogString(eLog_Verbose,"Notifying new item event...");
	SetEvent(m_newItemEvent);
	LOGGING_LINE LogString(eLog_Verbose,"Waiting for thread to end");

#ifdef _XBOX
	// Attempting to WaitForSingleObject on a thread in the XBOX will kill the entire system
	// if the thread has exited
	// unlike in windows, where it will simply return INVALID_HANDLE and all is fine.
	// lets make sure this code can NEVER freeze though
	DWORD startTime = ::GetTickCount();
	DWORD exitcode = STILL_ACTIVE;
	while (GetExitCodeThread(m_workerThread,&exitcode)==0)
	{
		if (exitcode!=STILL_ACTIVE)
			break;
		::Sleep(10);
		DWORD currentTime = GetTickCount();
		if ((currentTime - startTime) > 5000)
		{
			// stop waiting.  The unwinder will forcefully terminate remainders.
			return S_OK;
		}
	}

#else
	
	WaitForSingleObject(m_workerThread,5000); // wait for the thread to note that its been terminated.  wait max 60 secs.
#endif

	return S_OK;
}

HRESULT CPYDLLEngine::isProcessing(bool *outState) const
{
	LOGGING_LINE LogString(eLog_Verbose,"isProcessing(...)");
	if (m_bInitialized==false)
	{
		LOGGING_LINE LogString(eLog_Error,"Function called when not initialized.");
		return ERROR_NOT_READY;
	}

	*outState = m_bIsProcessing;

	return S_OK;
}

HRESULT CPYDLLEngine::cancelRequest(const unsigned long requestID)
{
	LOGGING_LINE LogString(eLog_Verbose,"cancelRequest(...)");
	if (m_bInitialized==false)
	{
		LOGGING_LINE LogString(eLog_Error,"Function called when not initialized.");
		return ERROR_NOT_READY;
	}

	// cancelling a request has no effect unless its state was in the following
	// ePY_NotQueuedYet ePY_Queued ePY_Processing

	// check for the element in the queue
	SmartRequestPointer req(NULL);
	if (!FindRequest(requestID,req,true,true,true,true))
	{
		LOGGING_LINE LogString(eLog_Error,"cancelRequest - request not found");
		return ERROR_NOT_FOUND;
	}

	req->Cancel();
	
	// here is a special case.  Cancelled requests may not remain in the pending queue.
	// they have to be removed from that queue and placed on the finished queue.
	{
		AUTOLOCK(lock,&m_PendingCriticalSection);
		RequestQueue::const_iterator pos = m_PendingRequests.find(requestID);
		if (pos != m_PendingRequests.end())
		{
			AUTOLOCK(lock2,&m_CompletedCriticalSection);
			m_CompletedRequests.insert(std::make_pair(requestID,pos->second.GetElement()));
			m_PendingRequests.erase(requestID);
		}
	}

	return S_OK;
}

HRESULT CPYDLLEngine::disposeRequest(const unsigned long requestID)
{
	LOGGING_LINE LogString(eLog_Verbose,"disposeRequest(...)");
	if (m_bInitialized==false)
	{
		LOGGING_LINE LogString(eLog_Error,"Function called when not initialized.");
		return ERROR_NOT_READY;
	}

	// this gets a bit tricky here.
	// the request may be in any queue.
	// we need to make it gone by removing it from any queue its currently in
	// except for the current request queue.
	// the problem is that the processing function will move it around while this is executing
	// we need to find a way for the processing function to discard it once it hits the endpoint... before even cancelling it.
	// like a garbage collector can run on the completed requests, for example.
	
	cancelRequest(requestID); // note - this function may block for a bit.
	SmartRequestPointer req(NULL);
	
	if (!FindRequest(requestID,req,true,true,true,true))
	{
		LOGGING_LINE LogString(eLog_Error,"disposeRequest could not find the request specified.");
		return ERROR_NOT_FOUND;
	}

	// set its status to garbage.
	{
		req->setGarbage();
	}

	return S_OK;
}

void CPYDLLEngine::ProcessingThreadStarter(void *context)
{
	if (!context) return;
	((CPYDLLEngine *)context)->ProcessingThread();
}

// -- This is the "big" processing thread that dispatches the requests --
// it runs on an infinite loop.
void CPYDLLEngine::ProcessingThread()
{
	bool proc = true;
	if (!m_bInitialized) return;

	// now we're processing.
	EnterCriticalSection(&m_flagsCriticalSection);
	m_bIsProcessing = true;
	LeaveCriticalSection(&m_flagsCriticalSection);

	time_t last_garbage_collect = time(NULL);
	
	while (1)
	{
		EnterCriticalSection(&m_flagsCriticalSection);
		proc = m_bShouldBeProcessing;
		LeaveCriticalSection(&m_flagsCriticalSection);
		if (!proc) 
		{
			if (m_QueuedRequests.size ()==0)
			{
				LOGGING_LINE LogString(eLog_Verbose,"The PY Processing thread is terminating gracefully as there are no remaining items to process.");
				break;
			}
			else
			{
				LOGGING_LINE LogString(eLog_ExtraVerbose,"The PY Processing thread is terminating, but first waiting for %i elements to process..",(int)m_QueuedRequests.size());
			}
		}

		// find the next queued request and make it into the working request.
		bool init = false;
		
		EnterCriticalSection(&m_flagsCriticalSection);
		init = m_bInitialized;
		// we also grab the local address and server address
		// here, during the flags critical section
		// which is also engaged when these are changing, in case the rug is pulled out from under us.
		SOCKADDR_IN saServerAddress = m_ServerAddress;
		SOCKADDR_IN saLocalSocketBind = m_LocalSocketBind; 
		LeaveCriticalSection(&m_flagsCriticalSection);

		// if init is false it means the engine is restarting
		// engine restart only takes as long as it takes to look up a hostname
		// we'd rather pause the thread and have it sleep for a while.
		if (init==true) 
		{
			{
				AUTOLOCK(lock,&m_QueuedCriticalSection);

				// search the queue for the first element that is ready to process.
				// since the data needs to be processed FIFO, the first element should have the lowest number so we just use an iterator
				RequestQueue::const_iterator pos = m_QueuedRequests.begin();
				if (pos != m_QueuedRequests.end())
				{
					// we got a live one.
					AUTOLOCK(lock2,&m_CurrentWorkingCriticalSection);
					m_CurrentlyWorkingItem = pos->second.GetElement();
					m_QueuedRequests.erase(m_CurrentlyWorkingItem->getRequestID());
					if (m_CurrentlyWorkingItem->getStatus()==ePY_Queued)
						m_CurrentlyWorkingItem->setStatus(ePY_Processing);

				}
			}

			// since this is the only function that is allowed to change the reference
			// of the currently working item we do not have to lock it EXCEPT when
			// actaully changing the reference it points to.
			if (m_CurrentlyWorkingItem.GetElement())
			{
				// process the item here - do the socket connection, etc.
				// when this function returns the item should be fully processed.
				try
				{
					m_CurrentlyWorkingItem->Process(saLocalSocketBind,saServerAddress,m_ResponseTimeoutMS);
				}
				catch (std::exception exp)
				{
					LOGGING_LINE LogString(eLog_Error,"Exception during request proces: %s",exp.what());
				}
				
				
				// this means that its set to a completed state.
				if ((m_CurrentlyWorkingItem->getStatus()!=ePY_Completed)&&(m_CurrentlyWorkingItem->getStatus()!=ePY_Cancelled))
				{
					// a request failed or broke halfway through.  Whatever the reason, we fail it.
					m_CurrentlyWorkingItem->setStatus(ePY_Failed);
				}
			
				// drop it on the done queue
				{
					if (m_CurrentlyWorkingItem->isExpired(m_RequestExpirationTimeoutMS / 1000))
						m_CurrentlyWorkingItem->setGarbage();

					if (m_CurrentlyWorkingItem->getGarbage()==false)
					{
						AUTOLOCK(lock2,&m_CompletedCriticalSection);
						m_CompletedRequests.insert(std::make_pair(m_CurrentlyWorkingItem->getRequestID(),m_CurrentlyWorkingItem.GetElement()));
					}
					AUTOLOCK(lock,&m_CurrentWorkingCriticalSection);
					m_CurrentlyWorkingItem = NULL;
				}
			}
		}

		// lets do a garbage collection sweep every 2 seconds or so.
		// best to do a little, but often, than a lot, rarely.
		// (ie, best to drop 1 fps smoothly, than lock up for a few seconds every now and then)
		time_t new_time = time(NULL);
		if (new_time - last_garbage_collect > 1)
		{
			last_garbage_collect = new_time;
			GarbageCollect();
		}
	
		// we do not wait if there are requests still waiting or if the engine is reinitialized
		if ((m_QueuedRequests.size() == 0)||(init==false)) 
		{
			DWORD result = WaitForSingleObject(m_newItemEvent,1000);
		}
		else
		{
			// sleep zero allows other threads to run if they need it
			// but doesn't actually spend any time waiting.  Its basically 
			// a yield() type operation.
			// this prevents stuttering if a game is using this library and a lot of things are queued.
			// since its more important for it to keep rendering smoothly.  A couple milliseconds
			// delay on the request is no matter.
			Sleep(0);
		}
	}

	LOGGING_LINE LogString(eLog_Verbose,"Shutting down PY Processing thread...");

	EnterCriticalSection(&m_flagsCriticalSection);
	m_bIsProcessing = false;
	LeaveCriticalSection(&m_flagsCriticalSection);

	LOGGING_LINE LogString(eLog_Verbose,"PY Processing thread exit");

}

bool CPYDLLEngine::FindRequest(unsigned long requestID,SmartRequestPointer &dest,bool search_pending,bool search_queued,bool search_current,bool search_completed)
{
	if (search_current)
	{
		AUTOLOCK(lock,&m_CurrentWorkingCriticalSection);
		if (m_CurrentlyWorkingItem.GetElement())
		{
			if (m_CurrentlyWorkingItem->getRequestID()==requestID)
			{
				if (m_CurrentlyWorkingItem->getGarbage()==false)
				{
					dest = m_CurrentlyWorkingItem;
					return true;
				}
			}
		}
	}

	if (search_pending)
	{
		// lock the structure so it does not change during this operation
		AUTOLOCK(lock,&m_PendingCriticalSection);
		RequestQueue::const_iterator pos = m_PendingRequests.find(requestID);
		if (pos != m_PendingRequests.end())
		{
			// found it!
			if (pos->second->getGarbage()==false)
			{
				dest = pos->second.GetElement();
				return true;
			}
		}
	}

	if (search_queued)
	{
		// lock the structure so it does not change during this operation
		AUTOLOCK(lock,&m_QueuedCriticalSection);
		RequestQueue::const_iterator pos = m_QueuedRequests.find(requestID);
		if (pos != m_QueuedRequests.end())
		{
			if (pos->second->getGarbage()==false)
			{
				// found it!
				dest = pos->second.GetElement();
				return true;
			}
		}
	}

	if (search_completed)
	{
		// lock the structure so it does not change during this operation
		AUTOLOCK(lock,&m_CompletedCriticalSection);
		RequestQueue::const_iterator pos = m_CompletedRequests.find(requestID);
		if (pos != m_CompletedRequests.end())
		{
			if (pos->second->getGarbage()==false)
			{
				// found it!
				dest = pos->second.GetElement();
				return true;
			}
		}
	}
	// not found.
	return false;
}


bool IsGarbage(std::pair<unsigned long,SmartRequestPointer> targ)
{
	return targ.second->getGarbage();
}

void CPYDLLEngine::GarbageCollect()
{
	// this function performs expiration house cleaning.
	// it only has to check the pending requests and completed requests.
	// queued and current requests which become garbage will automatically be taken care of.
	{
		AUTOLOCK(lock,&m_PendingCriticalSection);

		for (RequestQueue::iterator pos = m_PendingRequests.begin();pos != m_PendingRequests.end();)
		{
			if (pos->second->isExpired(m_RequestExpirationTimeoutMS / 1000))
			{
				LOGGING_LINE LogString(eLog_ExtraVerbose,"expiring request due to timeout: %lu.",pos->second->getRequestID());
				pos->second->setGarbage();
			}

			if (pos->second->getGarbage())
			{
				LOGGING_LINE LogString(eLog_ExtraVerbose,"Garbage collecting request %lu.",pos->second->getRequestID());
				m_PendingRequests.erase(pos++); 
				// NOTE THE POST INCREMENT - remember that (value++) first increments the value, but then returns the value before incrementing!
				// This expands to the following
				// temp = pos;pos++;erase(temp); 
				// Otherwise we'd be 'sawing off the branch we're sitting on'. 
			}
			else
			{
				++pos;
			}
		}
	}

	{
		AUTOLOCK(lock,&m_CompletedCriticalSection);
		for (RequestQueue::iterator pos = m_CompletedRequests.begin();pos != m_CompletedRequests.end();)
		{
			if (pos->second->isExpired(m_RequestExpirationTimeoutMS / 1000))
			{
				LOGGING_LINE LogString(eLog_ExtraVerbose,"Expiring request %lu due to timeout.",pos->second->getRequestID());
				pos->second->setGarbage();
			}

			if (pos->second->getGarbage())
			{
				LOGGING_LINE LogString(eLog_ExtraVerbose,"Garbage collecting request %lu.",pos->second->getRequestID());
				m_CompletedRequests.erase(pos++); // post increment or die!
			}
			else
			{
				++pos;
			}
		}
	}
}

HRESULT CPYDLLEngine::unsubmittedCount(unsigned long * output)
{	
	if (!output) return E_POINTER;
	AUTOLOCK(lock,&m_PendingCriticalSection);
	*output = (unsigned long)m_PendingRequests.size();
	return S_OK;
}

HRESULT CPYDLLEngine::queueCount(unsigned long * output)
{
	if (!output) return E_POINTER;
	AUTOLOCK(lock,&m_QueuedCriticalSection);
	*output = (unsigned long)m_QueuedRequests.size();
	return S_OK;
}

HRESULT CPYDLLEngine::completedCount(unsigned long * output)
{
	if (!output) return E_POINTER;
	AUTOLOCK(lock,&m_CompletedCriticalSection);
	*output = (unsigned long)m_CompletedRequests.size();
	return S_OK;
}

HRESULT CPYDLLEngine::LoadCryptoKeys(const unsigned char *const encryptionKey,const unsigned long encryptionKeyLength)
{
	try
	{
		m_pCrypto->LoadKeys(encryptionKey,encryptionKeyLength);
	}
	catch(std::exception stdexcp)
	{
		// probably a crypto exception
		LOGGING_LINE LogString(eLog_Error,"Unable to load the given crypto keys: Exception details: %s",stdexcp.what());

		return E_INVALIDARG;
	}
	return S_OK;
}