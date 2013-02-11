///============================================================================
/// \file		WorldEventDefines.h
/// \brief		Header file for a World Event Defines
/// \date		11-16-2006
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

#ifndef	WORLDEVENTDEFINES_H
#define	WORLDEVENTDEFINES_H

// tolua_begin

struct BROADCASTWORLDEVENTPARAMS
{
	IHashString *pEntityName;
	IHashString *pEntityType;
	IHashString *pEventName;
};

struct TRIGGERWORLDEVENTPARAMS
{
	IHashString *pEntityName;
	IHashString *pEntityType;
	IHashString *pEventName;
};

// tolua_end

typedef std::list<DWORD> WORLDEVENTLIST;
typedef std::map<DWORD, WORLDEVENTLIST> WORLDEVENTTRIGGERMAP;

struct QUERYWORLDEVENTSPARAMS
{
	QUERYWORLDEVENTSPARAMS()
	{
		pWorldEventList = NULL;
	}

	WORLDEVENTLIST *pWorldEventList;
};

struct QUERYWORLDEVENTTRIGGERSPARAMS
{
	QUERYWORLDEVENTTRIGGERSPARAMS()
	{
		pEntityName = NULL;
		pEntityType = NULL;
		pWorldEventTriggerMap = NULL;
	}

	IHashString *pEntityName;
	IHashString *pEntityType;
	WORLDEVENTTRIGGERMAP *pWorldEventTriggerMap;
};

struct WORLDEVENTINFOPARAMS
{
	WORLDEVENTINFOPARAMS()
	{
		bUseEntityName = false;
		bUseEntityType = false;
	}
	CHashString hszEventName;
	CHashString hszEntityName;
	CHashString hszEntityType;
	CHashString hszEventParam;
	bool bUseEntityName;
	bool bUseEntityType;
};

struct FINDWORLDEVENTPARAMS
{
	FINDWORLDEVENTPARAMS()
	{
		pEntityName = NULL;
		pEntityType = NULL;
		pEventName = NULL;
		pFoundTriggerName = NULL;
		bFound = false;
	}
	IHashString *pEntityName;
	IHashString *pEntityType;
	IHashString *pEventName;
	IHashString *pFoundTriggerName;
	bool bFound;
};

#endif
