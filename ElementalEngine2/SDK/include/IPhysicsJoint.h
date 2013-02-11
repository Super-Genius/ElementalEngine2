///==========================================================================
/// \file	IPhysicsJoint.h
/// \brief	Interface for an joint
/// \date	02/27/2009
/// \author	Marivaldo Cabral
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

#ifndef	_IPHYSICSJOINT_H
#define _IPHYSICSJOINT_H

namespace ElementalEngine
{

///==========================================================================
/// \class	IPhysicsJoint
/// \brief	Physics joint interface
///==========================================================================
class IPhysicsJoint
{

public:

	/// Destructor
	virtual ~IPhysicsJoint() {}; 

	/// Get the name of the joint's parent.
	virtual IHashString* GetParent() = 0;

	/// Get the name of a joint's child.
	virtual IHashString* GetChild() = 0;

	/// Get the name of the joint.
	virtual IHashString* GetName() = 0;

	/// Set the joint to break under a specified force and torque.
	/// \param maxForce = the maximum force before the joint will break.
	/// \param maxTorque = the maximum torque before the joint will break.
	virtual void SetBreakable( float maxForce, float maxTorque ) = 0;
};

} // namespace ElementalEngine

#endif // _IPHYSICSJOINT_H