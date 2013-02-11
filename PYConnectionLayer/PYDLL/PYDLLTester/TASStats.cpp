#include "stdafx.h"
#include "pydll.h"

#define TAS_MASTER_SERVER_ADDRESS "192.168.3.108"
//#define TAS_MASTER_SERVER_ADDRESS "www.datavertex.com"
#define TAS_MASTER_SERVER_PORT 4012

#define TAS_REQUESTTYPE_STATS 12

// The PYDLL C++ side can do encryption just fine
// the Java MCP is still missing that part.  Still good enough for demonstration purposes.
// so please leave this to FALSE for now.

#define TAS_USE_ENCRYPTION true

// This will be a PUBLIC KEY - the server keeps a matching private key.
#ifdef _XBOX
#define TAS_ENCRYPTIONKEY_FILENAME "game:\\Media\\encryptkey.key"
#else
#define TAS_ENCRYPTIONKEY_FILENAME "Media\\encryptkey.key"
#endif




// This only needs to be called once per game
bool TASSetupStatsConnection()
{
	if (PYDLL_initializePYConnection(TAS_MASTER_SERVER_ADDRESS,TAS_MASTER_SERVER_PORT)!=S_OK)
		return false; // unable to initialize

	PYDLL_setResponseTimeoutMS(1000UL * 15UL); // 15 second response timeout
	PYDLL_setRequestExpirationTimeoutMS(1000UL * 60UL); // 60 second garbage collection delay

	// heres an example of how to load encryption keys.  You can actually pull them from any source
	// as long as they are standard encoded RSA-1024 keys in binary form.
	// here we load them from file.
	if (TAS_USE_ENCRYPTION)
	{
#ifdef _XBOX
		printf("attempting to load: %s\r\n",TAS_ENCRYPTIONKEY_FILENAME);
#endif
		unsigned char encryptionKey[1024];
		FILE *ef = NULL;
		if (fopen_s(&ef,TAS_ENCRYPTIONKEY_FILENAME,"rb")!=0) return false;

#ifdef _XBOX
		printf("file sizing%s\r\n",TAS_ENCRYPTIONKEY_FILENAME);
#endif

		// how big is the file?
		fseek(ef,0,SEEK_END);
		unsigned long filesize_ef = ftell(ef);

		// check size
		if ((filesize_ef < 100) || (filesize_ef > 1024))
		{
			// the sizes are not the expected sizes.
			fclose(ef);
			return false;
		}
		// read the data in and send it to the DLL
		fseek(ef,0,SEEK_SET);
		fread(encryptionKey,filesize_ef,1,ef);
		fclose(ef);
		PYDLL_loadCryptoKeys(encryptionKey,filesize_ef);
	}

	if (PYDLL_startProcessing()!=S_OK)
		return false;

	return true;
	
}


// This function puts the given stats onto the queue for delivery and returns a UINT32 handle to the request
// This function does NOT block.  It returns a 0 if it failed to put the request on the queue.
// otherwise it returns a UINT32 that can be used to get the status of the sent stats
// Since stats are associated with a user, the username and password is included
// nonzero return value means the request was successfully placed on the queue
// it does NOT mean that it actually arrived.
unsigned long SendTASGameStats(const char * const phatyaffle_username, const char * const phatyaffle_password,
			  unsigned long score,float hit_miss_ratio)
{
	unsigned long requestID;

	if (PYDLL_createNewRequest(TAS_USE_ENCRYPTION,TAS_REQUESTTYPE_STATS,&requestID)!=S_OK)
		return 0UL; // unable to create a request - something is pretty wrong
	if (requestID==0) return 0UL; // 0 is an invalid request ID

	// If you wanted to associate this request with some user define data you could use 
	// PYDLL_setRequestUserData(...) to set it and PYDLL_getRequestUserData to retieve that value back at anytime.
	// that data could be anything you want.

	try
	{
		// write your data to it following the packet specification

		PYDLL_writeString(requestID,phatyaffle_username); 
		PYDLL_writeString(requestID,phatyaffle_password);
		PYDLL_writeUINT32(requestID,score);
		PYDLL_writeFLOAT32(requestID,hit_miss_ratio);
		
		// now put it on the queue.
		if (PYDLL_submitRequest(requestID)!=S_OK)  throw std::exception("failed to submit request");
	}
	catch (std::exception exc)
	{
		// here you could eg, write the message out to your debugger or whatever
		// or I guess throw it again
		::OutputDebugStringA("Failed to send TAS stats: ");
		::OutputDebugStringA(exc.what());
		::OutputDebugStringA("\n");

		PYDLL_cancelRequest(requestID);
		return 0UL;
	}

	
	
	return requestID;
}

// you can use this to check if the stat is done
bool TASIsStatSendComplete(const unsigned long requestID)
{
	eRequestStatus stat;
	if (PYDLL_getRequestStatus(requestID,&stat)!=S_OK)
		return true; // error'd stats are "complete"... since nothing more will occur.
	
	if (PYDLL_IsTerminalStatus(stat)) return true;

	return false;
}