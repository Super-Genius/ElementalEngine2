// MCPDlll.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#include "tchar.h"
#include "pydll.h"
#include "PYDLLEngine.h"

#ifdef _MANAGED
#pragma managed(push, off)
#endif

using namespace std;


BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
    return TRUE;
}

#ifdef _MANAGED
#pragma managed(pop)
#endif

// ---------------------------- THE ENGINE ------------------------------------
CPYDLLEngine g_Engine;

// --------------------- MAIN DLL INTERFACE GOES HERE --------------------------
// Pretty much all these functions do is route the call directly to the main engine anyway.
// This allows flexibility for later - the ability to run multiple engines
// for, say, unit testing, or to rework the API without having to constantly 
// edit code in here.

// Please see the pydll.h file for a long explanation of each function.
HRESULT PYDLL_initializePYConnection(const char * const serverAddress, const unsigned short serverPort)
{
	return g_Engine.initializePYConnection(serverAddress,serverPort);
}

HRESULT PYDLL_loadCryptoKeys(const unsigned char *const encryptionKey,const unsigned long encryptionKeyLength)
{
	return g_Engine.LoadCryptoKeys(encryptionKey,encryptionKeyLength);
}

HRESULT PYDLL_setResponseTimeoutMS(const unsigned long milliseconds)
{
	return g_Engine.setResponseTimeoutMS(milliseconds);
}

HRESULT PYDLL_setRequestExpirationTimeoutMS(const unsigned long milliseconds)
{
	return g_Engine.setRequestExpirationTimeoutMS(milliseconds);
}

HRESULT PYDLL_createNewRequest(const bool enableEncryption, const unsigned long requestType,unsigned long * const newRequestID)
{
	return g_Engine.createNewRequest(enableEncryption,requestType,newRequestID);
}

HRESULT PYDLL_setRequestUserData(const unsigned long requestID,const unsigned long datalength,const void * const dataBuffer)
{
	return g_Engine.setRequestUserData(requestID,datalength,dataBuffer);
}

HRESULT PYDLL_getRequestUserData(const unsigned long requestID,unsigned long *const datalength,void * const dataBuffer)
{
	return g_Engine.getRequestUserData(requestID,datalength,dataBuffer);
}

HRESULT PYDLL_getRequestStatus(const unsigned long requestID,eRequestStatus *status) 
{
	return g_Engine.getRequestStatus(requestID,status);
}

HRESULT PYDLL_submitRequest(const unsigned long requestID)
{
	return g_Engine.submitRequest(requestID);
}

HRESULT PYDLL_getRequestType(const unsigned long requestID,unsigned long * const requestType)
{
	return g_Engine.getRequestType(requestID,requestType);
}

HRESULT PYDLL_getNextFinishedRequestID(unsigned long * const requestID)
{
	return g_Engine.getNextFinishedRequestID(requestID);
}

HRESULT PYDLL_writeData(const unsigned long requestID,const unsigned long dataLength,const void * const ptrData)
{
	return g_Engine.writeData(requestID,dataLength,ptrData);
}

HRESULT PYDLL_writeString(const unsigned long requestID,const char *const stringptr)
{
	return g_Engine.writeString(requestID,stringptr);
}

HRESULT PYDLL_writeUINT8(const unsigned long requestID,const UINT8 data)
{
	return g_Engine.writeUINT8(requestID,data);
}

HRESULT PYDLL_writeUINT16(const unsigned long requestID,const UINT16 data)
{
	return g_Engine.writeUINT16(requestID,data);
}

HRESULT PYDLL_writeUINT32(const unsigned long requestID,const UINT32 data)
{
	return g_Engine.writeUINT32(requestID,data);
}

HRESULT PYDLL_writeFLOAT32(const unsigned long requestID,const FLOAT32 data)
{
	return g_Engine.writeFLOAT32(requestID,data);
}

HRESULT PYDLL_writeINT8(const unsigned long requestID,const INT8 data)
{
	return g_Engine.writeINT8(requestID,data);
}

HRESULT PYDLL_writeINT16(const unsigned long requestID,const INT16 data)
{
	return g_Engine.writeINT16(requestID,data);
}

HRESULT PYDLL_writeINT32(const unsigned long requestID,const INT32 data)
{
	return g_Engine.writeINT32(requestID,data);
}


HRESULT PYDLL_readData(const unsigned long requestID,const unsigned long dataLength, void * const ptrDestBuffer)
{
	return g_Engine.readData(requestID,dataLength,ptrDestBuffer);
}

HRESULT PYDLL_readString(const unsigned long requestID,const unsigned long stringBufferLength, char * const ptrBuffer)
{
	return g_Engine.readString(requestID,stringBufferLength,ptrBuffer);
}

HRESULT PYDLL_readUINT8(const unsigned long requestID,UINT8 * const data)
{
	return g_Engine.readUINT8(requestID,data);
}

HRESULT PYDLL_readUINT16(const unsigned long requestID,UINT16 * const data)
{
	return g_Engine.readUINT16(requestID,data);
}

HRESULT PYDLL_readUINT32(const unsigned long requestID,UINT32 * const data)
{
	return g_Engine.readUINT32(requestID,data);
}

HRESULT PYDLL_readFLOAT32(const unsigned long requestID,FLOAT32 * const data)
{
	return g_Engine.readFLOAT32(requestID,data);
}

HRESULT PYDLL_readINT8(const unsigned long requestID,INT8 * const data)
{
	return g_Engine.readINT8(requestID,data);
}

HRESULT PYDLL_readINT16(const unsigned long requestID,INT16 * const data)
{
	return g_Engine.readINT16(requestID,data);
}

HRESULT PYDLL_readINT32(const unsigned long requestID,INT32 * const data)
{
	return g_Engine.readINT32(requestID,data);
}

HRESULT PYDLL_startProcessing()
{
	return g_Engine.startProcessing();
}

HRESULT PYDLL_stopProcessing(const bool waitForRemainingItems)
{
	return g_Engine.stopProcessing(waitForRemainingItems);
}

HRESULT PYDLL_isProcessing(bool *outState) 
{
	return g_Engine.isProcessing(outState);
}

HRESULT PYDLL_cancelRequest(const unsigned long requestID)
{
	return g_Engine.cancelRequest(requestID);
}

HRESULT PYDLL_disposeRequest(const unsigned long requestID)
{
	return g_Engine.disposeRequest(requestID);
}

HRESULT PYDLL_unsubmittedCount(unsigned long *resultout)
{
	return g_Engine.unsubmittedCount(resultout);
}

HRESULT PYDLL_queueCount(unsigned long *resultout)
{
	return g_Engine.queueCount(resultout);
}


HRESULT PYDLL_completedCount(unsigned long *resultout)
{
	return g_Engine.completedCount(resultout);
}

bool PYDLL_IsTerminalStatus(const eRequestStatus stat)
{
	if (stat==ePY_Queued) return false;
	if (stat==ePY_Processing) return false;

	return true;
}

bool PYDLL_IsErrorStatus(const eRequestStatus stat)
{
	if (stat==ePY_UnknownRequest) return true;
	if (stat==ePY_Cancelled) return true;
	if (stat==ePY_Failed) return true;
	return false;
}


