///==========================================================================
/// \file	IPhysicsCloth.h
/// \brief	Physics object for cloth
/// \date	27/02/2009
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

#ifndef _PHYSICSCLOTH_H
#define _IPHYSICSCLOTH_H

namespace ElementalEngine
{

///==========================================================================
/// \class	IPhysicsCloth
/// \brief	Physics object for cloth
///==========================================================================
class IPhysicsCloth
{

public:
	/// Destructor
	~IPhysicsCloth() {};

	/// Get the name.
	virtual IHashString* GetName() = 0;

	/// Set the geometry the cloth is powering.
	virtual void SetGeometry( IGeometry* geom ) = 0;
	
	// Set the pose of cloth. (After its initial creation)
	/// \param pose = the matrix of pose of cloth
	//virtual void SetPose( Matrix4x4& pose ) = 0;
};

}
#endif