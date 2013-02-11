///============================================================================
/// \file	IPhysObject.h
/// \brief	Physics Object interface
/// \date	9-14-2005
/// \author	Kyle Swaim
/// \note	Copyright (c) 2005 Signature Devices, Inc.
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

#ifndef	_IPHYSOBJECT_H
#define	_IPHYSOBJECT_H

#include "IObject.h"

namespace ElementalEngine
{

class IPhysObject : public IObject
{
public:
	virtual ~IPhysObject() {};
	/// \brief	Raises a collision event when this object is collided with
	/// \brief	objCollision = pointer to the colliding object
	//virtual void OnCollide( IHashString *objCollision ) = 0;
};

}		// namespace ElementalEngine

#endif	// _IPHYSOBJECT_H
