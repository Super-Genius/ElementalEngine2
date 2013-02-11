///============================================================================
/// \file		WorldEventManager.h
/// \brief		Header file for a World Event Manager
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

#ifndef	WORLDEVENTMANAGER_H
#define	WORLDEVENTMANAGER_H

#include "CManager.h"
#include "lua.h"

class CWorldEventManager : public CManager
{
private:
	SINGLETONCONSTRUCTOROVERRIDE(CWorldEventManager);
	/// \brief The Default Constructor
	/// \return void
	CWorldEventManager();
public:
	/// \brief The Default Deconstructor
	/// \return void
	virtual ~CWorldEventManager();

	/// \brief	Function to get this component.  This is a singleton
	/// \param	void
	/// \return	IComponent*: This Manager
	static IComponent *Create(int nArgs, va_list argptr);

	virtual bool Init();
	virtual bool DeInit();

	void AddWorldEventTrigger( CWorldEventTrigger *pTrigger );
	void DeleteWorldEventTrigger( CWorldEventTrigger *pTrigger );

	///	\brief	Start the manager
	///	\param	size = 0
	///	\param	data = void pointer to NULL
	///	\return DWROD = message return code, MSG_HANDLED_STOP, MSG_HANDLED_PROCEED,
	///						error
	DWORD OnStart(DWORD size, void *data);

	///	\brief	Stop the manager
	///	\param	size = 0
	///	\param	data = void pointer to NULL
	///	\return DWROD = message return code, MSG_HANDLED_STOP, MSG_HANDLED_PROCEED,
	///						error
	DWORD OnStop(DWORD size, void *data);

	DWORD OnQueryWorldEventTriggers(DWORD size, void *in_params);

	DWORD OnGetWorldEvents(DWORD size, void *in_params);

	DWORD OnFindWorldEventTrigger(DWORD size, void *in_params);

	DWORD OnBroadcastWorldEvent(DWORD size, void *in_params);

	void RegisterLuaDefinitions();
	// ------------------ Messages ------------------ //

private:
	bool m_bIsRunning;

	CHashString m_hsWorldEventTriggerTypeName;
	CHashString m_hsWorldEventTypeName;

	lua_State *m_LuaState;

	/// map entity name to list of triggers
	WORLDEVENTTRIGGERMAP m_EntityNameToTriggerListMap;
	/// map entity type to list of triggers
	WORLDEVENTTRIGGERMAP m_EntityTypeToTriggerListMap;
	/// map entity name to list of events
	WORLDEVENTTRIGGERMAP m_EntityNameToEventListMap;
	/// map entity type to list of events
	WORLDEVENTTRIGGERMAP m_EntityTypeToEventListMap;
};

#endif
