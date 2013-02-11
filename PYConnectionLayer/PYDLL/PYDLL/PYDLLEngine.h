#pragma once

// comment out the following line to squeeze the very last
// drops of performance out of this thing
// at the cost of the ability to log.
// We do this because logging strings, even just the function calls, for tight, verbose inner loops
// that is spewing out the data, can be a significant overhead, even if the logging function doesnt
// actually do anything with the data.
#define LOGGING_ENABLED

// the safest way to make something turn into a comment depending on a condition
// other ways perform badly in the presence of strings or other situations
#ifdef LOGGING_ENABLED
	#define LOGGING_LINE if (1)
#else
	#define SLASH_PARAM( n ) /##n
	#define LOGGING_LINE SLASH_PARAM(/)
#endif

// This is the main PhatYaffle communication dll engine.
// this essentially the core
#include "RequestItem.h"

typedef SmartContainerPTR<CRequestItem> SmartRequestPointer;
typedef std::map< unsigned long,SmartRequestPointer> RequestQueue;
class CCryptoLayer;

class CPYDLLEngine
{
public:
	enum eEndianNess
	{
		eEndian_Unknown,
		eEndian_LittleEndian, // standard intel endian-ness
		eEndian_BigEndian, // others
	};

	enum eLogSeverity
	{
		eLog_None = 0,
		eLog_Error = 1,
		eLog_Warning = 2,
		eLog_Verbose = 3,
		eLog_ExtraVerbose = 4,
	};

#ifdef LOGGING_ENABLED
protected:
	CRITICAL_SECTION m_loggingCriticalSection; // used to serialize the log entries
	CCryptoLayer *m_pCrypto;
#endif
protected:
	// Volatile members that can change from any thread 
	volatile bool m_bInitialized;
	volatile bool m_bShouldBeProcessing;
	volatile bool m_bIsProcessing;
	CRITICAL_SECTION m_flagsCriticalSection; // used to access volatile members such as whether its processing

	// the Queues.
	RequestQueue m_PendingRequests;
	RequestQueue m_QueuedRequests;
	SmartRequestPointer m_CurrentlyWorkingItem;
	RequestQueue m_CompletedRequests;

	// each of these critical sections protect their individual pieces.
	// This granularity allows us to lock only the segment or structure
	// that is currently being worked on.
	CRITICAL_SECTION m_PendingCriticalSection; 
	CRITICAL_SECTION m_QueuedCriticalSection; 
	CRITICAL_SECTION m_CurrentWorkingCriticalSection; 
	CRITICAL_SECTION m_CompletedCriticalSection;

	HANDLE m_newItemEvent; // signal raised when theres fresh items in the queue - avoids a sleep
	HANDLE m_workerThread; 

	unsigned int m_endianNess;

	eLogSeverity m_logLevel; // set to 0 for no logging, 1 for only errors, 2 for errors and warnings, etc
	
	SOCKADDR_IN m_ServerAddress;
	SOCKADDR_IN m_LocalSocketBind; // this is in case the client has multiple network cards and would like to select a specific one

	unsigned long m_ulNextRequestID;

	unsigned long m_ResponseTimeoutMS;
	unsigned long m_RequestExpirationTimeoutMS;

	// -- Private / Protected utility functions
	unsigned long UTIL_ConvertHostnameToADDR(const char *const hostname) const;
	

	static void ProcessingThreadStarter(void *context);

	void ProcessingThread();
	bool FindRequest(unsigned long requestID,SmartRequestPointer &dest,bool search_pending,bool search_queued,bool search_current,bool search_completed);

	void GarbageCollect();

public:
	CPYDLLEngine(void);
	virtual ~CPYDLLEngine();

	bool UTIL_EndianConvert(void * const data,const unsigned long datalength) const;

	CCryptoLayer *getCryptoLayer();

#ifdef LOGGING_ENABLED
	// we hide this in an ifndef so that if programmers forget to 
	// put LOGGING_LINE in front of their logging or use
	// LOGGING_ENABLED to protect from it, it gives them a compiler error
	void LogString(const eLogSeverity level,const char * const formatstr,...) const;
#endif
	// DLL interface functions:
	// please see pydll.h for a better explanation of these (avoiding duplicate documentation is good)
	HRESULT initializePYConnection(const char * const serverAddress, const unsigned short serverPort);
	HRESULT LoadCryptoKeys(const unsigned char *const encryptionKey,const unsigned long encryptionKeyLength);
	HRESULT setResponseTimeoutMS(const unsigned long milliseconds);
	HRESULT setRequestExpirationTimeoutMS(const unsigned long milliseconds);
	HRESULT createNewRequest(const bool enableEncryption, const unsigned long requestType,unsigned long * const newRequestID);
	
	HRESULT setRequestUserData(const unsigned long requestID,const unsigned long datalength,const void * const dataBuffer);
	HRESULT getRequestUserData(const unsigned long requestID,unsigned long *const datalength,void * const dataBuffer);

	HRESULT getRequestStatus(const unsigned long requestID,eRequestStatus *status);
	HRESULT submitRequest(const unsigned long requestID);
	HRESULT getRequestType(const unsigned long requestID,unsigned long * const requestType);
	HRESULT getNextFinishedRequestID(unsigned long * const requestID);

	HRESULT writeData(const unsigned long requestID,const unsigned long dataLength,const void * const ptrData);
	HRESULT writeString(const unsigned long requestID,const char *const stringptr);
	HRESULT writeUINT8(const unsigned long requestID,const UINT8 data);
	HRESULT writeUINT16(const unsigned long requestID,const UINT16 data);
	HRESULT writeUINT32(const unsigned long requestID,const UINT32 data);
	HRESULT writeFLOAT32(const unsigned long requestID,const FLOAT32 data);
	HRESULT writeINT8(const unsigned long requestID,const INT8 data);
	HRESULT writeINT16(const unsigned long requestID,const INT16 data);
	HRESULT writeINT32(const unsigned long requestID,const INT32 data);

	HRESULT readData(const unsigned long requestID,const unsigned long dataLength, void * const ptrDestBuffer);
	HRESULT readString(const unsigned long requestID,const unsigned long stringBufferLength, char * const ptrBuffer);
	HRESULT readUINT8(const unsigned long requestID,UINT8 * const data);
	HRESULT readUINT16(const unsigned long requestID,UINT16 * const data);
	HRESULT readUINT32(const unsigned long requestID,UINT32 * const data);
	HRESULT readFLOAT32(const unsigned long requestID,FLOAT32 * const data);
	HRESULT readINT8(const unsigned long requestID,INT8 * const data);
	HRESULT readINT16(const unsigned long requestID,INT16 * const data);
	HRESULT readINT32(const unsigned long requestID,INT32 * const data);

	HRESULT startProcessing();
	HRESULT stopProcessing(const bool waitForRemainingItems);
	HRESULT isProcessing(bool *outState) const;
	
	HRESULT cancelRequest(const unsigned long requestID);
	HRESULT disposeRequest(const unsigned long requestID);

	HRESULT unsubmittedCount(unsigned long * output);
	HRESULT queueCount(unsigned long * output);
	HRESULT completedCount(unsigned long * output);
	
};