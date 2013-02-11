// PYDLLTester.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

// here we're going to include the DLL directly
// but you can also do runtime binding using LoadLibrary
// the DLL is totally C-compatible.
#include "pydll.h"
#include "TASStats.h"

#define SUCCEED_OR_DIE(X) resultvalue = (X); if (resultvalue != S_OK) {die_due_to_hresult(false,resultvalue,#X,__FILE__,__LINE__);} else okay(false,#X)
#define FAIL_OR_DIE(X) resultvalue = (X); if (resultvalue == S_OK) {die_due_to_hresult(true,resultvalue,#X,__FILE__,__LINE__);} else okay(true,#X)
#define ASSERT_TEST(X) if ((X)==false) die_due_to_bool(#X,__FILE__,__LINE__); else okay(false,#X)

#define NUM_ELEMENTS_TO_TEST 150
// change this to encrypt everything
#define TEST_ENCRYPTION false

void Pause()
{
#ifndef _XBOX
	while (_kbhit()) _getch();
	while (!_kbhit()) {Sleep(1000);}
	_getch();
#endif
}

void okay(bool fail_expected,char *message)
{
	if (fail_expected)
		printf("[OK]   !%s\r\n",message);
	else
		printf("[OK]   %s\r\n",message);
}

void die_due_to_bool(char *message,char *file,int line)
{
	printf("[FAIL] %s(%i) ASSERTION FAILED\n  ---> (%s)\r\n",file,line,message);

	printf("Program execution complete.  Press any key.");
	Pause();
	throw std::exception("ASsertion failure.");
}

void die_due_to_hresult(bool invert,HRESULT hr,char *message,char *file,int line)
{
#ifndef _XBOX
	LPVOID lpMsgBuf = NULL;
	// ask windows what the HRESULT stands for.
	FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL,hr,MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
			(LPSTR)&lpMsgBuf,0,NULL);
	// Process any inserts in lpMsgBuf.
	// ...
	// Display the string.
	if (invert)
	{
		printf("[FAIL] : %s(%i) (Failure expected)\r\n",file,line);
	}
	else
	{
		printf("[FAIL] : %s(%i)\r\n",file,line);
	}
	printf("    STATEMENT: (%s)\r\n   HRESULT:0x%X - Meaning: %s",message,hr,(char *)lpMsgBuf);
	LocalFree( lpMsgBuf );
#else
	// xbox has no error lookup, sadly
	if (invert)
	{
		printf("[FAIL] : %s(%i) (Failure expected)\r\n",file,line);
	}
	else
	{
		printf("[FAIL] : %s(%i)\r\n",file,line);
	}
	printf("    STATEMENT: (%s)\r\n   HRESULT:0x%X ",message,hr);
#endif

	printf("Program execution complete.  Press any key.");
	Pause();
	throw std::exception("ASsertion failure.");
}

void DumpQueueStatus()
{
	unsigned long q,s,c;
	PYDLL_queueCount(&q);
	PYDLL_unsubmittedCount(&s);
	PYDLL_completedCount(&c);
	printf("Queued:  %3lu -- Unsubmitted: %3lu -- completed: %3lu\r\n",q,s,c);
}

unsigned long MakeStressTestPacket()
{
	unsigned long requestID;
	PYDLL_createNewRequest(TEST_ENCRYPTION,1,&requestID);
	PYDLL_writeData(requestID,(unsigned long)strlen("Hello"),"Hello");
	PYDLL_writeString(requestID,"How are you gentelmen?");
	PYDLL_writeUINT8(requestID,1);
	PYDLL_writeUINT16(requestID,65534);
	PYDLL_writeUINT32(requestID,100000);
	PYDLL_writeFLOAT32(requestID,0.25f);
	PYDLL_writeINT8(requestID,-32);
	PYDLL_writeINT16(requestID,-32000);
	PYDLL_writeINT32(requestID,-100000);
	return requestID;
}


bool CheckStressTestResponse(unsigned long requestID)
{
	eRequestStatus stat;
	if (PYDLL_getRequestStatus(requestID,&stat)!=S_OK) return false;
	if (stat!=ePY_Completed) return false;

	// read the data back from the server
	char hellostring[16];
	char gentelmenstring[32];
	UINT8 uint8data = 0;
	UINT16 uint16data = 0;
	UINT32 uint32data = 0;
	FLOAT32 float32data = 0.0f;
	INT8 int8data = 0;
	INT16 int16data = 0;
	INT32 int32data = 0;
	memset(hellostring,0,16);

	if (PYDLL_readData(requestID,(unsigned long)strlen("Hello"),hellostring)!=S_OK) return false;
	if (PYDLL_readString(requestID,32,gentelmenstring)!=S_OK) return false;
	if (PYDLL_readUINT8(requestID,&uint8data)!=S_OK) return false;
	if (PYDLL_readUINT16(requestID,&uint16data)!=S_OK) return false;
	if (PYDLL_readUINT32(requestID,&uint32data)!=S_OK) return false;
	if (PYDLL_readFLOAT32(requestID,&float32data)!=S_OK) return false;
	if (PYDLL_readINT8(requestID,&int8data)!=S_OK) return false;
	if (PYDLL_readINT16(requestID,&int16data)!=S_OK) return false;
	if (PYDLL_readINT32(requestID,&int32data)!=S_OK) return false;

	if ((strcmp(hellostring,"Hello")==0)==false) return false;
	if ((strcmp(gentelmenstring,"How are you gentelmen?")==0)==false) return false;
	if ((uint8data == 1)==false) return false;
	if ((uint16data == 65534)==false) return false;
	if ((uint32data == 100000)==false) return false;
	if ((fabsf(float32data - 0.25f)<0.001f)==false) return false;
	if ((int8data == -32)==false) return false;
	if ((int16data == -32000)==false) return false;
	if ((int32data == -100000)==false) return false;
	return true;
}

void TestUserData(unsigned long requestID)
{
	HRESULT resultvalue = S_OK;
	unsigned long example_user_id = 0xABCDEF;
	unsigned long user_id_length = 4UL;
	// Test user-data before sending.
	SUCCEED_OR_DIE(PYDLL_setRequestUserData(requestID,user_id_length,&example_user_id));
	example_user_id = 0x00;
	user_id_length = 0UL;
	SUCCEED_OR_DIE(PYDLL_getRequestUserData(requestID,&user_id_length,NULL));
	ASSERT_TEST(user_id_length==4UL);
	SUCCEED_OR_DIE(PYDLL_getRequestUserData(requestID,&user_id_length,&example_user_id));
	ASSERT_TEST(example_user_id == 0xABCDEF);
}


void TestPYDLL()
{
	HRESULT resultvalue = S_OK;
	unsigned long requestID = 101;
	unsigned long requestType = 1;
	unsigned long next_RequestID = 0;
	
	eRequestStatus stat = ePY_UnknownRequest;
	printf("***TESTING FOR CORRECTNESS...***\r\n");

	SUCCEED_OR_DIE(PYDLL_initializePYConnection("localhost",4012));
	// test multiple initialization failing:
	SUCCEED_OR_DIE(PYDLL_initializePYConnection("localhost",4012));

	if (TEST_ENCRYPTION)
	{
		unsigned char encryptionKey[1024];
		memset(encryptionKey,0,1024);
		// make sure it rejects garbage keys
		FAIL_OR_DIE(PYDLL_loadCryptoKeys(encryptionKey,512));

		FILE *ef = NULL;
		ASSERT_TEST(fopen_s(&ef,"encryptKey.blb","rb")==0);
		fseek(ef,0,SEEK_END);
		unsigned long filesize_ef = ftell(ef);
		ASSERT_TEST(filesize_ef<1024);
		fseek(ef,0,SEEK_SET);
		fread(encryptionKey,filesize_ef,1,ef);
		fclose(ef);

		FAIL_OR_DIE(PYDLL_loadCryptoKeys(NULL,0));
		FAIL_OR_DIE(PYDLL_loadCryptoKeys(encryptionKey,0));
		FAIL_OR_DIE(PYDLL_loadCryptoKeys(encryptionKey,2048));
		SUCCEED_OR_DIE(PYDLL_loadCryptoKeys(encryptionKey,filesize_ef));
		SUCCEED_OR_DIE(PYDLL_loadCryptoKeys(encryptionKey,filesize_ef)); // make sure you can change keys any time
	}

	// cannot set 0 timeouts
	FAIL_OR_DIE(PYDLL_setResponseTimeoutMS(0));
	FAIL_OR_DIE(PYDLL_setRequestExpirationTimeoutMS(0));
	
	SUCCEED_OR_DIE(PYDLL_setResponseTimeoutMS(1000 * 5)); // 5 second response timeout
	SUCCEED_OR_DIE(PYDLL_setRequestExpirationTimeoutMS(1000 * 60)); // 60 second expiration

	FAIL_OR_DIE(PYDLL_getRequestStatus(requestID,&stat));
	SUCCEED_OR_DIE(PYDLL_getNextFinishedRequestID(&next_RequestID));
	ASSERT_TEST(next_RequestID == 0UL);
	ASSERT_TEST(stat == ePY_UnknownRequest);

	// Create a new request, ensure it exists, then dispose of it and ensure its gone
	SUCCEED_OR_DIE(PYDLL_createNewRequest(false,requestType,&requestID));
	SUCCEED_OR_DIE(PYDLL_getRequestStatus(requestID,&stat));
	SUCCEED_OR_DIE(PYDLL_getRequestType(requestID,&requestType));
	SUCCEED_OR_DIE(PYDLL_disposeRequest(requestID));
	FAIL_OR_DIE(PYDLL_getRequestStatus(requestID,&stat));
	FAIL_OR_DIE(PYDLL_getRequestType(requestID,&requestType));
	ASSERT_TEST(stat == ePY_UnknownRequest);

	requestType = 1;

	// Create a new request, ensure it exists, then cancel it, and ensure its still there, but cancelled.
	SUCCEED_OR_DIE(PYDLL_createNewRequest(false,requestType,&requestID));
	SUCCEED_OR_DIE(PYDLL_getRequestStatus(requestID,&stat));
	SUCCEED_OR_DIE(PYDLL_getRequestType(requestID,&requestType));
	SUCCEED_OR_DIE(PYDLL_cancelRequest(requestID));
	SUCCEED_OR_DIE(PYDLL_getRequestStatus(requestID,&stat));
	SUCCEED_OR_DIE(PYDLL_getRequestType(requestID,&requestType));
	ASSERT_TEST(requestType == 1);
	ASSERT_TEST(stat == ePY_Cancelled);

	SUCCEED_OR_DIE(PYDLL_createNewRequest(TEST_ENCRYPTION,requestType,&requestID));
	SUCCEED_OR_DIE(PYDLL_getRequestStatus(requestID,&stat));
	SUCCEED_OR_DIE(PYDLL_getRequestType(requestID,&requestType));
	ASSERT_TEST(requestType == 1);
	
	TestUserData(requestID);
	// write the expected data to the request - request #1 is a stress test packet with the following structure.
	SUCCEED_OR_DIE(PYDLL_writeData(requestID,(unsigned long)strlen("Hello"),"Hello"));
	SUCCEED_OR_DIE(PYDLL_writeString(requestID,"How are you gentelmen?"));
	SUCCEED_OR_DIE(PYDLL_writeUINT8(requestID,1));
	SUCCEED_OR_DIE(PYDLL_writeUINT16(requestID,65534));
	SUCCEED_OR_DIE(PYDLL_writeUINT32(requestID,100000));
	SUCCEED_OR_DIE(PYDLL_writeFLOAT32(requestID,0.25f));
	SUCCEED_OR_DIE(PYDLL_writeINT8(requestID,-32));
	SUCCEED_OR_DIE(PYDLL_writeINT16(requestID,-32000));
	SUCCEED_OR_DIE(PYDLL_writeINT32(requestID,-100000));

	FAIL_OR_DIE(PYDLL_writeData(requestID,0,NULL));
	FAIL_OR_DIE(PYDLL_writeData(requestID,14UL,NULL));

	TestUserData(requestID);

	// submit nonexistent request
	FAIL_OR_DIE(PYDLL_submitRequest(12345UL));
	// Submit actual request
	SUCCEED_OR_DIE(PYDLL_submitRequest(requestID));

	// Resubmit existing request
	FAIL_OR_DIE(PYDLL_submitRequest(requestID));

	SUCCEED_OR_DIE(PYDLL_getRequestStatus(requestID,&stat));
	ASSERT_TEST(stat > ePY_NotQueuedYet);

	TestUserData(requestID);


	// assert not allowed to write to an a submitted handle
	FAIL_OR_DIE(PYDLL_writeData(requestID,(unsigned long)strlen("Hello"),"Hello"));
	FAIL_OR_DIE(PYDLL_writeString(requestID,"How are you gentelmen?"));
	FAIL_OR_DIE(PYDLL_writeUINT8(requestID,1));
	FAIL_OR_DIE(PYDLL_writeUINT16(requestID,65534));
	FAIL_OR_DIE(PYDLL_writeUINT32(requestID,100000));
	FAIL_OR_DIE(PYDLL_writeFLOAT32(requestID,0.25f));
	FAIL_OR_DIE(PYDLL_writeINT8(requestID,-32));
	FAIL_OR_DIE(PYDLL_writeINT16(requestID,-32000));
	FAIL_OR_DIE(PYDLL_writeINT32(requestID,-100000));

	TestUserData(requestID);

	SUCCEED_OR_DIE(PYDLL_startProcessing());

	do 
	{
		HRESULT hr = PYDLL_getRequestStatus(requestID,&stat);
		if (hr!=0) break;

		::Sleep(100);
	} 
	while(PYDLL_IsTerminalStatus(stat)==false);

	SUCCEED_OR_DIE(PYDLL_getRequestStatus(requestID,&stat));

	// make sure the request succeeded.
	ASSERT_TEST(PYDLL_IsErrorStatus(stat)==false);

	TestUserData(requestID);
	// read the data back from the server
	char hellostring[16];
	char gentelmenstring[32];
	UINT8 uint8data = 0;
	UINT16 uint16data = 0;
	UINT32 uint32data = 0;
	FLOAT32 float32data = 0.0f;
	INT8 int8data = 0;
	INT16 int16data = 0;
	INT32 int32data = 0;
	memset(hellostring,0,16);

	SUCCEED_OR_DIE(PYDLL_readData(requestID,(unsigned long)strlen("Hello"),hellostring));
	SUCCEED_OR_DIE(PYDLL_readString(requestID,32,gentelmenstring));
	SUCCEED_OR_DIE(PYDLL_readUINT8(requestID,&uint8data));
	SUCCEED_OR_DIE(PYDLL_readUINT16(requestID,&uint16data));
	SUCCEED_OR_DIE(PYDLL_readUINT32(requestID,&uint32data));
	SUCCEED_OR_DIE(PYDLL_readFLOAT32(requestID,&float32data));
	SUCCEED_OR_DIE(PYDLL_readINT8(requestID,&int8data));
	SUCCEED_OR_DIE(PYDLL_readINT16(requestID,&int16data));
	SUCCEED_OR_DIE(PYDLL_readINT32(requestID,&int32data));

	ASSERT_TEST(strcmp(hellostring,"Hello")==0);
	ASSERT_TEST(strcmp(gentelmenstring,"How are you gentelmen?")==0);
	ASSERT_TEST(uint8data == 1);
	ASSERT_TEST(uint16data == 65534);
	ASSERT_TEST(uint32data == 100000);
	ASSERT_TEST(fabsf(float32data - 0.25f)<0.001f);
	ASSERT_TEST(int8data == -32);
	ASSERT_TEST(int16data == -32000);
	ASSERT_TEST(int32data == -100000);

	// discard the request
	SUCCEED_OR_DIE(PYDLL_disposeRequest(requestID));

	// make sure its gone.
	FAIL_OR_DIE(PYDLL_getRequestStatus(requestID,&stat));

	// now lets do some stress testing

	SUCCEED_OR_DIE(PYDLL_stopProcessing(false));

	time_t last_time = time(NULL);

	// submit 1000 requests rapidly
	// do this while they are being processed in the background.
	unsigned long statuses[NUM_ELEMENTS_TO_TEST];
	unsigned long expected_cancelled = 0;
	unsigned long expected_unknown = 0;
	unsigned long expected_succeeded = 0;
	unsigned long expected_failed = 0;
	for (int a=0;a<NUM_ELEMENTS_TO_TEST;a++)
	{
		requestID = MakeStressTestPacket();
		PYDLL_submitRequest(requestID);
		statuses[a] = requestID;

		if (a % 7 == 0)
		{
			//randomly cancel every 7th
			PYDLL_cancelRequest(requestID);
			expected_cancelled++;
		}
		else if (a % 5 == 0) // if its not the 7th, randomly dispose every 5th
		{
			// randomly dispose
			PYDLL_disposeRequest(requestID);
			expected_unknown++;
		}
		else
		{
			expected_succeeded++;
		}


		if (last_time != time(NULL))
		{
			last_time = time(NULL);
			DumpQueueStatus();
		}
	}

	SUCCEED_OR_DIE(PYDLL_startProcessing());
	unsigned long q = 1;
	bool triggered_test = false;
	while (q>0)
	{
		PYDLL_queueCount(&q);
		::Sleep(1000);
		if (triggered_test==false)
		{
			if (q < (NUM_ELEMENTS_TO_TEST / 2))
			{
				triggered_test = true;
				// we trigger a reinit during process to make sure it properly waits and processes appropriately.
				SUCCEED_OR_DIE(PYDLL_initializePYConnection("localhost",4012));
			}
		}
		DumpQueueStatus();
	}
	// compute final stats
	int succeeded = 0;
	int failed = 0;
	int cancelled = 0;
	int unknown = 0;
	for (int a=0;a<NUM_ELEMENTS_TO_TEST;a++)
	{
		PYDLL_getRequestStatus(statuses[a],&stat);
		if (stat == ePY_Cancelled) cancelled ++;
		if (stat == ePY_Completed)
		{
			if (CheckStressTestResponse(statuses[a]))
			{
				succeeded ++;
			}
			else
			{
				failed++;
			}
		}
		if (stat == ePY_Failed) failed ++;
		if (stat == ePY_UnknownRequest) unknown ++;
		
	}
	printf("Stats:     %lu cancelled.... %lu succeeded... %lu failed.... %lu unknown (disposed)\r\n",cancelled,succeeded,failed,unknown);
	printf("Expected:  %lu cancelled.... %lu succeeded... %lu failed.... %lu unknown\r\n",expected_cancelled,expected_succeeded,expected_failed,expected_unknown);

	ASSERT_TEST(cancelled==expected_cancelled);
	ASSERT_TEST(succeeded==expected_succeeded);
	ASSERT_TEST(failed==expected_failed);
	ASSERT_TEST(unknown==expected_unknown);


	// now we're going to test expiration times
	SUCCEED_OR_DIE(PYDLL_setRequestExpirationTimeoutMS(1000 * 2)); // 2 second expiration
	SUCCEED_OR_DIE(PYDLL_createNewRequest(TEST_ENCRYPTION,requestType,&requestID));
	printf("Submitting requests and waiting for request to expire...\r\n");
	::Sleep(5000); // wait for 5 seconds
	FAIL_OR_DIE(PYDLL_getRequestStatus(requestID,&stat)); // this must fail
	ASSERT_TEST(stat == ePY_UnknownRequest);

	requestID = MakeStressTestPacket();
	PYDLL_submitRequest(requestID);
	printf("Submitting requests and waiting for request to expire...\r\n");
	::Sleep(5000); // wait for 5 seconds
	FAIL_OR_DIE(PYDLL_getRequestStatus(requestID,&stat));
	ASSERT_TEST(stat == ePY_UnknownRequest);
	

	printf("---------------------------------------------------------\r\n");
	printf("------------------- ALL TESTS SUCCEEDED -----------------\r\n");
	printf("---------------------------------------------------------\r\n");
}

void TestTAS()
{
	HRESULT resultvalue = S_OK;

	ASSERT_TEST(TASSetupStatsConnection());
	unsigned long requestID;
	// here you'd use actual username prompted from user somehow
	requestID = SendTASGameStats("iocaine","sigdev42",64,0.75f);

	ASSERT_TEST(requestID>0);

	// you wouldn't wait like this in a real game.
	// you'd probably just check it periodically as part of a gui update or something, periodically coming back
	// to see if its complete.
	while (!TASIsStatSendComplete(requestID))
	{
		::Sleep(1000);
	}
	// if you wanted to stress test you could call SendTASGameStats in a loop.
	
	// all done
	// Note that the TAS stats packets is fire and forget.  we didnt have to wait until the stat was complete
	// we could just fire it into the queue and forget about it.
	// This would be different if there were some sort of response to the stats packet
	// we'd have to wait until it became "complete" (status) and then read the data out of it
	// for things like high score queries, that's how it'd be done.

	eRequestStatus stat = ePY_Processing;
	UINT8 result = 0;
	SUCCEED_OR_DIE(PYDLL_getRequestStatus(requestID,&stat));
	SUCCEED_OR_DIE(PYDLL_readUINT8(requestID,&result));
	ASSERT_TEST(stat == ePY_Completed);
	ASSERT_TEST(result == 1);
	printf("----------------------- DONE -----------------------------\r\n");

}

int main(int argc, _TCHAR* argv[])
{
	try
	{
		TestTAS();
	}
	catch (std::exception ex)
	{
		printf("Exception caught: %s",ex.what());
	}
	

#ifndef _XBOX
	printf("Program execution complete.  Press any key.\r\n");

	while (_kbhit()) 
	{
		_getch();
	}
	while (!_kbhit()) { Sleep(100); }
	_getch();
	exit(0);
#endif
}
