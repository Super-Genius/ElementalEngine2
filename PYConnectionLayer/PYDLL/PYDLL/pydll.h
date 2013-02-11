#pragma once

// ---------------------- THIS IS THE ONLY HEADER FILE THAT END-USERS OF PYDLL SHOULD NEED TO RECEIEVE --------------------
// ---------------------- They should be able to use this header along with perhaps the lib file for static linking -------
// ---------------------- They should be able to use this header WITHOUT the lib for runtime DLL loading ------------------

// Nick L (Iocaine) Standard wizard-generated DLL Support
#ifdef _USRDLL
	#ifdef PHATYAFFLECONNECTION_DLL_EXPORTS
		#define PYDLL_API __declspec(dllexport)
	#else
		#define PYDLL_API __declspec(dllimport)
	#endif
#else
// if its not a dll then its likely to just be a LIB file - no need to do a dllexport or something.
	#define PYDLL_API extern
#endif

#ifndef _HRESULT_DEFINED
#define _HRESULT_DEFINED
typedef long HRESULT;
#endif // !_HRESULT_DEFINED

// Note: Nothing in this include file should require any of the following
// the STL, windows headers
// we want this loadable in C as well as C++ so unfortunately, no fancy STL!
// and no C++-only features such as pass by &reference or templates.

// eRequestStatus - the various statuses that the requests are in
typedef enum 
{
	ePY_UnknownRequest, // The requestID was not found
	ePY_NotQueuedYet,   // The requestID was found but is not yet submitted (data can be added)
	ePY_Queued,         // The requestID has been placed on the queue but not yet processed
	ePY_Processing,     // The requestID is currently processing (connected to server)
	ePY_Cancelled,		// The requestID was cancelled due to direct cancellation or shutdown
	ePY_Failed,			// The requestID was failed due to connection or protocol errors (not due to app specific fail)
	ePY_Completed,		// The requestID completed without protocol or connection errors (still might be invalid)
} eRequestStatus;

// Whenever you use the SMART request system
// you need to pack one of these into a uint8
// followed by the name of the field like "score" as a string
// before the data.  This allows an extra layer of checking
// If you're not using SMART requests, dont bother.
typedef enum  
{
	eSR_UNKNOWN,
	eSR_INT8,
	eSR_INT16, 
	eSR_INT32,
	eSR_UINT8,
	eSR_UINT16, 
	eSR_UINT32,
	eSR_FLOAT,
	eSR_STRING,
	eSR_DATA
} eSmartRequestTypes;

// These typedefs are echoed in windows.h etc but we repeat them here
// so that it is not windows reliant
typedef signed char         INT8, *PINT8;
typedef signed short        INT16, *PINT16;
typedef signed int          INT32, *PINT32;
typedef signed __int64      INT64, *PINT64;
typedef unsigned char       UINT8, *PUINT8;
typedef unsigned short      UINT16, *PUINT16;
typedef unsigned int        UINT32, *PUINT32;
typedef unsigned __int64    UINT64, *PUINT64;
typedef float FLOAT32;

// ----------------------- EXPORTED FUNCTIONS --------------------------------
// All exports are extern c for easier use
// ALL functions return standard HRESULTS
// where 0 means success and other values indicate various reasons for failure
// Note that a function can still succeed without HRESULT failure in many cases
// HRESULT failures are generally translated to mean exceptional circumstances
// ---------------------------------------------------------------------------

// Conventions:
// All pointers passed in must be initialised by the caller, point to valid memory
// this library will NOT allocate buffer space for you.

// Here are some possible HRESULT error codes:
// * S_OK - success
// * ERROR_NOT_READY - trying to call functions when you havent initialized the library properly
// * ERROR_ALREADY_INITIALIZED - trying to call init multiple times
// * E_NOTIMPL - function not implmented yet
// * E_INVALIDARG - you have sent an argument that can't be right, like a 0 for timeout or size of data or something
// * ERROR_INCORRECT_ADDRESS - could not resolve the host address specified
// * ERROR_NOT_FOUND - Could not find the requestID you asked for.
// * ERROR_ACCESS_DENIED - Trying to read from a request that isn't complete yet.
// * E_POINTER - pointer to buffer is NULL or something
// * All of the WSA*** type winsock errors which could occur during trying to init winsock.
// You can use FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,...)
// in windows to automatically convert any HRESULT into localized readable error test.

// PYDLL_initializePYConnection
// Call this to change the connection parameters
// You can call this any time, but it will stall the queue processing for a bit
// Ideally you'd call this just once, but you can always have it init a couple times at different
// host addresses until you find one that works.  It just does a DNS lookup for the server address in question
// (unless you supply an IP)
// An example of why you might want to use this multiple times is if you have multiple PY servers
// that you'd like to switch between if one doesn't work.  However, that's supposed to be handled
// by a load balancer anyway... or if DNS fails and you'd like to try a couple times (more likely)
extern "C" PYDLL_API HRESULT PYDLL_initializePYConnection(const char * const serverAddress,const unsigned short serverPort);

// PYDLL_loadCryptoKeys
// Call this to initialize the crypto subsystem with the supplied keys
// if you don't call this, all packets with encryption set to true will fail.
// The keys and messageid's assigned to your game app should have been given along with this DLL API
extern "C" PYDLL_API HRESULT PYDLL_loadCryptoKeys(const unsigned char *const encryptionKey,const unsigned long encryptionKeyLength);

// PYDLL_setResponseTimeoutMS
// - sets the response timeout (connection level).  Any queued or processing transaction that takes longer than this
//   with no data being sent or received will automatically be set to failed
//   recommended: 10000 in non-debug, 60000 in debug (due to breakpoints taking extra time)
extern "C" PYDLL_API HRESULT PYDLL_setResponseTimeoutMS(const unsigned long milliseconds);

// PYDLL_setRequestExpirationTimeoutMS
// - sets the request expiration timeout (global level).  Any request that exists for longer than this
//   in ANY state will be deleted entirely.  This should not happen (you should call disposeRequest
//   but this is a safety valve for those that leak.  Use 60 seconds or more.
extern "C" PYDLL_API HRESULT PYDLL_setRequestExpirationTimeoutMS(const unsigned long milliseconds);

// PYDLL_createNewRequest
// - Choose whether you want encryption or not, and the type of request (user defined)
// - outputs the new requestID into newRequestID
// - example:  unsigned long newreq;  PYDLL_createNewRequest(false,RTYPE_LOGIN,&newreq);
extern "C" PYDLL_API HRESULT PYDLL_createNewRequest(const bool enableEncryption, const unsigned long requestType,unsigned long * const newRequestID);

// PYDLL_setRequestUserData
// - This associates a request with a buffer of any data of your choice
// - It copies the data, so you can discard the buffer
// - You can ask it for the length of the data back at any time
// - You can ask it for the data back at any time no matter what status its in
//   Except for unknown request, of course.
extern "C" PYDLL_API HRESULT PYDLL_setRequestUserData(const unsigned long requestID,const unsigned long datalength,const void * const dataBuffer);

// PYDLL_getRequestUserData
// - datalength must point towards an unsigned long containing the size if your buffer.
// - if datalength is zero, it will be overwritten with the current size of the user data buffer, so you know how much
//   space to allocate.  In this case, dataBuffer can be NULL (it wont actually do any copy)
// - otherwise, it will copy up to datalength bytes into the databuffer, or up to the amount of data in the user
//   buffer if the user buffer is smaller, and put the number of bytes actually copied, into datalength.
extern "C" PYDLL_API HRESULT PYDLL_getRequestUserData(const unsigned long requestID,unsigned long *const datalength,void * const dataBuffer);

// PYDLL_getRequestStatus
// - Puts the status of the request with the given requestID into status
// - example:  eRequestStatus stat;  PYDLL_getRequestStatus(newreq,&stat);
extern "C" PYDLL_API HRESULT PYDLL_getRequestStatus(const unsigned long requestID,eRequestStatus * const status);

// PYDLL_submitRequest
// - Closes the request's writability, and places it on queue for delivery ASAP
// - After this, the status of the request will at least be ePY_Queued but
//   do not use that as an assertion - the threading is FAST and by the time this function
//   is back it might already be processing or done
extern "C" PYDLL_API HRESULT PYDLL_submitRequest(const unsigned long requestID);

// PYDLL_getRequestType
// - Returns the type of the given request.  The type is whatever value you passed in when you created it.
// - Remember that the requestType parameter must point to an actual valid unsigned long to write into.
extern "C" PYDLL_API HRESULT PYDLL_getRequestType(const unsigned long requestID,unsigned long * const requestType);

// PYDLL_getNextFinishedRequestID
// - Returns the next request that is completed, failed, or cancelled.
//   You can use this in a loop to consume all requests that have completed.
// - It will return the same request over and over until you call PYDLL_disposeRequest on it
//   so make sure that you do that before calling it again else you're headed for infinite loop
// - will not return items that have not yet been submitted.
extern "C" PYDLL_API HRESULT PYDLL_getNextFinishedRequestID(unsigned long * const requestID);

// PYDLL_writeData - writes the raw data from a given buffer into the requestID in question
extern "C" PYDLL_API HRESULT PYDLL_writeData(const unsigned long requestID,const unsigned long dataLength,const void * const ptrData);

// PYDLL_writeString
// - writes the given string into the requestID.  
// - Strings are always char arrays, not wchar - so if you want to send unicode, remember to do a UTF-8
//   transformation on the character array beforehand.  The server on the other end will assume all
//   incoming strings are unicode encoded as UTF-8.
// - Expects a null terminated string.
// - This is the ONLY reliable way to read a text string, because of UTF-8 decoding and possible compression
// - A string write is actually just a writeUINT16 (length) followed by the data in the underlying protocol
extern "C" PYDLL_API HRESULT PYDLL_writeString(const unsigned long requestID,const char *const stringptr);

// ------------- PYDLL_writexxx functions -----------------------
// - Use these to write data to the request before its queued
// - If your data is one of these types please use it.  Yes, you can also use
//   writeData to write the raw data, but these functions take endian-ness into account.
// - The server is expecting little-endian (intel) data, so if you want to write stuff yourself
//   make sure you do it that way, or just use one of these functions.
extern "C" PYDLL_API HRESULT PYDLL_writeUINT8(const unsigned long requestID,const UINT8 data);
extern "C" PYDLL_API HRESULT PYDLL_writeUINT16(const unsigned long requestID,const UINT16 data);
extern "C" PYDLL_API HRESULT PYDLL_writeUINT32(const unsigned long requestID,const UINT32 data);
extern "C" PYDLL_API HRESULT PYDLL_writeFLOAT32(const unsigned long requestID,const FLOAT32 data);
extern "C" PYDLL_API HRESULT PYDLL_writeINT8(const unsigned long requestID,const INT8 data);
extern "C" PYDLL_API HRESULT PYDLL_writeINT16(const unsigned long requestID,const INT16 data);
extern "C" PYDLL_API HRESULT PYDLL_writeINT32(const unsigned long requestID,const INT32 data);

// NOTE: Read functions do not read back the data that you wrote.  That is one-way data.
//       They read back the data that the SERVER sends in response to your data.
//       If you want to associate data with your requestID use the PYDLL_setUserData() function

// PYDLL_readData
// - Reads raw data.  Datalength MUST be the number of bytes you want to read
// - ptrDestBuffer MUST already be preallocated to be at least datalength bytes.
// - all reading functions will cause a HRESULT error if you ask for more data than exists.
//   In other words, don't try to find out how much data is there by reading 9999999UL from this
// - Know in advance how long you expect your data to be.
extern "C" PYDLL_API HRESULT PYDLL_readData(const unsigned long requestID,const unsigned long dataLength, void * const ptrDestBuffer);

// PYDLL_readString
// - reads UP TO stringBufferLength characters from the server into ptrBuffer
// - if the string is longer than the buffer you give, an HRESULT error will occur
extern "C" PYDLL_API HRESULT PYDLL_readString(const unsigned long requestID,const unsigned long stringBufferLength, char * const ptrBuffer);

// PYDLL_readXXXX - read functions are all the same
// - These functions read the appropriate amount of data from the server buffer
//   just like the write functions, these guarantee data-endianness is correct
// - By the time the data comes out of these functions you should be able to use it
//   as per normal - ie, it will have already been endian-swapped and decompressed or whatever else
extern "C" PYDLL_API HRESULT PYDLL_readUINT8(const unsigned long requestID,UINT8 * const data);
extern "C" PYDLL_API HRESULT PYDLL_readUINT16(const unsigned long requestID,UINT16 * const data);
extern "C" PYDLL_API HRESULT PYDLL_readUINT32(const unsigned long requestID,UINT32 * const data);
extern "C" PYDLL_API HRESULT PYDLL_readFLOAT32(const unsigned long requestID,FLOAT32 * const data);
extern "C" PYDLL_API HRESULT PYDLL_readINT8(const unsigned long requestID,INT8 * const data);
extern "C" PYDLL_API HRESULT PYDLL_readINT16(const unsigned long requestID,INT16 * const data);
extern "C" PYDLL_API HRESULT PYDLL_readINT32(const unsigned long requestID,INT32 * const data);

// PYDLL_startProcessing
// - Runs the actual processing engine.  You can call this as many times as you want
//   It will do nothing if its already processing.  Otherwise it will start the processing
//   thread(s) and begin making requests.
// - Its probably best practice to start this right off the bat and let it pull things
//   from the queue as you submit them, rather than fill the queue up and then start it.
extern "C" PYDLL_API HRESULT PYDLL_startProcessing();

// PYDLL_stopProcessing
// - Starts powering down the machinery.  Note that if you set waitForRemainingitems
//   to false, it will move all currently queued or processing items to CANCELLED
//   or FAILED if they are busy working, and immediately cut of all sockets and pipes.
// - Otherwise, the threads will keep going until the last queued item is done, and then
//   terminate.  This function will NOT block.
extern "C" PYDLL_API HRESULT PYDLL_stopProcessing(const bool waitForRemainingItems);

// PYDLL_isProcessing
// - Returns true if the processor is still doing work.
// - Note that it will return true even after stopProcessing is called, until the
//   engine actually shuts down.  So if you want to do a non-blocking graceful shutdown
// simply call stopProcessing(true) and then poll this until it returns false.
extern "C" PYDLL_API HRESULT PYDLL_isProcessing(bool *outState);

// PYDLL_cancelRequest
// - If a request is queued, NotQueuedYet, or Processing, this causes it to become
//   Cancelled.  Otherwise it has no effect. 
// - If its processing it will immediately cut the connection
//   which can sometimes cause it to become "failed"
extern "C" PYDLL_API HRESULT PYDLL_cancelRequest(const unsigned long requestID);

// PYDLL_disposeRequest
// - Once you are done with a request you should definitely call disposeRequest on it
// - Disposing a request also cancels it if its not already in a terminal state
// - After calling this, the requestID is totaly freed up and will respond with
//   ePY_UnknownRequest.
// - If you fail to dispose your requests, you could suffer performance loss over
//   time, but with an upper bound depending on PYDLL_setRequestExpirationTimeoutMS
// - If you set PYDLL_setRequestExpirationTimeoutMS sufficiently low (like 30 sec)
//   it might be unneccesary to bother disposing requests as they will naturally
//   expire and self-dispose after that time is reached.
extern "C" PYDLL_API HRESULT PYDLL_disposeRequest(const unsigned long requestID);

// PYDLL_unsubmittedCount
// returns how many requests are waiting for submission and will not sent until submitted
extern "C" PYDLL_API HRESULT PYDLL_unsubmittedCount(unsigned long *resultout);

// PYDLL_queueCount
// returns how many requests are submitted and are waiting to be sent
extern "C" PYDLL_API HRESULT PYDLL_queueCount(unsigned long *resultout);

// PYDLL_completedCount
// returns how many requests are completed and have not yet been disposed
extern "C" PYDLL_API HRESULT PYDLL_completedCount(unsigned long *resultout);

// utility functions.
// PYDLL_IsTerminalStatus returns true if the status given 
// will not change anymore unless you take action.
// for example, ePY_Queued is NOT a terminal status, because it will change to ePY_Processing
// but ePY_UnknownRequest, ePY_NotQueuedYet, ePY_Failed, and ePY_Completed are terminal statuses
// because they will not change into something else (and arent on the queue);
// you can use this function to wait for a queued item to finish.
extern "C" PYDLL_API bool PYDLL_IsTerminalStatus(const eRequestStatus stat);

// PYDLL_IsErrorStatus returns true if the given status is an error.
extern "C" PYDLL_API bool PYDLL_IsErrorStatus(const eRequestStatus stat);

