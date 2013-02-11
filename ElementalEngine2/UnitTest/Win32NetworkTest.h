#ifndef _NETWORKTEST_H_
#define _NETWORKTEST_H_

class CWin32NetworkTest : public CTestBase
{
public:
	CWin32NetworkTest( CRichEditCtrl *dialogEditControl );
	~CWin32NetworkTest();

	virtual void Initialize();

	void SetTimeToCheck(float val);
	bool TickTime( void );
	
	// Test case wrapper fn's
	void TestNetworkPeerCreateHost();
	void TestNetworkPeerCreateClient();
	void TestNetworkPeerConnect();
	void TestNetworkPeerDisconect();
	void TestNetworkPeerCommunicate();
	
private:
	IToolBox * m_ToolBox;

	CHashString m_hszCWin32NetworkPeerObjectType;

	CHashString m_hszTestHostName;
	CHashString m_hszTestClientName;

	float m_fTimeCheck;
};

#endif //_NETWORKTEST_H_