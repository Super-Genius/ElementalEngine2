///==========================================================================
/// \file	FlackSquadNetwork.h
/// \brief	Component for flacksquad network messages
///==========================================================================

#ifndef _FLACKSQUADNETWORK_H_
#define _FLACKSQUADNETWORK_H_

// tolua_begin
#define NETWORKMSGID_SPAWNOBJECT			1
#define NETWORKMSGID_DESTROYOBJECT			2
#define NETWORKMSGID_OBJECTSELECTTARGET		3
#define NETWORKMSGID_TURRETANGLES			4
#define NETWORKMSGID_TURRETSTARTFIRE		5
#define NETWORKMSGID_TURRETSTOPFIRE			6
#define NETWORKMSGID_TURRETMISSILEFIRE		7
#define NETWORKMSGID_PLAYERDIED				8
#define NETWORKMSGID_CHANGEMAPPOSITION		9
#define NETWORKMSGID_PLAYERSPAWN			10
#define NETWORKMSGID_MISSIONREADY			11
#define NETWORKMSGID_ENDMISSION				12
#define NETWORKMSGID_UPDATEHEALTH			13
#define NETWORKMSGID_OBJECTSETWAYPOINT		14
#define NETWORKMSGID_RESTARTMISSION			15
#define	NETWORKMSGID_SPAWNEVENT				16

struct NETWORKMSG_SPAWNOBJECTPARAMS
{
	NETWORKMSG_SPAWNOBJECTPARAMS()
	{
		iMsgID = NETWORKMSGID_SPAWNOBJECT;
	}
	UINT iMsgID; // must be first
	UINT iTypeID;
	UINT iObjectID;
	UINT iFormationID;
	UINT iEventID;
	Vec3 v3SpawnPosition;
};

struct NETWORKMSG_DESTROYOBJECTPARAMS
{
	NETWORKMSG_DESTROYOBJECTPARAMS()
	{
		iMsgID = NETWORKMSGID_DESTROYOBJECT;
	}
	UINT iMsgID; // must be first
	UINT iTypeID;
	UINT iObjectID;
};

struct NETWORKMSG_OBJECTSELECTTARGETPARAMS
{
	NETWORKMSG_OBJECTSELECTTARGETPARAMS()
	{
		iMsgID = NETWORKMSGID_OBJECTSELECTTARGET;
	}
	UINT iMsgID; // must be first
	UINT iTypeID;
	UINT iObjectID;
	UINT iPlayerLocationID;
};

struct NETWORKMSG_OBJECTSETWAYPOINTPARAMS
{
	NETWORKMSG_OBJECTSETWAYPOINTPARAMS()
	{
		iMsgID = NETWORKMSGID_OBJECTSETWAYPOINT;
	}
	UINT iMsgID; // must be first
	UINT iTypeID;
	UINT iObjectID;
	Vec3 v3Waypoint;
};

struct NETWORKMSG_TURRETSTARTFIREPARAMS
{
	NETWORKMSG_TURRETSTARTFIREPARAMS()
	{
		iMsgID = NETWORKMSGID_TURRETSTARTFIRE;
	}
	UINT iMsgID; // must be first
	UINT iPlayerID;
	UINT iWeapon;
};

struct NETWORKMSG_TURRETMISSILEFIREPARAMS
{
	NETWORKMSG_TURRETMISSILEFIREPARAMS()
	{
		iMsgID = NETWORKMSGID_TURRETMISSILEFIRE;
	}
	UINT iMsgID;
	UINT iPlayerID;
	UINT iTypeID;
	UINT iObjectID;
	UINT iMissileTypeID;
};

struct NETWORKMSG_TURRETANGLESPARAMS
{
	NETWORKMSG_TURRETANGLESPARAMS()
	{
		iMsgID = NETWORKMSGID_TURRETANGLES;
		fPitch = 0.0f;
	}
	UINT iMsgID; // must be first
	UINT iPlayerID;
	EulerAngle eulTurretOrientation;
	float fPitch;
};

struct NETWORKMSG_TURRETSTOPFIREPARAMS
{
	NETWORKMSG_TURRETSTOPFIREPARAMS()
	{
		iMsgID = NETWORKMSGID_TURRETSTOPFIRE;
	}
	UINT iMsgID; // must be first
	UINT iPlayerID;
};

struct NETWORKMSG_CHANGEMAPPOSITIONPARAMS
{
	NETWORKMSG_CHANGEMAPPOSITIONPARAMS()
	{
		iMsgID = NETWORKMSGID_CHANGEMAPPOSITION;
	}
	UINT iMsgID; // must be first
	UINT iMapPositionID;
};

struct NETWORKMSG_PLAYERSPAWNPARAMS
{
	NETWORKMSG_PLAYERSPAWNPARAMS()
	{
		iMsgID = NETWORKMSGID_PLAYERSPAWN;
	}
	UINT iMsgID; // must be first
	bool bIsSelf;
	UINT iPlayerID;
	UINT iMapPositionID;
};

struct NETWORKMSG_MISSIONREADYPARAMS
{
	NETWORKMSG_MISSIONREADYPARAMS()
	{
		iMsgID = NETWORKMSGID_MISSIONREADY;
		iPlayerID = 0;
	}
	UINT iMsgID; // must be first
	UINT iPlayerID;
};

struct NETWORKMSG_ENDMISSIONPARAMS
{
	NETWORKMSG_ENDMISSIONPARAMS()
	{
		iMsgID = NETWORKMSGID_ENDMISSION;
		bGameOver = false;
		bVictory = false;
	}
	UINT iMsgID; // must be first
	bool bGameOver;
	bool bVictory;
};

struct NETWORKMSG_RESTARTMISSIONPARAMS
{
	NETWORKMSG_RESTARTMISSIONPARAMS()
	{
		iMsgID = NETWORKMSGID_RESTARTMISSION;
	}
	UINT iMsgID; // must be first
};

struct NETWORKMSG_UPDATEHEALTHPARAMS
{
	NETWORKMSG_UPDATEHEALTHPARAMS()
	{
		iMsgID = NETWORKMSGID_UPDATEHEALTH;
		fHealth = 0;
		bAdjustHealthEvent = false;
	}
	UINT iMsgID; // must be first
	float fHealth;
	bool bAdjustHealthEvent;
};

struct NETWORKMSG_SPAWNEVENTPARAMS
{
	NETWORKMSG_SPAWNEVENTPARAMS()
	{
		iMsgID = NETWORKMSGID_SPAWNEVENT;
	}
	UINT iMsgID; // must be first
	UINT iEventID;
	UINT iEventObjectID;
	UINT iFormationID;
	UINT iFormationObjectID;
};

// tolua_end

class CFlackSquadNetwork : public IComponent
{
private:
	SINGLETONCONSTRUCTOROVERRIDE( CFlackSquadNetwork );
	CFlackSquadNetwork();

public:
	/// \brief	Default Destructor
	~CFlackSquadNetwork();

	static IComponent *Create(int nArgs, va_list argptr);

	void Serialize(IArchive &ar) { }
	IHashString *GetComponentType();
	bool IsKindOf(IHashString *compType);

	DWORD OnRecvNetworkPacket( DWORD size, void *param );
	DWORD OnNetworkStateChanged( DWORD size, void *param );
	DWORD OnNetworkGameSearchComplete( DWORD size, void *param );
	DWORD OnNetworkPlayerLeaving( DWORD size, void *param );
	DWORD OnNetworkPlayerJoining(DWORD size, void *param);
	DWORD OnNetworkPlayerReadinessChanged(DWORD size, void *param);
	DWORD OnXBoxGameInviteAccepted(DWORD size, void *param);

private:
	IToolBox* m_ToolBox;
	CHashString m_hsComponentType;
};

#endif //_FLACKSQUADNETWORK_H_
