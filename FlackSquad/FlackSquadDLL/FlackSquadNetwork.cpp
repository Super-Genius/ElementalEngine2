///==========================================================================
/// \file	CFlackSquadNetwork.cpp
/// \brief	Component for network management
///==========================================================================

#include "stdafx.h"

REGISTER_COMPONENT_SINGLETON(CFlackSquadNetwork);
REGISTER_MESSAGE_HANDLER(RecvNetworkPacket, OnRecvNetworkPacket, CFlackSquadNetwork);
REGISTER_MESSAGE_HANDLER(NetworkStateChanged, OnNetworkStateChanged, CFlackSquadNetwork);
REGISTER_MESSAGE_HANDLER(NetworkGameSearchComplete, OnNetworkGameSearchComplete, CFlackSquadNetwork);
REGISTER_MESSAGE_HANDLER(NetworkPlayerLeaving, OnNetworkPlayerLeaving, CFlackSquadNetwork);
REGISTER_MESSAGE_HANDLER(NetworkPlayerJoining, OnNetworkPlayerJoining, CFlackSquadNetwork);
REGISTER_MESSAGE_HANDLER(NetworkPlayerReadinessChanged, OnNetworkPlayerReadinessChanged, CFlackSquadNetwork);
REGISTER_MESSAGE_HANDLER_PRIORITY(XBoxGameInviteAccepted, OnXBoxGameInviteAccepted, CFlackSquadNetwork, 0x0000ffff);

IComponent *CFlackSquadNetwork::Create(int nArgs, va_list argptr)
{
	return SINGLETONINSTANCE(CFlackSquadNetwork);
}

CFlackSquadNetwork::CFlackSquadNetwork() : 
	m_hsComponentType(_T("CFlackSquadNetwork"))
{
	m_ToolBox = EngineGetToolBox();
}

CFlackSquadNetwork::~CFlackSquadNetwork()
{
}

IHashString *CFlackSquadNetwork::GetComponentType()
{
	return &m_hsComponentType;
}

bool CFlackSquadNetwork::IsKindOf(IHashString *compType)
{
	return (compType->GetUniqueID() == m_hsComponentType.GetUniqueID());
}

DWORD CFlackSquadNetwork::OnRecvNetworkPacket( DWORD size, void *param )
{
	VERIFY_MESSAGE_SIZE( size, sizeof(RECVNETWORKPACKETPARAMS) );
	RECVNETWORKPACKETPARAMS *rnpp = (RECVNETWORKPACKETPARAMS*)param;

	static CHashString hsNotifyEvent(_T("Mission_NetworkReceiveMessage_Event"));
	static CHashString hsStateObjectName(_T("GameStateObject"));
	static CHashString hsStateObjectType = _T("CQHStateObject");

	TRIGGEREVENTPARAMS tep;
	tep.EventName = &hsNotifyEvent;
	tep.EventParamsArchive = rnpp->pArchive;
	static DWORD msgTriggerEvent = CHashString(_T("TriggerEvent")).GetUniqueID();
	EngineGetToolBox()->SendMessage(msgTriggerEvent, sizeof(tep), &tep, &hsStateObjectName, &hsStateObjectType);

	return MSG_HANDLED_STOP;
}

DWORD CFlackSquadNetwork::OnNetworkStateChanged( DWORD size, void *param )
{
	VERIFY_MESSAGE_SIZE( size, sizeof(NETWORKSTATECHANGEDPARAMS) );
	NETWORKSTATECHANGEDPARAMS &nscp = *(NETWORKSTATECHANGEDPARAMS*)param;

	switch (nscp.NewState)
	{
	case NETWORKSTATEIDLE:
		{
			static CHashString hsNotifyEvent(_T("Game_Multiplayer_NotifyDisconnected_Event"));
			static CHashString hsStateObjectName(_T("GameStateObject"));
			static CHashString hsStateObjectType = _T("CQHStateObject");

			TRIGGEREVENTPARAMS tep;
			tep.EventName = &hsNotifyEvent;
			tep.EventParamsArchive = NULL;
			static DWORD msgTriggerEvent = CHashString(_T("TriggerEvent")).GetUniqueID();
			EngineGetToolBox()->SendMessage(msgTriggerEvent, sizeof(tep), &tep, &hsStateObjectName, &hsStateObjectType);
		}
		break;
	case NETWORKSTATEGAMEPLAY:
		{
			static CHashString hsNotifyEvent(_T("Multiplayer_NotifyGameStart_Event"));
			static CHashString hsStateObjectName(_T("GameStateObject"));
			static CHashString hsStateObjectType = _T("CQHStateObject");

			TRIGGEREVENTPARAMS tep;
			tep.EventName = &hsNotifyEvent;
			tep.EventParamsArchive = NULL;
			static DWORD msgTriggerEvent = CHashString(_T("TriggerEvent")).GetUniqueID();
			EngineGetToolBox()->SendMessage(msgTriggerEvent, sizeof(tep), &tep, &hsStateObjectName, &hsStateObjectType);
		}
		break;
	case NETWORKSTATELOBBY:
		{
			static CHashString hsNotifyEvent(_T("Multiplayer_NotifyGameLobby_Event"));
			static CHashString hsStateObjectName(_T("GameStateObject"));
			static CHashString hsStateObjectType = _T("CQHStateObject");

			TRIGGEREVENTPARAMS tep;
			tep.EventName = &hsNotifyEvent;
			tep.EventParamsArchive = NULL;
			static DWORD msgTriggerEvent = CHashString(_T("TriggerEvent")).GetUniqueID();
			EngineGetToolBox()->SendMessage(msgTriggerEvent, sizeof(tep), &tep, &hsStateObjectName, &hsStateObjectType);
			break;
		}
	};

	return MSG_HANDLED_STOP;
}

DWORD CFlackSquadNetwork::OnNetworkGameSearchComplete( DWORD size, void *param )
{
	VERIFY_MESSAGE_SIZE( size, sizeof(NETWORKGAMESEARCHCOMPLETEPARAMS) );
	NETWORKGAMESEARCHCOMPLETEPARAMS &ngsc = *(NETWORKGAMESEARCHCOMPLETEPARAMS*)param;

	static CHashString hsNotifyEvent(_T("Multiplayer_NotifyGameSearchComplete_Event"));
	static CHashString hsStateObjectName(_T("GameStateObject"));
	static CHashString hsStateObjectType = _T("CQHStateObject");

	CREATEARCHIVE ca;
	CHashString memType(_T("Memory"));

	ca.mode = STREAM_MODE_WRITE | STREAM_MODE_READ;
	ca.streamData = NULL;
	ca.streamSize = 0;
	ca.streamType = &memType;

	static DWORD msgHash_CreateArchive = CHashString( _T("CreateArchive") ).GetUniqueID();
	m_ToolBox->SendMessage( msgHash_CreateArchive, sizeof(CREATEARCHIVE), &ca );

	IArchive *archive = ca.archive;
	archive->SetIsWriting(true);
	archive->SeekTo(0);
	archive->Write(ngsc.iNumGames);
	for (int i=0; i<ngsc.iNumGames; i++)
	{
		GAMESEARCHINFO info = ngsc.vGameList.GetEntry(i);
		archive->Write(&info, sizeof(GAMESEARCHINFO));
	}

	TRIGGEREVENTPARAMS tep;
	tep.EventName = &hsNotifyEvent;
	tep.EventParamsArchive = archive;
	static DWORD msgTriggerEvent = CHashString(_T("TriggerEvent")).GetUniqueID();
	EngineGetToolBox()->SendMessage(msgTriggerEvent, sizeof(tep), &tep, &hsStateObjectName, &hsStateObjectType);

	return MSG_HANDLED_STOP;
}

DWORD CFlackSquadNetwork::OnNetworkPlayerLeaving( DWORD size, void *param )
{
	VERIFY_MESSAGE_SIZE( size, sizeof(NETWORKPLAYERLEAVINGPARAMS) );
	NETWORKPLAYERLEAVINGPARAMS &nplp = *(NETWORKPLAYERLEAVINGPARAMS*)param;

	static CHashString hsNotifyEvent(_T("Game_CoOp_PlayerDisconnected_Event"));
	static CHashString hsStateObjectName(_T("GameStateObject"));
	static CHashString hsStateObjectType = _T("CQHStateObject");

	TRIGGEREVENTPARAMS tep;
	tep.EventName = &hsNotifyEvent;
	tep.EventParamsArchive = NULL;
	static DWORD msgTriggerEvent = CHashString(_T("TriggerEvent")).GetUniqueID();
	EngineGetToolBox()->SendMessage(msgTriggerEvent, sizeof(tep), &tep, &hsStateObjectName, &hsStateObjectType);

	return MSG_HANDLED_STOP;
}

DWORD CFlackSquadNetwork::OnNetworkPlayerJoining(DWORD size, void *param)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(NETWORKPLAYERJOININGPARAMS));
	NETWORKPLAYERJOININGPARAMS &npjp = *(NETWORKPLAYERJOININGPARAMS*) param;

	static CHashString hsNotifyEvent(_T("Game_CoOp_PlayerConnected_Event"));
	static CHashString hsStateObjectName(_T("GameStateObject"));
	static CHashString hsStateObjectType = _T("CQHStateObject");

	TRIGGEREVENTPARAMS tep;
	tep.EventName = &hsNotifyEvent;
	tep.EventParamsArchive = NULL;
	static DWORD msgTriggerEvent = CHashString(_T("TriggerEvent")).GetUniqueID();
	EngineGetToolBox()->SendMessage(msgTriggerEvent, sizeof(tep), &tep, &hsStateObjectName, &hsStateObjectType);

	return MSG_HANDLED_STOP;
}

DWORD CFlackSquadNetwork::OnNetworkPlayerReadinessChanged(DWORD size, void *param)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(NETWORKGAMEPLAYER));
	NETWORKGAMEPLAYER &ngp = *(NETWORKGAMEPLAYER*)param;

	static CHashString hsNotifyEvent(_T("Multiplayer_NotifyPlayerReadinessChanged_Event"));
	static CHashString hsStateObjectName(_T("GameStateObject"));
	static CHashString hsStateObjectType = _T("CQHStateObject");

	CREATEARCHIVE ca;
	CHashString memType(_T("Memory"));

	ca.mode = STREAM_MODE_WRITE | STREAM_MODE_READ;
	ca.streamData = NULL;
	ca.streamSize = 0;
	ca.streamType = &memType;

	static DWORD msgHash_CreateArchive = CHashString( _T("CreateArchive") ).GetUniqueID();
	m_ToolBox->SendMessage( msgHash_CreateArchive, sizeof(CREATEARCHIVE), &ca );

	IArchive *archive = ca.archive;
	archive->SetIsWriting(true);
	archive->SeekTo(0);
	archive->Write(&ngp, sizeof(NETWORKGAMEPLAYER));

	TRIGGEREVENTPARAMS tep;
	tep.EventName = &hsNotifyEvent;
	tep.EventParamsArchive = archive;
	static DWORD msgTriggerEvent = CHashString(_T("TriggerEvent")).GetUniqueID();
	EngineGetToolBox()->SendMessage(msgTriggerEvent, sizeof(tep), &tep, &hsStateObjectName, &hsStateObjectType);	

	return MSG_HANDLED_STOP;
}

DWORD CFlackSquadNetwork::OnXBoxGameInviteAccepted(DWORD size, void *param)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(DWORD));

	DWORD playerIndex = *(DWORD*)param;

	static CHashString hsNotifyEvent(_T("Game_GameInviteAccepted_Event"));
	static CHashString hsStateObjectName(_T("GameStateObject"));
	static CHashString hsStateObjectType = _T("CQHStateObject");

	CREATEARCHIVE ca;
	CHashString memType(_T("Memory"));

	ca.mode = STREAM_MODE_WRITE | STREAM_MODE_READ;
	ca.streamData = NULL;
	ca.streamSize = 0;
	ca.streamType = &memType;

	static DWORD msgHash_CreateArchive = CHashString( _T("CreateArchive") ).GetUniqueID();
	m_ToolBox->SendMessage( msgHash_CreateArchive, sizeof(CREATEARCHIVE), &ca );

	IArchive *archive = ca.archive;
	archive->SetIsWriting(true);
	archive->SeekTo(0);
	archive->Write((int)playerIndex);

	TRIGGEREVENTPARAMS tep;
	tep.EventName = &hsNotifyEvent;
	tep.EventParamsArchive = archive;
	static DWORD msgTriggerEvent = CHashString(_T("TriggerEvent")).GetUniqueID();
	EngineGetToolBox()->SendMessage(msgTriggerEvent, sizeof(tep), &tep, &hsStateObjectName, &hsStateObjectType);

	return MSG_HANDLED_PROCEED;
}