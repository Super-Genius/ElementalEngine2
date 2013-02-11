///==========================================================================
/// \file	IPhysicsManager.h
/// \brief	Physics interface of the CManager interface
/// \date	2-18-2005
/// \date	3-17-2009
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

#ifndef _IPHYSICSMANAGER_H
#define _IPHYSICSMANAGER_H

namespace ElementalEngine
{

///==========================================================================
/// \class	IPhysicsManager
/// \brief	Defines a Manager for IPhysObjects
///==========================================================================
class IPhysicsManager : public CManager
{
protected:
	IPhysicsManager(const TCHAR *compType) : CManager(compType, PHYSICSPRIORITY) {}

public:

	/// Default Destructor
	virtual ~IPhysicsManager() {};

	/// Initializes the Physics Manager and Physics system
	/// \return	True on sucess, False on failure
	virtual bool Init() = 0;

	/// Updates the Physics Manager and Physics system
	virtual void Update(DWORD tickCount) = 0;
	
	/// DeInitializes the Physics Manager and Physics system
	/// \return	True on sucess, False on failure
	virtual bool DeInit() = 0;

	/// Set the gravity
	/// \brief	The new gravity vector
	virtual void SetGravity( Vec3& gravity ) = 0;

	/// Get the gravity
	/// \return The gravity vector
	virtual Vec3* GetGravity() = 0;

	/// Set the physical world's scale
	/// \param	The scale in meters per Physycs units
	virtual void SetScale( float scale ) = 0;

	/// Get the physical world's scale
	/// \return The scale in meters per PhysX units
	virtual float GetScale() = 0;

	/// Test if two contact groups are set as a contact pair
	virtual bool IsContactPair( unsigned int groupA, unsigned int groupB ) = 0;

	// ------------------ Messages ------------------ //

	/// Start the physics manager.
	virtual DWORD OnStart( DWORD size, void* params ) = 0;

	/// Stop the physics manager.
	virtual DWORD OnStop( DWORD size, void* params ) = 0;

	/// Set a contact pair to trigger contact reports.
	virtual DWORD OnSetContactPair( DWORD size, void* params ) = 0;

	/// Set a collision pair to trigger collision events.
	virtual DWORD OnSetCollisionPair( DWORD size, void* params ) = 0;

	/// Gets the first object that intersects the specified ray.
	virtual DWORD OnRaycast( DWORD size, void* params ) = 0;

	/// Gets all objects that intersect the specified ray.
	virtual DWORD OnRaycastArchive( DWORD size, void* params ) = 0;

	/// Gets all objects that intersect the specified ray.
	virtual DWORD OnRaycastList( DWORD size, void* params ) = 0;

	/// Gets all objects that intersect the specified AABB.
	virtual DWORD OnIntersectAABB( DWORD size, void* params ) = 0;

	/// Gets all objects that intersect the specified OOBB.
	virtual DWORD OnIntersectOOBB( DWORD size, void* params ) = 0;

	/// Cache a physics object for fast loading
	virtual DWORD OnCachePhysicsObject( DWORD size, void* param ) = 0;

	/// Toggle physics visualization.
	virtual DWORD OnToggleRemoteVisualizer( DWORD size, void* param ) = 0;

	/// Shut down the physics system.
	virtual DWORD OnShutDownPhysics( DWORD size, void* params ) = 0;

	/// Set gravity
	virtual DWORD OnSetGravity( DWORD size, void* param ) = 0;

	/// Set the unit scale
	virtual DWORD OnSetPhysicsScale( DWORD size, void* param ) = 0;

	/// Forces actors to move to their specified location.
	virtual DWORD OnSetForceMoveActors( DWORD size, void* param ) = 0;

	/// Force all actors kinematic.
	virtual DWORD OnForceKinematic( DWORD size, void* param ) = 0;

};

} // ElementalEngine

#endif // _IPHYSICSMANAGER_H