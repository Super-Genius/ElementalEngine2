///==========================================================================
/// \file	IComponent.hpp
/// \brief	Interface for components
/// \date	02/26/2005
/// \author	Kenneth Hurley
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

#ifndef	_ICOMPONENT_H
#define _ICOMPONENT_H

#include "IHashString.hpp"

namespace ElementalEngine
{
///==========================================================================
/// \class	IComponent
/// \brief	Component class to register in ToolBox
///	Right now this is just a wrapper class so things are components in the
///	Toolbox when they are registered and retrieved.
///==========================================================================
class IComponent
{
public:
	/// make sure derived destructors are called and dynamic_cast can be used
	virtual ~IComponent() {};

	/// Serialize this component to the archive.
	///	\param ar = reference to the archive class
	virtual void Serialize(IArchive &ar) = 0;

	/// return a string of the component type
	/// \return IHashString *reference to textual classname
	virtual IHashString *GetComponentType() = 0;

	/// Checks if this is similar to it's base classes component
	/// \return true if it is simlar, else false
	virtual bool IsKindOf(IHashString *compType) = 0;

};

/// the component create function prototype
typedef IComponent *COMPONENTCREATEFUNC(int nArgs, va_list argptr);
}	//namespace ElementalEngine


#endif	// _ICOMPONENT_H