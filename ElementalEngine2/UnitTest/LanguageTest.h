#ifndef _LANGUAGETEST_H_
#define _LANGUAGETEST_H_

struct ENTRYPARAM
{
	StdString szName;
	StdString szType;
};

typedef vector<ENTRYPARAM> PARAMVEC;

class CLanguageTest : public CTestBase
{
public:
	CLanguageTest( CRichEditCtrl *dialogEditControl );
	~CLanguageTest();

	virtual void Initialize();
	
	// Test case wrapper fn's
	void TestCreateLanguageObject();
	void TestCreateLanguageEntry();
	void TestCreateFilledInLanguage();
	void TestLoadingLanguageFiles();
	void TestGettingSimpleEntry();
	void TestGettingComplexEntry1();
	void TestGettingComplexEntry2();
	void TestGettingComplexEntryWithHalfFilledArchive();
	void TestGettingComplexEntryWithNoArchive();

private:
	void SerializeLanguageObject(IArchive *ar, StdString language, int numEntries);
	void SerializeLanguageEntry(IArchive *ar, StdString key, StdString text, PARAMVEC formatParams);
	void AddParamToVector(PARAMVEC &vec, StdString name, StdString type);

	IArchive* CreateMemArchive();

	CHashString m_hszLanguageObjectType;
	CHashString m_hszLanguageEntryType;
};

#endif //_LANGUAGETEST_H_