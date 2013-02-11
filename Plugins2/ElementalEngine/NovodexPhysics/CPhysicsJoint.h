///==========================================================================
/// \file	CPhysicsJoint.h
/// \brief	Wrapper for an NxJoint
/// \date	5/08/2007
/// \date	3/23/2009
/// \author	Kyle Swaim
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

#ifndef	CPHYSICSJOINT_H
#define CPHYSICSJOINT_H

namespace ElementalEngine
{

class CPhysicsJoint;

typedef list<CPhysicsJoint*> JOINTLIST;

class CPhysicsJoint : public IPhysicsJoint
{
public:
	IHashString* m_Name;
	NxJoint* m_Joint;

private:
	/// Constructor
	CPhysicsJoint();
public:
	/// Construct from an NxJoint
	CPhysicsJoint( NxJoint* joint );

	/// Destructor
	~CPhysicsJoint();

	/// Get the name of the joint's parent.
	IHashString* GetParent();

	/// Get the name of a joint's child.
	IHashString* GetChild();

	/// Get the name of the joint.
	IHashString* GetName();

	/// Set the joint to break under a specified force and torque.
	/// \param maxForce = the maximum force before the joint will break.
	/// \param maxTorque = the maximum torque before the joint will break.
	void SetBreakable( float maxForce, float maxTorque );

	/// Get actor PhysX implementation
	NxJoint* GetNxJoint();

};

} // namespace ElementalEngine

#endif // CPHYSICSJOINT_H