///==========================================================================
/// \file	COctreeSceneManager.h
/// \brief	Manages octree scene managers
/// \date	4-16-2008
/// \author	Kyle Swaim
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

#ifndef COCTREESCENEMANAGER
#define COCTREESCENEMANAGER

#include "stdafx.h"

class COctreeScene;
class COctreeSceneNode;

///==========================================================================
/// \class	COctreeSceneManager
/// \brief	Defines a Manager for COctrees.
///==========================================================================
class COctreeSceneManager : public CManager
{
private:
	SINGLETONCONSTRUCTOROVERRIDE( COctreeSceneManager );

	/// Default Constructor
	///	Since COctreeSceneManager is implemented as a singleton the
	///	constructor is private.
	COctreeSceneManager();

public:
	/// Default Destructor
	virtual ~COctreeSceneManager();

	/// Function to get this component.
	/// \return	The singleton instance in this case
	static IComponent *Create( int nArgs, va_list argptr );

	/// Initializes the Physics Manager and Physics system
	/// \return	True on sucess, False on failure
	virtual bool Init();

	/// Updates the Physics Manager and Physics system
	virtual void Update(DWORD tickCount);
	
	/// DeInitializes the Physics Manager and Physics system
	/// \return	True on sucess, False on failure
	virtual bool DeInit();

	// ------------------ Messages ------------------ //

	/// Set the active scene.
	DWORD OnSetActiveScene( DWORD size, void* params );

	/// Visit the active scene
	DWORD OnVisitScene( DWORD size, void* params );

	/// Visit the active scene with the frustum visitor
	DWORD OnVisitSceneFrustum( DWORD size, void* params );

	/// Visit the active scene with the AABB visitor.
	DWORD OnVisitSceneAABB( DWORD size, void* params );

	/// Visit the active scene with the sphere visitor.
	DWORD OnVisitSceneSphere( DWORD size, void* params );

	/// Add a new scene object to the active scene.
	DWORD OnAddSceneObject( DWORD size, void* params );

	/// Remove a scene object from the active scene.
	/// depricated, scene objects now call this directly
	DWORD OnRemoveSceneObject( DWORD size, void* params );
	
	/// Clear all scene objects from the active scene.
	DWORD OnClearScene( DWORD size, void* params );

private:	
	CHashString m_hszCOctreeSceneType;

	CHashString m_hszActiveSceneName;
	COctreeScene* m_pActiveOctreeScene;
};

#endif	// COCTREESCENEMANAGER
