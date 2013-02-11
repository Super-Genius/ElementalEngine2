///=====================================================================
/// \file	CObjectFactory.h
/// \brief	Definition of CObjectFactory Class
///         This file defines a CObjectFactory class which handles
///         all internal engine object creation.
/// \date	05/01/2005
/// \author	Brian Bazyk
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
///=====================================================================

#ifndef _CCOBJECTFACTORY_H__
#define _CCOBJECTFACTORY_H__

class CObjectFactory : public IComponent
{
	// Internal type
	typedef map<DWORD, IComponent*> ObjectKeyMap_t;
	typedef set<DWORD> UniqueIDSet_t;

	SINGLETONCONSTRUCTOROVERRIDE(CObjectFactory);

public:
	/// \brief The Default Constructor
	CObjectFactory();

	/// \brief The Default Destructor
	~CObjectFactory();

	/// \brief Function to get this component.  This is a singleton
	/// \return	IComponent*: the factory
	static IComponent* Create( int nArgs, va_list argptr );

	/// Serialize this component to the archive.
	///	\param ar = reference to the archive class
	virtual void Serialize(IArchive &ar);

	/// return a string of the component type
	/// \return IHashString * pointer to textual classname
	virtual IHashString *GetComponentType();

	/// Checks if this component type is similar to a base class's componet type
	/// \return true if similar, else false
	virtual bool IsKindOf(IHashString *compType);

	/// delete the objects in the delayed delete list
	void DeleteDelayedObjects();

	// ---Messages---
	DWORD OnAddObject(DWORD size, void *params);
	DWORD OnCreateObject(DWORD size, void *params);
	DWORD OnDeleteObject(DWORD size, void *params);
	/// Delay object deletion until the end of the update cycle
	/// \param size - sizeof(DELETEOBJECTPARAMS)
	/// \param params - ptr to DELETEOBJECTPARAMS struct
	DWORD OnDelayedDeleteObject(DWORD size, void *params);
	DWORD OnInitObject(DWORD size, void *params);
	DWORD OnSerializeObject(DWORD size, void *params);
	/// Find an Object that was previous created
	/// \param size = sizeof(IHashString)
	/// \param params = IHashString * point to name
	DWORD OnFindObject(DWORD size, void *params);
	/// message to rename the object's name
	/// \param size - sizeof(RENAMEOBJNAME)
	/// \param *params - ptr to RENAMEOBJNAME struct
	DWORD OnRenameObject(DWORD size, void *params);

	/// message to get the component type of an object by name
	/// \param size - sizeof(GETCOMPONENTTYPEPARAMS)
	/// \param *params - ptr to GETCOMPONENTTYPEPARAMS struct
	DWORD OnGetComponentType(DWORD size, void *params);

	/// Generates a unique object name given a name to use as a root
	DWORD OnGenerateUniqueObjectName(DWORD size, void *params);

	/// Enables/Disables the system to query for duplicate objects. 
	/// It is a large performance hit but a good utility, it needs the ability to turn on and off.
	DWORD OnFlagDuplicateObjectChecking(DWORD size, void *params);

private:

	// cached toolbox access
	IToolBox *m_ToolBox;
	// component name
	CHashString m_ComponentType;
	// map of object ptrs keyed with their name uid's
	ObjectKeyMap_t m_ObjectKeyMap;
	// pointer to the delayed deleteion listener
	IListener *m_DelayedDeleteListener;
	// list of ids for delayed deletion
	UniqueIDSet_t m_DelayedDeleteList;
	// Map of base names to their last ID used
	map<DWORD, int> m_GeneratedNameMap;

	bool m_bDuplicateObjectCheck;
};

#endif