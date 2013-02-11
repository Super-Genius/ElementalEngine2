#include "stdafx.h"
#include "..\plugins2\include\languagelocalizer\LanguageLocalizerDefines.h"

CLanguageTest::CLanguageTest( CRichEditCtrl *dialogEditControl )
	: CTestBase( dialogEditControl )
{
	m_hszLanguageObjectType = _T("CLanguage");
	m_hszLanguageEntryType = _T("CLanguageEntry");
}

CLanguageTest::~CLanguageTest()
{}

void CLanguageTest::Initialize()
{
	CTestBase::Initialize();
	RegisterTestCase( _T( "Testing programmatic creation of language object..." ), (TESTCASE)&CLanguageTest::TestCreateLanguageObject );
	RegisterTestCase( _T( "Testing programmatic creation of language entry..." ), (TESTCASE)&CLanguageTest::TestCreateLanguageEntry );
	RegisterTestCase( _T( "Testing programmatic creation of language with entries..." ), (TESTCASE)&CLanguageTest::TestCreateFilledInLanguage );
	RegisterTestCase( _T( "Testing loading of language files..." ), (TESTCASE)&CLanguageTest::TestLoadingLanguageFiles );
	RegisterTestCase( _T( "Testing getting a simple entry..." ), (TESTCASE)&CLanguageTest::TestGettingSimpleEntry );
	RegisterTestCase( _T( "Testing getting a more complex entry 1..." ), (TESTCASE)&CLanguageTest::TestGettingComplexEntry1 );
	RegisterTestCase( _T( "Testing getting a more complex entry 2..." ), (TESTCASE)&CLanguageTest::TestGettingComplexEntry2 );
	RegisterTestCase( _T( "Testing getting a more complex entry without full archive..." ), (TESTCASE)&CLanguageTest::TestGettingComplexEntryWithHalfFilledArchive );
	RegisterTestCase( _T( "Testing getting a more complex entry without an archive..." ), (TESTCASE)&CLanguageTest::TestGettingComplexEntryWithNoArchive );
}

void CLanguageTest::TestCreateLanguageObject()
{
	DWORD retval;
	CHashString hszName(_T("LanguageTest_01"));
	retval = CreateObject(&hszName, NULL, &m_hszLanguageObjectType);
	TestAssert(_T("Create language object"), (retval == MSG_HANDLED));

	bool found = FindObject(&hszName);
	TestAssert(_T("Find language object"), (found == true));

	IArchive *ar = CreateMemoryArchive();
	ar->SetIsWriting(true);
	ar->SeekTo(0);
	SerializeLanguageObject(ar, _T("English"), 0);
	retval = SerializeObject(&hszName, ar, false);
	TestAssert(_T("Serialize language object from mem"), (retval == MSG_HANDLED));

	retval = DeleteObject(&hszName);
	TestAssert(_T("Delete test language"), (retval == MSG_HANDLED));

	found = FindObject(&hszName);
	TestAssert(_T("Object not found"), (found == false));
}

void CLanguageTest::TestCreateLanguageEntry()
{
	DWORD retval;
	CHashString hszName(_T("EntryTest_01"));
	retval = CreateObject(&hszName, NULL, &m_hszLanguageEntryType);
	TestAssert(_T("Create language entry"), (retval == MSG_HANDLED));

	bool found = FindObject(&hszName);
	TestAssert(_T("Find language entry"), (found == true));

	IArchive *ar = CreateMemoryArchive();
	ar->SetIsWriting(true);
	ar->SeekTo(0);
	PARAMVEC vec;
	AddParamToVector(vec, _T("TestName"), _T("string"));
	SerializeLanguageEntry(ar, _T("TestString"), _T("Hello %1%"), vec);
	retval = SerializeObject(&hszName, ar, false);
	TestAssert(_T("Serialize language object from mem"), (retval == MSG_HANDLED));

	retval = DeleteObject(&hszName);
	TestAssert(_T("Delete test language"), (retval == MSG_HANDLED));

	found = FindObject(&hszName);
	TestAssert(_T("Object not found"), (found == false));
}

void CLanguageTest::TestCreateFilledInLanguage()
{
	DWORD retval;
	CHashString hszLangName(_T("LanguageTest_02"));
	retval = CreateObject(&hszLangName, NULL, &m_hszLanguageObjectType);
	TestAssert(_T("Create language object"), (retval == MSG_HANDLED));

	bool found = FindObject(&hszLangName);
	TestAssert(_T("Find language object"), (found == true));

	IArchive *ar = CreateMemoryArchive();
	ar->SetIsWriting(true);
	ar->SeekTo(0);
	SerializeLanguageObject(ar, _T("English"), 2);
	PARAMVEC vec;
	vec.clear();
	SerializeLanguageEntry(ar, _T("TestKey1"), _T("Testomatic"), vec);
	AddParamToVector(vec, _T("TestName"), _T("string"));
	SerializeLanguageEntry(ar, _T("TestKey2"), _T("Hello %1%"), vec);
	retval = SerializeObject(&hszLangName, ar, false);
	TestAssert(_T("Serialize language object from mem (with entries)"), (retval == MSG_HANDLED));

	// internally created entries by the language
	// NOTE: names are generated as such
	CHashString hszInternalEntry1(_T("English_Entry_0"));
	CHashString hszInternalEntry2(_T("English_Entry_1"));

	found = FindObject(&hszInternalEntry1);
	TestAssert(_T("Find internally created entry 1"), (found == true));
	found = FindObject(&hszInternalEntry2);
	TestAssert(_T("Find internally created entry 2"), (found == true));

	retval = DeleteObject(&hszLangName);
	TestAssert(_T("Delete test language"), (retval == MSG_HANDLED));

	found = FindObject(&hszLangName);
	TestAssert(_T("Language not found"), (found == false));

	// try to find entries, they should be deleted too
	found = FindObject(&hszInternalEntry1);
	TestAssert(_T("Entry 1 not found"), (found == false));
	found = FindObject(&hszInternalEntry2);
	TestAssert(_T("Entry 2 not found"), (found == false));
}

void CLanguageTest::TestLoadingLanguageFiles()
{
	DWORD retval;
	retval = LoadFile(_T("TestLanguage1.lxl"));
	TestAssert(_T("Load file for language 1"), (retval == MSG_HANDLED));

	retval = LoadFile(_T("TestLanguage2.lxl"));
	TestAssert(_T("Load file for language 2"), (retval == MSG_HANDLED));
}

void CLanguageTest::TestGettingSimpleEntry()
{
	DWORD retval;
	retval = LoadFile(_T("TestLanguage1.lxl"));
	TestAssert(_T("Load file for language 1"), (retval == MSG_HANDLED));
	
	CHashString hszTestKey(_T("TestString1"));
	CHashString hszOutputText;
	GETLANGUAGETEXTPARAMS gltp;
	gltp.arParams = NULL;
	gltp.hszKeyString = &hszTestKey;
	gltp.hszTranslatedText = &hszOutputText;
	static DWORD msgGetLanguageText = CHashString(_T("GetLanguageText")).GetUniqueID();
	
	retval = m_ToolBox->SendMessage(msgGetLanguageText, sizeof(GETLANGUAGETEXTPARAMS), &gltp);
	TestAssert(_T("Message to get text entry"), (retval == MSG_HANDLED));

	StdString szOutput = hszOutputText.GetString();
	TestAssertEquals(_T("Returned translation"), _T("Hello, my name is flo"), szOutput);

	retval = LoadFile(_T("TestLanguage2.lxl"));
	TestAssert(_T("Load file for language 2"), (retval == MSG_HANDLED));

	retval = m_ToolBox->SendMessage(msgGetLanguageText, sizeof(GETLANGUAGETEXTPARAMS), &gltp);
	TestAssert(_T("Message to get text entry"), (retval == MSG_HANDLED));

	szOutput = hszOutputText.GetString();
	TestAssertEquals(_T("Returned translation"), _T("So let's pretend this is another language"), szOutput);
}

void CLanguageTest::TestGettingComplexEntry1()
{
	DWORD retval;
	retval = LoadFile(_T("TestLanguage1.lxl"));
	TestAssert(_T("Load file for language 1"), (retval == MSG_HANDLED));

	IArchive *ar = CreateMemoryArchive();
	ar->SetIsWriting(true);
	ar->SeekTo(0);
	ar->Write(_T("Nick")); // string value

	CHashString hszTestKey(_T("TestString2"));
	CHashString hszOutputText;
	GETLANGUAGETEXTPARAMS gltp;
	gltp.arParams = ar;
	gltp.hszKeyString = &hszTestKey;
	gltp.hszTranslatedText = &hszOutputText;
	static DWORD msgGetLanguageText = CHashString(_T("GetLanguageText")).GetUniqueID();
	
	retval = m_ToolBox->SendMessage(msgGetLanguageText, sizeof(GETLANGUAGETEXTPARAMS), &gltp);
	TestAssert(_T("Message to get text entry"), (retval == MSG_HANDLED));

	StdString szOutput = hszOutputText.GetString();
	TestAssertEquals(_T("Returned translation"), _T("Hello, Nick, how are you?"), szOutput);

	retval = LoadFile(_T("TestLanguage2.lxl"));
	TestAssert(_T("Load file for language 2"), (retval == MSG_HANDLED));

	retval = m_ToolBox->SendMessage(msgGetLanguageText, sizeof(GETLANGUAGETEXTPARAMS), &gltp);
	TestAssert(_T("Message to get text entry"), (retval == MSG_HANDLED));

	szOutput = hszOutputText.GetString();
	TestAssertEquals(_T("Returned translation"), _T("Again, Nick, let's say this is actually German"), szOutput);
}

void CLanguageTest::TestGettingComplexEntry2()
{
	DWORD retval;
	retval = LoadFile(_T("TestLanguage1.lxl"));
	TestAssert(_T("Load file for language 1"), (retval == MSG_HANDLED));

	IArchive *ar = CreateMemoryArchive();
	ar->SetIsWriting(true);
	ar->SeekTo(0);
	ar->Write(_T("Nick")); // string value
	ar->Write(5); // int value

	CHashString hszTestKey(_T("TestString3"));
	CHashString hszOutputText;
	GETLANGUAGETEXTPARAMS gltp;
	gltp.arParams = ar;
	gltp.hszKeyString = &hszTestKey;
	gltp.hszTranslatedText = &hszOutputText;
	static DWORD msgGetLanguageText = CHashString(_T("GetLanguageText")).GetUniqueID();
	
	retval = m_ToolBox->SendMessage(msgGetLanguageText, sizeof(GETLANGUAGETEXTPARAMS), &gltp);
	TestAssert(_T("Message to get text entry"), (retval == MSG_HANDLED));

	StdString szOutput = hszOutputText.GetString();
	TestAssertEquals(_T("Returned translation"), _T("Well, Nick, I have 5 apples."), szOutput);

	retval = LoadFile(_T("TestLanguage2.lxl"));
	TestAssert(_T("Load file for language 2"), (retval == MSG_HANDLED));

	retval = m_ToolBox->SendMessage(msgGetLanguageText, sizeof(GETLANGUAGETEXTPARAMS), &gltp);
	TestAssert(_T("Message to get text entry"), (retval == MSG_HANDLED));

	szOutput = hszOutputText.GetString();
	TestAssertEquals(_T("Returned translation"), _T("I have 5 apples, Mr. Nick. Here's a few % tests: %"), szOutput);
}

void CLanguageTest::TestGettingComplexEntryWithHalfFilledArchive()
{
	DWORD retval;
	retval = LoadFile(_T("TestLanguage1.lxl"));
	TestAssert(_T("Load file for language 1"), (retval == MSG_HANDLED));

	IArchive *ar = CreateMemoryArchive();
	ar->SetIsWriting(true);
	ar->SeekTo(0);
	ar->Write(_T("Nick")); // string value
	
	CHashString hszTestKey(_T("TestString3"));
	CHashString hszOutputText;
	GETLANGUAGETEXTPARAMS gltp;
	gltp.arParams = ar;
	gltp.hszKeyString = &hszTestKey;
	gltp.hszTranslatedText = &hszOutputText;
	static DWORD msgGetLanguageText = CHashString(_T("GetLanguageText")).GetUniqueID();

	// if archive is incomplete, will default integer to 0	
	retval = m_ToolBox->SendMessage(msgGetLanguageText, sizeof(GETLANGUAGETEXTPARAMS), &gltp);
	TestAssert(_T("Message to get text entry"), (retval == MSG_HANDLED));

	StdString szOutput = hszOutputText.GetString();
	TestAssertEquals(_T("Returned translation"), _T("Well, Nick, I have 0 apples."), szOutput);

	retval = LoadFile(_T("TestLanguage2.lxl"));
	TestAssert(_T("Load file for language 2"), (retval == MSG_HANDLED));

	retval = m_ToolBox->SendMessage(msgGetLanguageText, sizeof(GETLANGUAGETEXTPARAMS), &gltp);
	TestAssert(_T("Message to get text entry"), (retval == MSG_HANDLED));

	szOutput = hszOutputText.GetString();
	TestAssertEquals(_T("Returned translation"), _T("I have 0 apples, Mr. Nick. Here's a few % tests: %"), szOutput);
}

void CLanguageTest::TestGettingComplexEntryWithNoArchive()
{
	DWORD retval;
	retval = LoadFile(_T("TestLanguage1.lxl"));
	TestAssert(_T("Load file for language 1"), (retval == MSG_HANDLED));

	CHashString hszTestKey(_T("TestString3"));
	CHashString hszOutputText;
	GETLANGUAGETEXTPARAMS gltp;
	gltp.arParams = NULL;
	gltp.hszKeyString = &hszTestKey;
	gltp.hszTranslatedText = &hszOutputText;
	static DWORD msgGetLanguageText = CHashString(_T("GetLanguageText")).GetUniqueID();

	// if archive is incomplete, will default integer to 0
	// string defaults to ""
	retval = m_ToolBox->SendMessage(msgGetLanguageText, sizeof(GETLANGUAGETEXTPARAMS), &gltp);
	TestAssert(_T("Message to get text entry"), (retval == MSG_HANDLED));

	StdString szOutput = hszOutputText.GetString();
	TestAssertEquals(_T("Returned translation"), _T("Well, , I have 0 apples."), szOutput);

	retval = LoadFile(_T("TestLanguage2.lxl"));
	TestAssert(_T("Load file for language 2"), (retval == MSG_HANDLED));

	retval = m_ToolBox->SendMessage(msgGetLanguageText, sizeof(GETLANGUAGETEXTPARAMS), &gltp);
	TestAssert(_T("Message to get text entry"), (retval == MSG_HANDLED));

	szOutput = hszOutputText.GetString();
	TestAssertEquals(_T("Returned translation"), _T("I have 0 apples, Mr. . Here's a few % tests: %"), szOutput);
}

void CLanguageTest::SerializeLanguageObject(IArchive *ar, StdString language, int numEntries)
{
	// NOTE: coordinates with version 1.0 serialization
	ar->Write(1.0f);					// version
	ar->Write(language);				// language
	ar->Write(numEntries);				// number of entries
}

void CLanguageTest::SerializeLanguageEntry(IArchive *ar, StdString key, StdString text, PARAMVEC formatParams)
{
	// NOTE: coordinates with version 1.0 serialization
	ar->Write(1.0f);							// version
	ar->Write(key);								// key string
	ar->Write(text);							// text string
	ar->Write((UINT)formatParams.size());		// number of params
	for (UINT i=0; i<formatParams.size(); i++)
	{
		ar->Write(formatParams[i].szName);		// name of param
		ar->Write(formatParams[i].szType);		// type of param
	}
}

void CLanguageTest::AddParamToVector(PARAMVEC &vec, StdString name, StdString type)
{
	ENTRYPARAM entry;
	entry.szName = name;
	entry.szType = type;
	vec.push_back(entry);
}