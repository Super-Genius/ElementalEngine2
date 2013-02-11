#include "stdafx.h"
#include "..\plugins2\include\Win32Network\Win32NetworkDefines.h"
#include "NetworkMessages.h"

class CWin32NetworkTestObject : public IComponent
{
	SINGLETONCONSTRUCTOROVERRIDE(CWin32NetworkTestObject);

public:
	static IComponent *Create(int nArgs, va_list argptr);

	CWin32NetworkTestObject()
	{
		m_hszComponentType = _T("CWin32NetworkTestObject");
	}
	
	virtual void Serialize(IArchive &ar) {}
	virtual IHashString *GetComponentType() { return &m_hszComponentType; }
	virtual bool IsKindOf(IHashString *compType) { return m_hszComponentType.GetUniqueID() == compType->GetUniqueID(); }

	DWORD OnRecvNetworkPacket(DWORD size, void *param)
	{
		EngineGetToolBox()->Log(LOGINFORMATION, _T("OnRecvNetworkPacket\n"));
		return MSG_HANDLED_STOP;
	}

private:
	CHashString m_hszComponentType;
};
REGISTER_COMPONENT_SINGLETON(CWin32NetworkTestObject);
REGISTER_MESSAGE_HANDLER(RecvNetworkPacket, OnRecvNetworkPacket, CWin32NetworkTestObject);

IComponent *CWin32NetworkTestObject::Create(int nArgs, va_list argptr)
{
	return SINGLETONINSTANCE(CWin32NetworkTestObject);
}


CWin32NetworkTest::CWin32NetworkTest( CRichEditCtrl *dialogEditControl )
	: CTestBase( dialogEditControl )
{
	m_ToolBox = EngineGetToolBox();

	m_hszCWin32NetworkPeerObjectType = _T("CWin32NetworkPeer");
	m_hszTestHostName = _T("CWin32NetworkTest_TestHost");
	m_hszTestClientName = _T("CWin32NetworkTest_TestClient");
    m_fTimeCheck = 0.0f;
}

CWin32NetworkTest::~CWin32NetworkTest()
{

}

void CWin32NetworkTest::Initialize()
{
	CTestBase::Initialize();
	RegisterTestCase( _T( "Testing Creating a Peer Host..." ), (TESTCASE)&CWin32NetworkTest::TestNetworkPeerCreateHost );
	RegisterTestCase( _T( "Testing Creating a Peer Client..." ), (TESTCASE)&CWin32NetworkTest::TestNetworkPeerCreateClient );
	RegisterTestCase( _T( "Testing Connecting Peer Client to Host..." ), (TESTCASE)&CWin32NetworkTest::TestNetworkPeerConnect );
	RegisterTestCase( _T( "Testing Communicating Peer Client to Host..." ), (TESTCASE)&CWin32NetworkTest::TestNetworkPeerCommunicate );
	RegisterTestCase( _T( "Testing Disconnecting Peer Client from Host..." ), (TESTCASE)&CWin32NetworkTest::TestNetworkPeerDisconect );
}

void CWin32NetworkTest::SetTimeToCheck( float val )
{
	ITimer* timer = m_ToolBox->GetTimer();
	m_fTimeCheck = timer->GetRealTime() + val;
}

bool CWin32NetworkTest::TickTime()
{
	ITimer* timer = m_ToolBox->GetTimer();
	return (m_fTimeCheck < timer->GetRealTime());
}

void CWin32NetworkTest::TestNetworkPeerCreateHost()
{
	DWORD retval; 

	retval = CreateObject( &m_hszTestHostName , NULL, &m_hszCWin32NetworkPeerObjectType );
	TestAssert(_T("Create Peer Host"), (retval == MSG_HANDLED));

	retval = InitializeObject( &m_hszTestHostName );
	TestAssert(_T("Init Peer Host"), (retval == MSG_HANDLED));

	CREATENETWORKPEER net;
	net.iMaxClients = 5;
	net.iServerPort = 10000;
	net.iThreadSleepTimer = 30;

	static DWORD msgHash_StartHost = CHashString(_T("StartHost")).GetUniqueID();
	retval = m_ToolBox->SendMessage(msgHash_StartHost, sizeof(CREATENETWORKPEER), &net, &m_hszTestHostName, &m_hszCWin32NetworkPeerObjectType);
	TestAssert(_T("Start Peer Host"), (retval == MSG_HANDLED));
}

void CWin32NetworkTest::TestNetworkPeerCreateClient()
{
	DWORD retval; 

	retval = CreateObject( &m_hszTestClientName , NULL, &m_hszCWin32NetworkPeerObjectType );
	TestAssert(_T("Create Peer Client"), (retval == MSG_HANDLED));

	InitializeObject( &m_hszTestClientName );
	TestAssert(_T("Init Peer Client"), (retval == MSG_HANDLED));

	CREATENETWORKPEER net;
	net.iThreadSleepTimer = 30;
	
	static DWORD msgHash_StartClient = CHashString(_T("StartClient")).GetUniqueID();
	retval = m_ToolBox->SendMessage(msgHash_StartClient, sizeof(CREATENETWORKPEER), &net, &m_hszTestClientName, &m_hszCWin32NetworkPeerObjectType);
	TestAssert(_T("Start Peer Client"), (retval == MSG_HANDLED));
}

void CWin32NetworkTest::TestNetworkPeerConnect()
{
	DWORD retval;
	CONNECTPEER net;
	net.szAddress = _T("127.0.0.1");
	net.iServerPort = 10000;
	net.iPasswordSize = 0;
		
	static DWORD msgHash_Connect = CHashString(_T("Connect")).GetUniqueID();
	retval = m_ToolBox->SendMessage(msgHash_Connect, sizeof(CONNECTPEER), &net, &m_hszTestClientName, &m_hszCWin32NetworkPeerObjectType);
	TestAssert(_T("Connect Client To Host"), (retval == MSG_HANDLED) && (net.bResult == true) );
}

void CWin32NetworkTest::TestNetworkPeerDisconect()
{
	DWORD retval;
	DISCONNECTPEER net;

	static DWORD msgHash_Disconnect = CHashString(_T("Disconnect")).GetUniqueID();
	retval = m_ToolBox->SendMessage(msgHash_Disconnect, sizeof(DISCONNECTPEER), &net, &m_hszTestClientName, &m_hszCWin32NetworkPeerObjectType);
	TestAssert(_T("Disconnec From Host"), (retval == MSG_HANDLED) && (net.bResult == true) );
}

void CWin32NetworkTest::TestNetworkPeerCommunicate()
{
	DWORD retval;

	const TCHAR *pTestMsg = _T("Hello");

	SENDNETWORKPACKETPARAMS snpp;
	snpp.pData = (void*)pTestMsg;
	snpp.iDataSize = (DWORD)_tcslen( pTestMsg ) + 1; // include null terminator
	static DWORD msgHash_SendNetworkPacket = CHashString(_T("SendNetworkPacket")).GetUniqueID();
	retval = m_ToolBox->SendMessage(msgHash_SendNetworkPacket, sizeof(snpp), &snpp, &m_hszTestClientName, &m_hszCWin32NetworkPeerObjectType);

	TestAssert(_T("Send Network Packet Client->Host"), (retval == MSG_HANDLED) );
}
