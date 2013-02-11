///============================================================================
/// \file	NetworkMessages.h
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

#ifndef NETWORKMESSAGES_H
#define NETWORKMESSAGES_H

namespace ElementalEngine
{

// tolua_begin

struct SEARCHFORNETWORKGAMESPARAMS
{
	UINT dwUserIndex;

	SEARCHFORNETWORKGAMESPARAMS()
	{
		dwUserIndex = 0;
	}
};

struct SENDNETWORKPACKETPARAMS
{
	UINT iTargetPlayerIndex;
	bool bReliable;
	bool bSequential;
	void *pData;
	DWORD iDataSize;

	SENDNETWORKPACKETPARAMS()
	{
		iTargetPlayerIndex = 0xFFFFFFFF;
		bReliable = true;
		bSequential = true;
		pData = NULL;
		iDataSize = 0;
	}
};

struct RECVNETWORKPACKETPARAMS
{
	IArchive *pArchive;

	RECVNETWORKPACKETPARAMS()
	{
		pArchive = NULL;
	}
};

enum NETWORKSTATE
{
	NETWORKSTATEIDLE, 
	NETWORKSTATEHOSTING, 
	NETWORKSTATEJOINING, 
	NETWORKSTATELOBBY, 
	NETWORKSTATEGAMEPLAY, 
};

struct NETWORKSTATECHANGEDPARAMS
{
	NETWORKSTATE OldState;
	NETWORKSTATE NewState;
};

struct NETWORKPLAYERLEAVINGPARAMS
{
};

struct NETWORKPLAYERJOININGPARAMS
{
};

struct GAMESEARCHINFO
{
	DWORD dwGameHostHash;
	int iPing;
	GAMESEARCHINFO()
	{
		dwGameHostHash = 0;
		iPing = 0;
	}
};

// tolua_end
typedef vector<GAMESEARCHINFO> GAMESEARCHVECTOR;
// tolua_begin

// quick lua wrapper class
class CLuaGameSearchVector
{
public:
	~CLuaGameSearchVector() { m_Vector.clear(); }
	inline int GetSize() { return (int)m_Vector.size(); }
	inline GAMESEARCHINFO GetEntry(int index) { return m_Vector[index]; }

	// tolua_end
	GAMESEARCHVECTOR m_Vector;
	// tolua_begin
};

struct NETWORKGAMESEARCHCOMPLETEPARAMS
{
	CLuaGameSearchVector vGameList;
	int iNumGames;
	NETWORKGAMESEARCHCOMPLETEPARAMS()
	{
		iNumGames = 0;	
	}
};

struct NETWORKHOSTGAMEPARAMS
{
	UINT dwUserIndex;
	bool bLocalGame;

	NETWORKHOSTGAMEPARAMS()
	{
		dwUserIndex = 0;
		bLocalGame = false;
	}
};

struct NETWORKJOINGAMEPARAMS
{
	UINT dwGameIndex;
	NETWORKJOINGAMEPARAMS()
	{
		dwGameIndex = 0;
	}
};

struct NETWORKGAMEPLAYER
{
	DWORD dwPlayerNameHash;
	int iPlayerIndex;
	bool bReadyToPlay;
	NETWORKGAMEPLAYER()
	{
		dwPlayerNameHash = 0;
		iPlayerIndex = 0;
		bReadyToPlay = false;
	}
};

// tolua_end

typedef vector<NETWORKGAMEPLAYER> NETWORKGAMEPLAYERVECTOR;

// tolua_begin
class CLuaGamePlayerList
{
public:
	~CLuaGamePlayerList() { m_PlayerList.clear(); }
	inline size_t GetSize() { return m_PlayerList.size(); }
	inline NETWORKGAMEPLAYER GetEntry(int index) { return m_PlayerList[index]; }
	
	// tolua_end
	NETWORKGAMEPLAYERVECTOR m_PlayerList;
	// tolua_begin
};

struct GAMEPLAYERLISTUPDATEPARAMS
{
	UINT iNumPlayers;
	CLuaGamePlayerList vPlayerList;
	GAMEPLAYERLISTUPDATEPARAMS()
	{
		iNumPlayers = 0;
	}
};

struct LOCALPLAYERREADYPARAMS
{
	bool bReadyToPlay;
	LOCALPLAYERREADYPARAMS()
	{
		bReadyToPlay = false;
	}
};

// tolua_end

}	//namespace ElementalEngine
#endif //NETWORKMESSAGES_H