///==========================================================================
/// \file	COCtreeScene.cpp
/// \brief	Implementation of an Octree Scene.
/// \date	4/16/2008
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

#ifndef COCTREESCENE
#define COCTREESCENE

#include "stdafx.h"

// This is a safety valve to prevent the octree from growing infinitly.
#define MAX_OCTREE_DEPTH 100
#define NUM_CHILDREN 8
#define NUM_NEIGHBORS 6
#define NULL_NODE = -1

struct NODEOBJECTINFO
{
	// pointer to the object
	ISceneObject *object;
	// pointer to the node that the object is currently associated with
	COctreeSceneNode *node;
};

enum NODEID
{
	FRONT_UPPER_LEFT = 0,
	FRONT_UPPER_RIGHT = 1,
	FRONT_LOWER_LEFT = 2,
	FRONT_LOWER_RIGHT = 3,
	BACK_LOWER_RIGHT = 4,
	BACK_LOWER_LEFT = 5,
	BACK_UPPER_RIGHT = 6,
	BACK_UPPER_LEFT = 7
};

typedef map< DWORD, NODEOBJECTINFO > NODEOBJECTMAP;
typedef list<COctreeSceneNode*> NODEPOOL;

///==========================================================================
/// \class	COctreeScene
/// \brief	A Loose Octree. The octree can grow dynamicly both larger and
/// smaller. 
///==========================================================================
class COctreeScene : public CObjectTemplate<IScene>
{
protected:
	/// Constructor
	/// \param	parentName = name of parent in hierarchy
	/// \param	name = IHashString * name of this object
	COctreeScene( IHashString *parentName, IHashString *name );

public:
	/// Destructor
	virtual ~COctreeScene();

	/// The standard static component create function
	static IComponent* Create( int nArgs, va_list argptr );

	/// Initialize the object
	virtual void Init();

	/// Deinitializes the CPhysicsObject and deallocates
	///	any allocated memory. Resets all internal data.
	virtual void DeInit();

	/// Visit each node in the Octree.
	/// \param visitor = A visitor
	/// \param sceneObjectList = A list of ISceneObject pointers.
	/// \param sortID = The type of object to add to the list.
	void Visit( IVisitor* visitor, DWORD sortID );

	/// Visit each node within a frustum.
	/// \param visitor = A visitor
	/// \paran frustum = A frustum
	/// \param sceneObjectList = A list of ISceneObject pointers.
	/// \param sortID = The type of object to add to the list.
	void VisitFrustum( IVisitor* visitor, CFrustum* frustum, DWORD sortID );

	/// Visit each node within an axis aligned bounding box.
	/// \param visitor = A visitor
	/// \paran aabb = An axis aligned bounding box
	/// \param sceneObjectList = A list of ISceneObject pointers.
	/// \param sortID = The type of object to add to the list.
	void VisitAABB( IVisitor* visitor, AABB* aabb, DWORD sortID );

	/// Visit each node within a sphere.
	/// \param visitor = A visitor
	/// \paran position = A sphere's center
	/// \param radius = A sphere's radius
	/// \param sceneObjectList = A list of ISceneObject pointers.
	/// \param sortID = The type of object to add to the list.
	void VisitSphere( IVisitor* visitor, Vec3* position, float radius, DWORD sortID );

	// Add an ISceneObject to the Octree
	/// \param object = A scene object.
	void AddSceneObject( ISceneObject* object );
	// Add an ISceneObject to the Octree's list of global objects
	void AddGlobalSceneObject( ISceneObject* object );
	// Remove an ISceneObject from the Octree's list of global objects
	void RemoveGlobalObject( ISceneObject* object );

	/// Update a scene object in the octree.
	/// \param pObject = A pointer to the scene object to update.
	virtual void UpdateObject( ISceneObject *pObject );

	/// Clears the scene and cleans up all COctreeSceneNodes.
	void ClearScene();

	/// Set the root octnode of the octree.
	/// \node = The new root node.
	inline void SetRootNode( COctreeSceneNode* node ) { m_pRootNode = node; }

private:
	/// Helper to visit all global objects.  Called by all other Visit functions.
	/// \param visitor = The visitor.
	/// \param sortID = The type of object to visit.
	void VisitGlobalObjects( IVisitor* visitor, DWORD sortID );

	// root node of the octree
	COctreeSceneNode* m_pRootNode;

	// set of global objects
	struct LessISceneObject : public binary_function <ISceneObject *, ISceneObject *, bool> 
	{
		bool operator()( ISceneObject *_Left, ISceneObject *_Right ) const
		{
			return _Left->GetName()->GetUniqueID() < _Right->GetName()->GetUniqueID();
		}
	};
	typedef set<ISceneObject*,LessISceneObject> GLOBALOBJECTSET;
	typedef map<DWORD,GLOBALOBJECTSET> GLOBALOBJECTSETMAP;
	GLOBALOBJECTSETMAP m_GlobalObjectSetMap;
};

#endif // COCTREESCENE