#ifndef ENTITYQUERY_H
#define ENTITYQUERY_H

class CEntityQueryTest : public CTestBase
{
public:
	CEntityQueryTest();
	~CEntityQueryTest();
	void Initialize();
	void DeInitialize();

	bool TestEmptyQuery();
	bool TestQuery1();
	bool TestQuery2();

private:
	void SetupWorld();
};

#endif // #ifndef ENTITYQUERY_H