///============================================================================
/// \file		Win32NetworkPeer.cpp
/// \brief		Implementation of Win32 Network Peer Server or Client(using Raknet)
/// \date		06-27-2008
/// \author		Patrick Ghiocel
/// \note   Elemental Engine
///         Copyright (c)  2005-2008 Signature Devices, Inc.
///
///         This code is redistributable under the terms of the EE License.
///
///         This code is distributed without warranty or implied warranty of
///			merchantability or fitness for a particular purpose. See the 
///			EE License for more details.
///
///         You should have received a copy of the EE License along with this
///			code; If not, write to Signature Devices, Inc.,
///			3200 Bridge Parkway Suite 102, Redwood City, CA 94086 USA.
///============================================================================



#include "StdAfx.h"

REGISTER_COMPONENT(CWin32NetworkPeer);
REGISTER_MESSAGE_HANDLER( StartHost, OnStartHost, CWin32NetworkPeer );
REGISTER_MESSAGE_HANDLER( StartClient, OnStartClient, CWin32NetworkPeer );
REGISTER_MESSAGE_HANDLER( Connect, OnConnect, CWin32NetworkPeer );
REGISTER_MESSAGE_HANDLER( Disconnect, OnDisconnect, CWin32NetworkPeer );
REGISTER_MESSAGE_HANDLER( SendNetworkPacket, OnSendNetworkPacket, CWin32NetworkPeer );

CWin32NetworkPeer::CWin32NetworkPeer(IHashString *parentName, IHashString *name, bool bAddToHier) : 
	OBJECTTEMPLATE( CWin32NetworkManager, CWin32NetworkPeer, IObject, parentName, name)
{
	m_ToolBox = EngineGetToolBox();
	m_pPeer = NULL;
	m_bIsServer = false;
	m_pSendPacketBuffer = new BYTE[SENDPACKETBUFFERSIZE];

	m_pReceivedPacketArchive = NULL;
}

CWin32NetworkPeer::~CWin32NetworkPeer()
{
	DeInit();
	SAFE_DELETE_ARRAY( m_pSendPacketBuffer );
}

IComponent *CWin32NetworkPeer::Create(int nArgs, va_list argptr)
{
	IHashString *name;
	IHashString *parentName;
	IObject *self;
	bool bAddToHier;

	name = va_arg(argptr, IHashString *);
	parentName = va_arg(argptr, IHashString *);
	// create the object however you must create it
	try
	{
		// try and create an instance
		if (nArgs == 2)
		{
			// default to false
			self = new CWin32NetworkPeer(parentName, name, false);
		}
		else if (nArgs == 3)
		{
			bAddToHier = va_arg(argptr, bool);
			self = new CWin32NetworkPeer(parentName, name, bAddToHier);
		}
	}
	catch(...)
	{
		return NULL;
	}

	return self;
}

void CWin32NetworkPeer::Init()
{
	m_pPeer = RakNetworkFactory::GetRakPeerInterface();
//	REGISTER_CLASS_MEMBER_RPC( m_pPeer, CWin32NetworkPeer, ForwardRPCMessage );

	CREATEARCHIVE ca;
	ca.mode = STREAM_MODE_WRITE | STREAM_MODE_READ;
	ca.streamData = NULL; // use internal buffer
	ca.streamSize = 0;
	CHashString memType(_T("Memory"));
	ca.streamType = &memType;
	static DWORD msgHash_CreateArchive = CHashString(_T("CreateArchive")).GetUniqueID();
	if (m_ToolBox->SendMessage(msgHash_CreateArchive, sizeof(CREATEARCHIVE), &ca) == MSG_HANDLED)
	{
		m_pReceivedPacketArchive = ca.archive;
	}
}
	
void CWin32NetworkPeer::DeInit()
{
	if( m_pPeer != NULL )
	{
		RakNetworkFactory::DestroyRakPeerInterface(m_pPeer);
		m_pPeer = NULL;
	}
	if (m_pReceivedPacketArchive)
	{
		m_pReceivedPacketArchive->Close();
		m_pReceivedPacketArchive = NULL;
	}
}

void CWin32NetworkPeer::Serialize(IArchive &ar)
{
	if (ar.IsReading())
	{
	
	}
	else
	{
	
	}
}

bool CWin32NetworkPeer::Update()
{
	Packet* pPacket = m_pPeer->Receive();
	if (pPacket)
	{
		switch (pPacket->data[0])
		{
		case ID_REMOTE_DISCONNECTION_NOTIFICATION:
			m_ToolBox->Log(LOGINFORMATION, _T("Win32 Network Peer %s: A client has disconnected.\n"), GetName()->GetString());
			break;
		case ID_REMOTE_CONNECTION_LOST:
			m_ToolBox->Log(LOGINFORMATION, _T("Win32 Network Peer %s: A client has lost the connection.\n"), GetName()->GetString());
			break;
		case ID_REMOTE_NEW_INCOMING_CONNECTION:
			m_ToolBox->Log(LOGINFORMATION, _T("Win32 Network Peer %s: A client has connected.\n"), GetName()->GetString());
			break;
		case ID_CONNECTION_REQUEST_ACCEPTED:
			m_ToolBox->Log(LOGINFORMATION, _T("Win32 Network Peer %s: Connection request accepted.\n"), GetName()->GetString());
			break;
		case ID_NEW_INCOMING_CONNECTION:
			m_ToolBox->Log(LOGINFORMATION, _T("Win32 Network Peer %s: A connection is incoming.\n"), GetName()->GetString());
			break;
		case ID_NO_FREE_INCOMING_CONNECTIONS:
			m_ToolBox->Log(LOGINFORMATION, _T("Win32 Network Peer %s: The server is full.\n"), GetName()->GetString());
			break;
		case ID_DISCONNECTION_NOTIFICATION:
			if (m_bIsServer)
				m_ToolBox->Log(LOGINFORMATION, _T("Win32 Network Peer %s: A client has disconnected.\n"), GetName()->GetString());
			else 
				m_ToolBox->Log(LOGINFORMATION, _T("Win32 Network Peer %s: The client has been disconnected.\n"), GetName()->GetString());
			break;
		case ID_CONNECTION_LOST:
			if (m_bIsServer)
				m_ToolBox->Log(LOGINFORMATION, _T("Win32 Network Peer %s: A client has been lost connection.\n"), GetName()->GetString());
			else 
				m_ToolBox->Log(LOGINFORMATION, _T("Win32 Network Peer %s: The client has been lost connection.\n"), GetName()->GetString());
			break;
		case ID_USER_PACKET_ENUM:
			OnReceivePacket( pPacket );
			break;
		default:
			m_ToolBox->Log(LOGINFORMATION, _T("Win32 Network Peer %s: Message with identifier %i has arrived.\n"), GetName()->GetString(), pPacket->data[0]);
			break;
		}
		m_pPeer->DeallocatePacket(pPacket);
	}

	return true;
}

DWORD CWin32NetworkPeer::OnStartHost( DWORD size, void *params  )
{
	VERIFY_MESSAGE_SIZE(size, sizeof(CREATENETWORKPEER));
	CREATENETWORKPEER* pNetHost = (CREATENETWORKPEER*)params;

	m_pPeer->Startup( pNetHost->iMaxClients, pNetHost->iThreadSleepTimer, 
		&SocketDescriptor( pNetHost->iServerPort, 0 ), 1 );
	m_pPeer->SetMaximumIncomingConnections(pNetHost->iMaxClients);
	m_bIsServer = true;
	return MSG_HANDLED_STOP;
}

DWORD CWin32NetworkPeer::OnStartClient( DWORD size, void *params )
{
	VERIFY_MESSAGE_SIZE(size, sizeof(CREATENETWORKPEER));
	CREATENETWORKPEER* pNetHost = (CREATENETWORKPEER*)params;

	m_pPeer->Startup( 1, pNetHost->iThreadSleepTimer, &SocketDescriptor(), 1 );
	m_bIsServer = false;

	return MSG_HANDLED_STOP;
}

//Note: Connect() returning true does NOT mean you are connected. It means you should wait for 
//the message ID_CONNECTION_REQUEST_ACCEPTED or ID_CONNECTION_ATTEMPT_FAILED. In the meantime, 
//your GUI should show a waiting screen.
DWORD CWin32NetworkPeer::OnConnect( DWORD size, void *params )
{
	VERIFY_MESSAGE_SIZE(size, sizeof(CONNECTPEER));
	CONNECTPEER* pNetHost = (CONNECTPEER*)params;
	
	if( pNetHost->iPasswordSize > 0 )
		pNetHost->bResult = m_pPeer->Connect( pNetHost->szAddress, pNetHost->iServerPort, pNetHost->szPassword, pNetHost->iPasswordSize );
	else
		pNetHost->bResult = m_pPeer->Connect( pNetHost->szAddress, pNetHost->iServerPort, 0, 0 );

	return MSG_HANDLED_STOP;
}

DWORD CWin32NetworkPeer::OnDisconnect( DWORD size, void *params )
{
	VERIFY_MESSAGE_SIZE(size, sizeof(DISCONNECTPEER) );
	DISCONNECTPEER* pNet = (DISCONNECTPEER*)params;

	m_pPeer->CloseConnection( m_pPeer->GetSystemAddressFromIndex(0), pNet->bNotify, pNet->iChannel );
	pNet->bResult = true;

	return MSG_HANDLED_STOP;
}

DWORD CWin32NetworkPeer::OnSendNetworkPacket( DWORD size, void *params )
{
	VERIFY_MESSAGE_SIZE(size, sizeof(SENDNETWORKPACKETPARAMS) );
	SENDNETWORKPACKETPARAMS* pSNPP = (SENDNETWORKPACKETPARAMS*)params;

	if (!m_pPeer)
		return MSG_ERROR;

	int iPacketSize = pSNPP->iDataSize + 1; // data with packet identifier
	if (iPacketSize > SENDPACKETBUFFERSIZE)
	{
		// packet too big
		return MSG_ERROR;
	}

	// build packet with packet identifier prefixed
	m_pSendPacketBuffer[0] = ID_USER_PACKET_ENUM;
	memcpy( &m_pSendPacketBuffer[1], pSNPP->pData, pSNPP->iDataSize );

	bool bResult = m_pPeer->Send( (const char*)m_pSendPacketBuffer, iPacketSize, HIGH_PRIORITY, RELIABLE_ORDERED, 0, UNASSIGNED_SYSTEM_ADDRESS, true );
	if (!bResult)
	{
		return MSG_ERROR;
	}

	return MSG_HANDLED_STOP;
}

void CWin32NetworkPeer::OnReceivePacket( Packet *pPacket )
{
	int iDataSize = (pPacket->bitSize / 8) - 1; // data in bytes without packet identifier
	m_pReceivedPacketArchive->SetIsWriting( true );
	m_pReceivedPacketArchive->SeekTo( 0 );
	m_pReceivedPacketArchive->Write( pPacket->data, iDataSize );

	RECVNETWORKPACKETPARAMS rnpp;
	rnpp.pArchive = m_pReceivedPacketArchive;
	static DWORD msgRecvNetworkPacket = CHashString(_T("RecvNetworkPacket")).GetUniqueID();
	m_ToolBox->SendMessage(msgRecvNetworkPacket, sizeof(rnpp), &rnpp);
}
