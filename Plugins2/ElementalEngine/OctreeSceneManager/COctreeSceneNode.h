///==========================================================================
/// \file	COCtreeSceneNode.cpp
/// \brief	Implementation of an Octree Scene Node.
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

#ifndef COCTREESCENENODE
#define COCTREESCENENODE

#include "stdafx.h"

#define OCTREENODE_TARGET_DENSITY	10
#define LOOSE_FACTOR				2

///==========================================================================
/// \class	COctreeSceneNode
/// \brief	Loose Octree Node
///==========================================================================
class COctreeSceneNode : public CObjectTemplate<ISceneNode>
{
public:
	/// Default Constructor
	/// \param owner = The Octree this octnode belongs to.
	/// \param parent = The parent octnode.
	/// \param center = The center of the octnode.
	/// \param dimensions = The length of the sides of the octnode.
	/// \param depth = The depth of the node relative to the starting octnode.
	COctreeSceneNode( COctreeScene* owner, COctreeSceneNode* parent, 
		Vec3& center, float dimensions, int depth );

	/// Construct as a parent of another node. This is used to grow the octree.
	/// \param owner = The Octree this octnode belongs to.
	/// \param child = The child octnode that is creating its parent octnode.
	/// \param childID = Which octant the child is in.
	/// \param center = The center of the octnode.
	/// \param dimensions = The length of the sides of the octnode.
	/// \param depth = The depth of the node relative to the starting octnode.
	COctreeSceneNode( COctreeScene* owner, COctreeSceneNode* child, NODEID childID,
		Vec3& center, float dimensions, int depth );

	/// Destructor
	~COctreeSceneNode();

	/// Create the a instance of IObject
	/// \param nArgs = The number of arguements.
	/// \param va_list = List of agruements.
	/// \return	IComponent pointer
	static IComponent *Create(int nArgs, va_list argptr);
	
	/// Initalize the node
	void Init();

	/// Deinitialize the node
	void DeInit();

	ISceneNode* UpdateSceneObject( ISceneObject* object );

	// Update an ISceneObject, adds object if not currently in scene.
	/// \param object = A scene object.
	/// \return The octnode the object was added to.
	COctreeSceneNode* UpdateSceneObject( ISceneObject* object, Vec3* position, float radius );

	/// Remove an ISceneObject.
	/// \param object = A scene object.
	/// \return True on successful removal of the object, otherwise false.
	bool RemoveObject( ISceneObject* object );

	/// Visit each node in the Octree.
	/// \param visitor = A visitor
	/// \param sortID = The sortID of the desired objects.
	void Visit( IVisitor* visitor, DWORD sortID );

	/// Visit each node within a frustum.
	/// \param visitor = A visitor
	/// \paran frustum = A frustum
	/// \param sortID = The sortID of the desired objects.
	/// \param parentResult = The result of the parent's intersection test.
	void VisitFrustum( IVisitor* visitor, CFrustum* frustum, DWORD sortID, INTERSECTION_RESULT parentResult=INTERSECTION_PARTIAL );

	/// Visit each node within an axis aligned bounding box.
	/// \param visitor = A visitor
	/// \paran aabb = An axis aligned bounding box
	/// \param sortID = The sortID of the desired objects.
	/// \param parentResult = The result of the parent's intersection test.
	void VisitAABB( IVisitor* visitor, AABB* aabb, DWORD sortID, INTERSECTION_RESULT parentResult=INTERSECTION_PARTIAL );

	/// Visit each node within a sphere.
	/// \param visitor = A visitor
	/// \paran position = A sphere's center
	/// \param radius = A sphere's radius
	/// \param sortID = The sortID of the desired objects.
	/// \param parentResult = The result of the parent's intersection test.
	void VisitSphere( IVisitor* visitor, Vec3* position, float radius, DWORD sortID, INTERSECTION_RESULT parentResult=INTERSECTION_PARTIAL );

	/// Gets the list of objects attached to the scene node.
	/// \param sortID = The sortID of the desired objects.
	/// \return The list of objects with the specified sortID.
	virtual IDSCENEOBJECTMAP* GetObjSet( DWORD sortID );

	/// Get the scene node's AABB
	/// \param min = The scene node's AABB min.
	/// \param max = The scene node's AABB max.
	virtual void GetBoundingAABB( Vec3& min, Vec3& max );

	/// Get the scene node's bounding sphere
	/// \param position = The scene node's bounding sphere position.
	/// \param radius = The scene node's bounding sphere radius.
	virtual void GetBoundingSphere( Vec3& position, float& radius );

	/// Get the scene node's parent scene node.
	/// \return The scene node's parent scene node.
	virtual ISceneNode* GetParent();

	/// Get the child scene node at the specified index.
	/// \param index = The index of the desired child.
	/// \return The child scene node at the specified index.
	virtual ISceneNode* GetChild( int index );

	/// Get the unique ID
	/// \return The unique ID.
	virtual DWORD GetUniqueID() const { return m_iUniqueID; }

private:
	/// Test if an sphere fits completely inside a octnode.
	/// \param spherePos = The center of a sphere.
	/// \param sphereRad = The radius of a sphere.
	/// \param nodePos = The center of an octnode.
	/// \param nodeDim = The dimensions of an octnode.
	/// \return True if the octnode encomapses the sphere, otherwise false.
	bool FitsInNode( Vec3& spherePos, float sphereRad, Vec3& nodePos, float nodeDim );

	/// Classify which octant an object is positioned in.
	/// \param position = The position of an object in world space.
	/// \return The NODEID of the octant the position is closest to.
	int ClassifyObject( Vec3& position );

	/// Get the opposite NODEID
	/// \param node = A NODEID.
	/// \return The opposite NODEID.
	NODEID GetOppositeNode( NODEID node );

	/// Make sure a child is allocated at octant.
	/// \iChildOctant = index of the child octant.
	void NewChild( int iChildOctant );

	/// Grow a new parent octnode. This octnode becomes
	/// a child of the new octnode.
	/// \param spherePos = The position of the object to insert
	///	into the new parent.
	/// \return A pointer to the new parent octant.
	COctreeSceneNode* GrowOctnode( Vec3& spherePos );

	/// Insert an ISceneObject into a child octant
	/// \param object = An ISceneObject pointer.
	/// \param position = The position of the object.
	/// \param radius = the bounding radius of the object.
	/// \return A pointer to the octant the object was inserted into.
	COctreeSceneNode* InsertObjectIntoChild( ISceneObject* object, Vec3* position, float radius );

	int GetChildCount();
	void DeleteIfEmpty();
	void DeleteChild( DWORD iChildUniqueID );

	COctreeScene* m_pOwner;
	COctreeSceneNode* m_pParentNode;

	Vec3 m_Position;
	float m_Dimensions;
	float m_LooseDimensions;
	int m_Depth;
	Vec3 m_BoundingMins;
	Vec3 m_BoundingMaxs;
	Vec3 m_LooseBoundingMins;
	Vec3 m_LooseBoundingMaxs;
	float m_fBoundingRadius;
	float m_fLooseBoundingRadius;

	COctreeSceneNode* m_Children[NUM_CHILDREN];
	int m_NeighborIndex[NUM_NEIGHBORS];

	SORTIDOBJECTMAP m_Objects;
	int m_iObjectCount;

	static const Vec3 m_OctantSignes[NUM_CHILDREN];

	/// The last direction the octree grew in. This is to keep the
	/// octree growing evenly.
	NODEID m_LastGrowth;

	/// Unique identifier to allow fast comparison of quadnode's.
	DWORD m_iUniqueID;
	/// Static to generate unique identifiers;
	static DWORD s_iGenUniqueID;

	bool m_MaxCapacity;
};

#endif // COCTREESCENENODE