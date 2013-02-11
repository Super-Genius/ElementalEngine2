///============================================================================
/// \file		WorldEvent.h
/// \brief		Header file for a World Event
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

#ifndef	WORLDEVENT_H
#define	WORLDEVENT_H

#include "CObjectTemplate.hpp"

class CWorldEvent : public CObjectTemplate<IObject>
{
protected:
	/// \brief Default Constructor
	/// \param parentName = name of parent in hierarchy
	/// \param name = IHashString * name of this object
	CWorldEvent(IHashString *parentName, IHashString *name);

public:
	/// \briefDefault Destructor
	~CWorldEvent();

	/// \brief Create an instance
	/// \return	Pointer to this
	static IComponent *Create(int nArgs, va_list argptr);

	virtual void Serialize(IArchive &ar);
	virtual void Init();
	virtual void DeInit();

	DWORD OnTriggerWorldEvent(DWORD size, void *in_params);
	DWORD OnGetWorldEventInfo(DWORD size, void *in_params);
	DWORD OnSetWorldEventEvent(DWORD size, void *in_params);

	const CHashString& GetEntityName() const { return m_szEntityName; }

	bool IsTriggerEntityByName() const;
	bool IsTriggerEntityByType() const;

private:
	CHashString m_szTriggerName;
	CHashString m_szEntityName;
	CHashString m_szEntityType;
	CHashString m_szEventName;
	CHashString m_szEventParam;

	IArchive *m_TriggerArchive;
};

#endif