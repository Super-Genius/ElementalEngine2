///==========================================================================
/// \file	IObject.h
/// \brief	Interface for Entity objects
/// \date	02/21/2005
/// \author	Kyle Swaim
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
///==========================================================================

#ifndef	IOBJECT_H
#define IOBJECT_H

#include "IComponent.hpp"

namespace ElementalEngine
{
class IHashString;
///==========================================================================
/// \class	IObject
/// \brief	Entity object interface
///==========================================================================
class IObject : public IComponent
{
public:
	virtual ~IObject() {};
	/// \fn		virtual void Init()
	/// \brief	Initialize the object
	virtual void Init() = 0;
	/// \fn		virtual void Update()
	/// \brief	Update the object
	virtual bool Update() = 0;
	/// \fn		virtual void DeInit()
	/// \brief	Deinitialize the object
	virtual void DeInit() = 0;
	/// Get the name for this object
	/// \return IHashString name for object
	virtual IHashString *GetName() = 0;
	/// Set the name for this object
	/// \param inName = IHashString name for object
	virtual void SetName(IHashString *inName) = 0;

	/// Get the parents name for this object
	/// \return IHashString name for parent
	virtual IHashString *GetParentName() = 0;
	/// Set the parent name for this object
	/// \param inName = IHashString name for parent
	virtual void SetParentName(IHashString *inName) = 0;
};

/// list of GUI components(elements) stored per page
typedef list<IObject *> OBJECTLIST;
/// set of IObjects
typedef set<IObject *> OBJECTSET;

/// map of name to an IComponent for quick access
typedef map<DWORD, IObject *> IDTOOBJECTMAP;

}	//namespace ElementalEngine

#endif	// IOBJECT_H