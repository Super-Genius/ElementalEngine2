///============================================================================
/// \file		WorldEventTrigger.h
/// \brief		Header file for a World Event Trigger
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

#ifndef	WORLDEVENTTRIGGER_H
#define	WORLDEVENTTRIGGER_H

#include "CObjectTemplate.hpp"

typedef std::list<IObject*> WORLDEVENTPTRLIST;
class CWorldEventManager;

class CWorldEventTrigger : public CObjectTemplate<IObject>
{
protected:
	/// \brief Default Constructor
	/// \param parentName = name of parent in hierarchy
	/// \param name = IHashString * name of this object
	CWorldEventTrigger(IHashString *parentName, IHashString *name);

public:
	/// \briefDefault Destructor
	~CWorldEventTrigger();

	/// \brief Create an instance
	/// \return	Pointer to this
	static IComponent *Create(int nArgs, va_list argptr);

	virtual void Serialize(IArchive &ar);
	virtual void Init();
	virtual void DeInit();

	DWORD OnAddWorldEvent(DWORD size, void *in_params);
	DWORD OnRemoveWorldEvent(DWORD size, void *in_params);
	DWORD OnQueryWorldEvents(DWORD size, void *in_params);
	DWORD OnGetWorldEventInfo(DWORD size, void *in_params);

	bool BroadcastWorldEvent(BROADCASTWORLDEVENTPARAMS *pParams);

	const CHashString& GetEntityName() const { return m_szEntityName; }
	const CHashString& GetEntityType() const { return m_szEntityType; }
	const CHashString& GetEventName() const { return m_szEventName; }
	WORLDEVENTPTRLIST* GetWorldEventList() { return &m_WorldEvents; }

	bool IsTriggeredByEntityName() const;
	bool IsTriggeredByEntityType() const;

private:
	CWorldEventManager *m_pWorldEventManager;
	CHashString m_szEntityName;
	CHashString m_szEntityType;
	CHashString m_szEventName;
	WORLDEVENTPTRLIST m_WorldEvents;
};

#endif