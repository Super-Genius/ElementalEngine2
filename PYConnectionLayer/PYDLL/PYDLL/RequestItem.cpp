#include "StdAfx.h"
#include "pydll.h"
#include "PYDLLEngine.h"
#include "RequestItem.h"
#include "CryptoLayer.h"

// if you uncomment this, stress testing CAN flood you.
#define DISALLOW_FLOODING

// how long to wait between each request, before the next is allowed (in milliseconds)
// setting this too low will allow flooding to occur.
// setting this too high will mean thats the maximum rate of requests per second per client
// if you dont expect a lot of requests per second (RECOMMENDED!) set it to a reasonable delay like 250
#define WAIT_TIME_BETWEEN_SENDS 100

CRequestItem::CRequestItem(const bool encryption,const unsigned long requestType,const unsigned long requestID,CPYDLLEngine *parent) :
	m_ulRequestID(requestID),
	m_bUsingEncryption(encryption),
	m_pParent(parent),
	m_ulRequestType(requestType),
	m_status(ePY_NotQueuedYet),
	m_pUserData(NULL),
	m_ulUserDataLength(0),
	m_iRefCount(0),
	m_bGarbage(false),
	m_ConnectionSocket(0)
{
#ifdef PREALLOCATE_DATA_SPACE
	m_Data.reserve(DATA_PREALLOCATION_AMOUNT); // we do this for speed, up front.  Most requests will be smaller than this, in bytes.
#endif
	m_LastTouchTime = time(NULL);
	m_MagicHeader[0] = 'P';
	m_MagicHeader[1] = 'H';
	m_MagicHeader[2] = 'A';
	m_MagicHeader[3] = 'T';
}

CRequestItem::~CRequestItem(void)
{
	if (m_pUserData) delete [] m_pUserData;
}
void CRequestItem::copyUserData(const unsigned long bytes,void * const destination)
{
	unsigned long howmany = min(bytes,m_ulUserDataLength);
	if (howmany == 0) return;
	memcpy(destination,m_pUserData,howmany);

	m_LastTouchTime = time(NULL);

}

void CRequestItem::setUserData(const unsigned long bytes,const void * const source)
{
	if (m_pUserData ) delete [] m_pUserData;
	m_pUserData = NULL;
	
	if (bytes>0)
	{
		m_pUserData = new unsigned char[bytes];
		memcpy(m_pUserData,source,bytes);
		m_ulUserDataLength = bytes;
	}
	m_LastTouchTime = time(NULL);
}

bool CRequestItem::addData(const unsigned long bytes,const void * const source)
{
	if (!source) return false;
	if (bytes==0) return false;
	if (m_status != ePY_NotQueuedYet) return false;

	if ((m_Data.size() + bytes) >= MAXIMUM_MESSAGE_SIZE)
	{
		return false;
	}

	const unsigned char * const srcptr = (const unsigned char * const)source;
	for (unsigned long a=0;a<bytes;++a)
	{
		m_Data.push_back(srcptr[a]);
	}
	m_LastTouchTime = time(NULL);
	return true;
}

bool CRequestItem::readData(const unsigned long requestBytes,void * const destination, unsigned long * const actualBytes)
{
	if (!destination) return false;
	if (!actualBytes) return false;
	if (requestBytes==0) return false;
	
	if (m_status != ePY_Completed) return false;

	// completing the request initializes the reader.
	if (m_ResponseReader == m_ResponseData.end())
	{
		*actualBytes = 0;
		return false;
	}

	unsigned char * destptr = (unsigned char *)destination;
	for (unsigned long a=0;a<requestBytes;++a)
	{
		// read a byte in until you get to the end.
		destptr[0] = *m_ResponseReader;
		++destptr;
		++(*actualBytes);
		++m_ResponseReader;
		if (m_ResponseReader == m_ResponseData.end())
		{
			// we got exactly as many as we needed.
			if (requestBytes == *actualBytes) return true;
			// not enough data to fulfill request.
			return false;
		}
	}

	m_LastTouchTime = time(NULL);

	return true;

}


void CRequestItem::Cancel()
{
	
	
	// must cause the request to cancel.
	// warning: could be re-entrant.
	// warning: could be on a different thread to the Process() thread.
	// by the time this function exits, the thing must be done with.
	if (m_status == ePY_Completed) return; // no!
	if (m_status == ePY_Failed) return; // no!
	m_status = ePY_Cancelled;
	m_LastTouchTime = time(NULL);
	if (m_ConnectionSocket != 0)
		closesocket(m_ConnectionSocket);
}

bool CRequestItem::UTIL_CreateFreshSocket(SOCKET &outputsocket,const SOCKADDR_IN &localSocketAddr,const unsigned long connectionTimeoutMS)
{
	outputsocket = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
	timeval wait_time_inverval;
	LINGER lingervalue;
	lingervalue.l_onoff = 0;
	lingervalue.l_linger = 0;
	wait_time_inverval = UTIL_MSToTimeVal(connectionTimeoutMS);

	int reuse = 1;
	
	//int wres = setsockopt(outputsocket,SOL_SOCKET,SO_LINGER,(const char *)&lingervalue,sizeof(LINGER));
	int wres = setsockopt(outputsocket,SOL_SOCKET,SO_DONTLINGER,(const char *)&reuse,sizeof(reuse));
	wres = setsockopt(outputsocket,SOL_SOCKET,SO_SNDTIMEO,(const char *)&wait_time_inverval,sizeof(timeval));
	wres = setsockopt(outputsocket,SOL_SOCKET,SO_RCVTIMEO,(const char *)&wait_time_inverval,sizeof(timeval));

	if (bind(outputsocket,(struct sockaddr*)&localSocketAddr,sizeof(SOCKADDR_IN)) == SOCKET_ERROR)
	{
		LOGGING_LINE m_pParent->LogString(CPYDLLEngine::eLog_Error,"Unable to bind local socket.");
		return false;
	}
	return true;

}

void CRequestItem::Process(const SOCKADDR_IN &localSocketAddr,const SOCKADDR_IN &serverSocketAddr,const unsigned long connectionTimeoutMS)
{
	m_LastTouchTime = time(NULL);
	if (getGarbage())
		return;

	if (m_status == ePY_Cancelled)
		return;

	LOGGING_LINE m_pParent->LogString(CPYDLLEngine::eLog_Verbose,"Request %lu is starting to process...",m_ulRequestID);

	m_status = ePY_Processing;

	CCryptoLayer *crypt = m_pParent->getCryptoLayer();

	if (!crypt)
	{
		LOGGING_LINE m_pParent->LogString(CPYDLLEngine::eLog_Error,"Crypto layer not found.");
		m_status = ePY_Failed;
		return;
	}

	if (m_bUsingEncryption)
	{
		if (crypt->IsReady()==false)
		{
			LOGGING_LINE m_pParent->LogString(CPYDLLEngine::eLog_Error,"Crypto layer is not ready.");
			m_status = ePY_Failed;
			return;
		}
	}

	// warning: cancel might be called at any time during this.
	// warning: called on a different thread to all other functions.
	//          lock appropriately
	// data does not need to be locked, as its not allowed to retrieve data unless
	// in a completed state and its not allowed to add data unless its before processing.

	// first, we finalize things by packing the data up in a nice block to send all at once.
	// stream format is:
	// [PHAT][uint32 type][uint32 size][uint8 crypto][bytearray of data]
	//     size does not include the cryto block, only the bytearray
	//     crypto flag will be an ODD NUMBER if crypto is turned on (1,3,5,7,9... etc).  It can be random.  Only its evenness counts
	// When the packet is ENCRYPTED, the format of the header stays the same but the bytearray changes:

	// note that because of java limitations the actual maximum size is MAX_INT not MAX_UINT.

	// [bytearray] = {rijndael 16 byte key} + { [PHAT][DECRYPTED_SIZE UINT32][DATA][PADDING] }
	//              key encrypted using RSA              payload encrypted with key
	
	LOGGING_LINE m_pParent->LogString(CPYDLLEngine::eLog_ExtraVerbose,"Request %lu: Packing header and data",m_ulRequestID);

	s_PYPacketHeader m_Header;
	s_PYPacketHeader m_WrittenHeader; // after endian conversion

	memcpy(m_Header.m_szMAGIC,m_MagicHeader,4);

	// REMEMBER TO ENDIAN CONVERT
	m_Header.m_ulType = m_ulRequestType;
	m_Header.m_ulSizeOfDataBlob = (unsigned long)m_Data.size();
	m_Header.m_ucCrypto = crypt->GetCryptographicallyStrongRandomByte();
	while ((m_Header.m_ucCrypto % 2) != 0) m_Header.m_ucCrypto++; // allow overflow, wraparound, and make it an even number

	
	// even numbers mean NO ENCRYPTION.

	unsigned char *datablob = NULL;
	if (m_Header.m_ulSizeOfDataBlob > 0)
	{
		datablob = new unsigned char[m_Header.m_ulSizeOfDataBlob];
		unsigned char *ptrvalue = &datablob[0];
		for (std::vector<unsigned char>::iterator currentbyte = m_Data.begin();currentbyte != m_Data.end();++currentbyte)
		{
			*ptrvalue = *currentbyte;
			++ptrvalue;
		}
	}

	LOGGING_LINE m_pParent->LogString(CPYDLLEngine::eLog_ExtraVerbose,"Request %lu: HEader size: %lu - data size: %lu",m_ulRequestID,sizeof(m_Header),m_Header.m_ulSizeOfDataBlob);

	fd_set writeset;
	fd_set readset;
	timeval wait_time_inverval;
	wait_time_inverval = UTIL_MSToTimeVal(connectionTimeoutMS);

	try
	{
		// now, if encryption is enabled, we need to encrypt the payload
		// the header changes.
		// note that its valid to have a zero payload (just a header)
		// for eg when a client needs to poll for a data or something
		// or run a heartbeat (this should be VERY RARE)

		if ((m_bUsingEncryption)&&(m_Header.m_ulSizeOfDataBlob > 0))
		{
			unsigned long newsize = crypt->GetEncryptedSize(m_Header.m_ulSizeOfDataBlob);
			if (newsize == 0)
			{
				LOGGING_LINE m_pParent->LogString(CPYDLLEngine::eLog_Error,"Encryption failed - size issue");
				throw std::exception("Encryption failed - size issue");
			}
			unsigned char *newbuffer = new unsigned char[newsize];
			if (crypt->Encrypt(datablob,m_Header.m_ulSizeOfDataBlob,newbuffer,newsize,m_ucEncryptionKeys)==false)
			{
				LOGGING_LINE m_pParent->LogString(CPYDLLEngine::eLog_Error,"Encryption failed - encryption issue");
				delete [] newbuffer;
				throw std::exception("Encryption failed - encryption issue");
			}

			delete [] datablob ;
			datablob = newbuffer;
			m_Header.m_ulSizeOfDataBlob = newsize;
			m_Header.m_ucCrypto = crypt->GetCryptographicallyStrongRandomByte();
			// now force it to be odd
			if (m_Header.m_ucCrypto % 2 == 0) m_Header.m_ucCrypto++; // allow overflow - odd numbers mean ENCRYPTION
		}

		// right - we now have the header and data blob set up.
		// we're ready to establish a connection and start sending.

		// Over here we retry up to 5 times to connect
		LOGGING_LINE m_pParent->LogString(CPYDLLEngine::eLog_ExtraVerbose,"Request %lu: Connecting to server... ",m_ulRequestID);
		for (int a=0;a<5;++a) // try to reconnect due to socket in use... this should only happen during STRESS TESTING!!
		{	
			SOCKET s;
			if (UTIL_CreateFreshSocket(s,localSocketAddr,connectionTimeoutMS))
			{
				m_ConnectionSocket = s;
				if (connect(m_ConnectionSocket,(struct sockaddr*)&serverSocketAddr,sizeof(SOCKADDR_IN)) != SOCKET_ERROR)
				{
					break;
				}
			}

			int err = WSAGetLastError();
			
			if ((a==4)||(err==WSAETIMEDOUT))
			{
				// failed for the last time.
				// note that tiemdout fails instantly
				// because TIMEDOUT indicated that the full repeats of the TCP/IP retry stack
				// already tried.  (Remember, TCP/IP auto-retries several times).
				LOGGING_LINE m_pParent->LogString(CPYDLLEngine::eLog_Error,"Could not connect to host: WSAError: %i",err);
				throw std::exception("Could not connect to host");
			}
			else
			{
				// the reason it gets here is usualy because there are no addresses in use
				// we wait a moment for addresses to clear.
				closesocket(m_ConnectionSocket);
				m_ConnectionSocket = 0;
				LOGGING_LINE m_pParent->LogString(CPYDLLEngine::eLog_Warning,"Could not connect to host (%i) - retrying in 1 sec",err);
				::Sleep(1000); // wait for a short time, for a socket to become available.
			}
		}

		wait_time_inverval = UTIL_MSToTimeVal(connectionTimeoutMS);
		writeset.fd_array[0] = m_ConnectionSocket;
		writeset.fd_count = 1;

		LOGGING_LINE m_pParent->LogString(CPYDLLEngine::eLog_ExtraVerbose,"Request %lu: Waiting for a clear pipe ",m_ulRequestID);

		int selectvalue = select(0,NULL,&writeset,NULL,&wait_time_inverval);
		if ((selectvalue<=0)||(m_status != ePY_Processing))
		{
			// we failed to get a clear channel
			LOGGING_LINE m_pParent->LogString(CPYDLLEngine::eLog_Error,"socket did not become writeable in %lu ms",connectionTimeoutMS);
			throw std::exception("socket did not become writable in time to send header");
		}
		m_LastTouchTime = time(NULL); // reset the activity time.

		LOGGING_LINE m_pParent->LogString(CPYDLLEngine::eLog_ExtraVerbose,"Request %lu: Sending header... ",m_ulRequestID);
		// now we send!
		m_WrittenHeader = m_Header;
		m_pParent->UTIL_EndianConvert(&m_WrittenHeader.m_ulType,sizeof(m_WrittenHeader.m_ulType));
		m_pParent->UTIL_EndianConvert(&m_WrittenHeader.m_ulSizeOfDataBlob,sizeof(m_WrittenHeader.m_ulSizeOfDataBlob));
		m_pParent->UTIL_EndianConvert(&m_WrittenHeader.m_ucCrypto,sizeof(m_WrittenHeader.m_ucCrypto));

		if (UTIL_SafeSend((unsigned char *)&m_WrittenHeader,(unsigned long)sizeof(m_WrittenHeader))==false)
		{
			LOGGING_LINE m_pParent->LogString(CPYDLLEngine::eLog_Error,"Could not send header to server");
			throw std::exception("Unable to send header to server");
		}

		if (m_status != ePY_Processing)
		{
			LOGGING_LINE m_pParent->LogString(CPYDLLEngine::eLog_Warning,"Request cancelled during socket send.");
			throw std::exception("Request was cancelled during socket send");
		}
		m_LastTouchTime = time(NULL); // reset the activity time.
		
		if (m_Header.m_ulSizeOfDataBlob > 0)
		{
			LOGGING_LINE m_pParent->LogString(CPYDLLEngine::eLog_ExtraVerbose,"Request %lu: Sending data... (%lu bytes)",m_ulRequestID,m_Header.m_ulSizeOfDataBlob);
			// now we send!
			if (UTIL_SafeSend(datablob,m_Header.m_ulSizeOfDataBlob)==false)
			{
				LOGGING_LINE m_pParent->LogString(CPYDLLEngine::eLog_Error,"Could not send data to server");
				throw std::exception("Unable to send packet data to server");
			}
		}
		m_LastTouchTime = time(NULL); // reset the activity time.

		if (m_status != ePY_Processing)
		{
			LOGGING_LINE m_pParent->LogString(CPYDLLEngine::eLog_Warning,"Request cancelled during socket send.");
			throw std::exception("Request was cancelled during send of data");
		}

		// we're done at this point.  Now we wait for the server to respond (data will be incoming on the socket)

		wait_time_inverval = UTIL_MSToTimeVal(connectionTimeoutMS);
		readset.fd_array[0] = m_ConnectionSocket;
		readset.fd_count = 1;

		LOGGING_LINE m_pParent->LogString(CPYDLLEngine::eLog_ExtraVerbose,"Request %lu: waiting for server response... ",m_ulRequestID);
		selectvalue = select(0,&readset,NULL,NULL,&wait_time_inverval);

		if ((selectvalue<=0)||(m_status != ePY_Processing))
		{
			// we failed to get a clear channel
			LOGGING_LINE m_pParent->LogString(CPYDLLEngine::eLog_Error,"socket did not become readable in %lu ms",connectionTimeoutMS);
			throw std::exception("No response was placed on the socket within the time allotted, or the socket was closed unexpectedly");
		}

		LOGGING_LINE m_pParent->LogString(CPYDLLEngine::eLog_ExtraVerbose,"Request %lu: reading header... ",m_ulRequestID);
		// first lets read the response header.
		s_PYPacketHeader responseHeader;
		if (!UTIL_SafeRecv((unsigned char *)&responseHeader,sizeof(responseHeader)))
		{
			LOGGING_LINE m_pParent->LogString(CPYDLLEngine::eLog_Error,"Could not read response header from host in  %lu ms",connectionTimeoutMS);
			throw std::exception("Server did not respond within the time allotted, or the socket was closed during read");
		}

		if (m_status != ePY_Processing)
		{
			LOGGING_LINE m_pParent->LogString(CPYDLLEngine::eLog_Warning,"Request cancelled during header read.");
			throw std::exception("Request was cancelled during read-back");
		}

		if (strncmp(responseHeader.m_szMAGIC,m_MagicHeader,4)!=0)
		{
			LOGGING_LINE m_pParent->LogString(CPYDLLEngine::eLog_Error,"Bad request response header - check for bad port");
			throw std::exception("Request header was incorrect.  Check the port?");
		}

		m_pParent->UTIL_EndianConvert(&responseHeader.m_ucCrypto,sizeof(responseHeader.m_ucCrypto));
		m_pParent->UTIL_EndianConvert(&responseHeader.m_ulSizeOfDataBlob,sizeof(responseHeader.m_ulSizeOfDataBlob));
		m_pParent->UTIL_EndianConvert(&responseHeader.m_ulType,sizeof(responseHeader.m_ulType));

		if (responseHeader.m_ulType != m_ulRequestType)
		{
			LOGGING_LINE m_pParent->LogString(CPYDLLEngine::eLog_Error,"Response header was incorrect - response type did not match with request type.");
			throw std::exception("Response header was incorrect - response type did not match with request type.");
		}

		// it is not valid to have zero-length response data.
		if (responseHeader.m_ulSizeOfDataBlob==0)
		{
			LOGGING_LINE m_pParent->LogString(CPYDLLEngine::eLog_Error,"Response Header is not allowed to have zero size.");
			throw std::exception("Response header was incorrect - Had zero size");
		}

		LOGGING_LINE m_pParent->LogString(CPYDLLEngine::eLog_ExtraVerbose,"Request %lu: reading data (%lu bytes)... ",m_ulRequestID,responseHeader.m_ulSizeOfDataBlob);
		unsigned long datasize = responseHeader.m_ulSizeOfDataBlob;
		unsigned char *datablock = new unsigned char[datasize];
		if (UTIL_SafeRecv(datablock,datasize)==false)
		{
			LOGGING_LINE m_pParent->LogString(CPYDLLEngine::eLog_Error,"Could not read %lu bytes into response data block",datasize);
			delete [] datablock;
			throw std::exception("Could not read the data from the server.  Either it timed out, the socket was closed, or there simply wasn't enough data sent");
		}

		if (m_status != ePY_Processing)
		{
			LOGGING_LINE m_pParent->LogString(CPYDLLEngine::eLog_Warning,"Request cancelled during data read.");
			delete [] datablock;
			throw std::exception("The request was cancelled during readback");
		}

		// over here we'd decrypt it.  Note: decryption can in fact fail.

		if ((responseHeader.m_ucCrypto % 2) != 0)
		{
			// the crypto byte is odd.  This means its encrypted.
			unsigned long decryptedsize = crypt->GetDecryptedSize(datasize);
			if (decryptedsize==0)
			{
				// denied!
				LOGGING_LINE m_pParent->LogString(CPYDLLEngine::eLog_Warning,"Encryption payload size is invalid.");
				delete [] datablock;
				throw std::exception("The size of the encrypted payload is invalid");
			}
			unsigned char *decrypteddata = new unsigned char[decryptedsize];
			bool success = crypt->Decrypt(datablock,datasize,decrypteddata,decryptedsize,m_ucEncryptionKeys);
			delete [] datablock;
			datablock = decrypteddata;
			datasize = decryptedsize;
			if (!success)
			{
				LOGGING_LINE m_pParent->LogString(CPYDLLEngine::eLog_Warning,"Failed to decrypt server response.");
				delete [] datablock;
				throw std::exception("The server's response was encryptd, but failed to decrypt correctly");
			}
		}
		// alter the buffer and the datasize.
		// now that its all decrypted, we're going to consume it into our receive buffer.
		LOGGING_LINE m_pParent->LogString(CPYDLLEngine::eLog_ExtraVerbose,"Request %lu: writing data (%lu bytes) into reader buffer for client... ",m_ulRequestID,datasize);

		for (unsigned long ulpos = 0; ulpos < datasize;++ulpos)
		{
			m_ResponseData.push_back(datablock[ulpos]);
		}

		delete [] datablock;
		

	}
	catch (std::exception exc)
	{
		LOGGING_LINE m_pParent->LogString(CPYDLLEngine::eLog_Error,"Error details: %s",exc.what());
		if (m_ConnectionSocket!=0)
			closesocket(m_ConnectionSocket);
		m_ConnectionSocket = 0;
		if (m_status == ePY_Processing) // if it wasnt cancelled...
			m_status = ePY_Failed; // connection error!
		else
			LOGGING_LINE m_pParent->LogString(CPYDLLEngine::eLog_Warning,"This job was cancelled.");
	}

	if (m_ConnectionSocket != 0)
		closesocket(m_ConnectionSocket);
	m_ConnectionSocket = 0;

#ifdef DISALLOW_FLOODING
	::Sleep(WAIT_TIME_BETWEEN_SENDS);
#endif

	//::Sleep(1);

	// always clean up!
	if (datablob) delete [] datablob;
	
	// now set the reader up.
	m_ResponseReader = m_ResponseData.begin();
	// whenever changing the status first check to see if its been cancelled before changing it.
	m_LastTouchTime = time(NULL); // reset the activity time.
	
	if (m_status == ePY_Processing)
		m_status = ePY_Completed;
}


bool CRequestItem::isExpired(unsigned long maxSeconds)
{
	time_t time_now = time(NULL);
	time_t seconds = time_now - m_LastTouchTime;

	if (seconds > maxSeconds) return true;

	return false;
}


// converts milliseconds to TIMEVAL stucture which is seconds and microseconds.
timeval CRequestItem::UTIL_MSToTimeVal(const unsigned long milliseconds) 
{
	unsigned long ms = milliseconds;
	unsigned long seconds = ms / 1000; // compute seconds.
	ms -= (seconds * 1000); // remove seconds from MS
	timeval val; 
	val.tv_sec = (long)seconds; // seconds
	val.tv_usec = (long)(ms * 1000); // microseconds are 1000 times smaller than seconds.
	return val;
}

// safely send data on a given socket.  Sends it all until there's nothing left to send or an error occured.
bool CRequestItem::UTIL_SafeSend(const unsigned char * const data,const unsigned long datalength)
{
	if (datalength == 0) return true;

	if (!data) return false;

	unsigned long pos = 0;
	unsigned long bytes_remaining = datalength;
	
	while (bytes_remaining > 0)
	{
		unsigned long bytes_send = bytes_remaining; 

		// convert down to 32k chunks - this is probably larger than the tcp window anyway
		// so bigger chunk is probably actually worse.
		if (bytes_send > 32000) bytes_send = 32000; 
		
		int sendvalue = send(m_ConnectionSocket,(const char *)&data[pos],(int)bytes_send,0);
		if ((sendvalue == SOCKET_ERROR)||(m_status != ePY_Processing))
		{
			// an error occurred or it was cancelled.
			return false;
		}
		bytes_remaining -= sendvalue;
		pos += sendvalue;

		if (sendvalue > 0)
		{
			m_LastTouchTime = time(NULL); // we got a couple bytes, so its okay!
		}
	}
	return true;
}

bool CRequestItem::UTIL_SafeRecv(unsigned char * const data,const unsigned long datalength)
{
	if (datalength == 0) return true;
	if (!data) return false;

	unsigned long pos = 0;
	unsigned long bytes_remaining = datalength;
	
	while (bytes_remaining > 0)
	{
		unsigned long bytes_send = bytes_remaining; 

		// convert down to 32k chunks - this is probably larger than the tcp window anyway
		// so bigger chunk is probably actually worse.
		if (bytes_send > 32000) bytes_send = 32000; 
		
		int sendvalue = recv(m_ConnectionSocket,(char * const)&data[pos],(int)bytes_send,0);
		if ((sendvalue < 1)||(m_status != ePY_Processing))
		{
			// an error occurred or it was cancelled.
			return false;
		}
		bytes_remaining -= sendvalue;
		pos += sendvalue;

		if (sendvalue > 0)
		{
			m_LastTouchTime = time(NULL); // we got a couple bytes, so its okay!
		}
	}
	
	return true;
}