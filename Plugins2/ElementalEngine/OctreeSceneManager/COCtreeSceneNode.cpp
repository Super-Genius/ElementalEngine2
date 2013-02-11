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

#include "StdAfx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

const Vec3 COctreeSceneNode::m_OctantSignes[NUM_CHILDREN] =
{
	Vec3(-1,  1, -1),	// LEFT_UPPER_FRONT
	Vec3( 1,  1, -1),	// RIGHT_UPPER_FRONT
	Vec3(-1, -1, -1),	// LEFT_LOWER_FRONT
	Vec3( 1, -1, -1),	// RIGHT_LOWER_FRONT
	Vec3( 1, -1,  1),	// RIGHT_LOWER_BACK
	Vec3(-1, -1,  1),	// LEFT_LOWER_BACK
	Vec3( 1,  1,  1),	// RIGHT_UPPER_BACK
	Vec3(-1,  1,  1)	// LEFT_UPPER_BACK
};

DWORD COctreeSceneNode::s_iGenUniqueID = 0;

REGISTER_COMPONENT( COctreeSceneNode );

COctreeSceneNode::COctreeSceneNode( COctreeScene* owner, COctreeSceneNode* parent, Vec3& center, float dimensions, int depth ) :
	OBJECTTEMPLATE( \0, COctreeSceneNode, ISceneNode, NULL, NULL )
{
	m_iUniqueID = s_iGenUniqueID++;

	m_pOwner = owner;
	m_pParentNode = parent;

	m_Position = center;
	m_Dimensions = dimensions;
	m_LooseDimensions = dimensions * LOOSE_FACTOR;
	m_Depth = depth;

	Init();

	memset( m_Children, 0, sizeof(m_Children) );

	m_LastGrowth = FRONT_UPPER_LEFT;

	m_iObjectCount = 0;
}

COctreeSceneNode::COctreeSceneNode( COctreeScene* owner, COctreeSceneNode* child, NODEID childID, Vec3& center, float dimensions, int depth ) :
	OBJECTTEMPLATE( \0, COctreeSceneNode, ISceneNode, NULL, NULL )
{
	m_iUniqueID = s_iGenUniqueID++;

	m_pOwner = owner;
	m_pParentNode = NULL;

	m_Position = center;
	m_Dimensions = dimensions;
	m_LooseDimensions = dimensions * LOOSE_FACTOR;
	m_Depth = depth;

	Init();

	// Set this as the root octnode.
	m_pOwner->SetRootNode( this );

	memset( m_Children, 0, sizeof(m_Children) );

	m_Children[childID] = child;
}

COctreeSceneNode::~COctreeSceneNode()
{
	DeInit();
}

IComponent *COctreeSceneNode::Create(int nArgs, va_list argptr)
{
	// No creating this class...
	return NULL;
}

void COctreeSceneNode::Init()
{
	m_MaxCapacity = false;

	float fHalfDim = m_Dimensions * 0.5f;
	Vec3 vHalfDim( fHalfDim, fHalfDim, fHalfDim );
	Vec3 vDim( m_Dimensions, m_Dimensions, m_Dimensions );
	m_BoundingMins = m_Position - vHalfDim;
	m_BoundingMaxs = m_Position + vHalfDim;
	m_LooseBoundingMins = m_Position - vDim;
	m_LooseBoundingMaxs = m_Position + vDim;
	m_fBoundingRadius = sqrtf( fHalfDim*fHalfDim * 3.0f );
	m_fLooseBoundingRadius = sqrtf( m_LooseDimensions*m_LooseDimensions * 3.0f );
}

void COctreeSceneNode::DeInit()
{
	// Clear all ISceneObjects
	m_Objects.clear();
	m_iObjectCount = 0;

	// clear children
	for( int i=0; i<NUM_CHILDREN; i++)
	{
		if( m_Children[i] != NULL )
		{
			// clean the child
			SAFE_DELETE( m_Children[i] );
		}
	}

	memset( m_Children, 0, sizeof(m_Children) );
}

ISceneNode* COctreeSceneNode::UpdateSceneObject( ISceneObject* object )
{
	Vec3 objPosition;
	float objRadius;
	object->GetBoundingSphere( objPosition, objRadius );

	return UpdateSceneObject( object, &objPosition, objRadius );
}

COctreeSceneNode* COctreeSceneNode::UpdateSceneObject( ISceneObject* object, Vec3* position, float radius )
{
	DWORD sortID = object->GetSceneSortID();

	// Check if the object fits inside this node
	if( FitsInNode( *position, radius, m_Position, m_LooseDimensions ) )
	{
		// early out if the object fits and is already a member of this scene node
		if (object->GetSceneNode() && object->GetSceneNode()->GetUniqueID() == GetUniqueID())
		{
			return this;
		}

		IDSCENEOBJECTMAP* objects = GetObjSet( sortID );

		if( objects->size() >= OCTREENODE_TARGET_DENSITY && !m_MaxCapacity )
		{
			m_MaxCapacity = true;

			// reinsert old objects into child nodes
			SORTIDOBJECTMAP::iterator sortIter = m_Objects.begin();
			for( ; sortIter != m_Objects.end(); ++sortIter )
			{
				// remove object from this node
				IDSCENEOBJECTMAP sceneObjectMap = sortIter->second;

				IDSCENEOBJECTMAP::iterator objectIter = sceneObjectMap.begin();
				for( ; objectIter != sceneObjectMap.end(); ++objectIter )
				{
					ISceneObject* sceneObject = objectIter->second;

					Vec3 sceneObjecPos;
					float sceneObjectRad;
					sceneObject->GetBoundingSphere( sceneObjecPos, sceneObjectRad );

					COctreeSceneNode* newNode = InsertObjectIntoChild( sceneObject, &sceneObjecPos, sceneObjectRad );
					if( newNode )
					{
						RemoveObject( sceneObject );
						sceneObject->SetSceneNode( m_pOwner, newNode );
					}
				}
			}
		}

		// if we have more than OCTREENODE_TARGET_DENSITY objects in this node so attempt to add the object
		// to a child octnode.
		if( m_MaxCapacity )
		{
			COctreeSceneNode* childInsertedInto = InsertObjectIntoChild( object, position, radius );
			if( childInsertedInto )
			{
				return childInsertedInto;
			}
		}

		// add object to this node
		DWORD objectID = object->GetName()->GetUniqueID();
		m_Objects[sortID][objectID] = object;
		m_iObjectCount++;
		return this;
	}
	// The object is too big for the node so we have to grow a new parent octnode
	else
	{
		if (m_pParentNode)
		{
			return m_pParentNode->UpdateSceneObject( object, position, radius );
		}
		else
		{
			COctreeSceneNode* newParent = GrowOctnode( *position );
			return newParent->UpdateSceneObject( object, position, radius );
		}
	}
}

bool COctreeSceneNode::RemoveObject( ISceneObject* object )
{
	// remove object from list of objects in node
	DWORD sortID = object->GetSceneSortID();
	IDSCENEOBJECTMAP* objects = GetObjSet( sortID );
	if( objects != NULL )
	{
		DWORD objNameID = object->GetName()->GetUniqueID();

		size_t iObjectsErased = objects->erase( objNameID );
		// make sure object was attached to the correct node
		assert( iObjectsErased > 0 );
		object->SetSceneNode( NULL, false );
		m_iObjectCount--;

		DeleteIfEmpty();

		return true;
	}

	return false;
}

void COctreeSceneNode::Visit( IVisitor* visitor, DWORD sortID )
{
	// add objects of the sortID type to the list
	IDSCENEOBJECTMAP* nodeObjectList = GetObjSet( sortID );
	IDSCENEOBJECTMAP::iterator iter;
	for( iter = nodeObjectList->begin(); iter != nodeObjectList->end(); iter++ )
	{
		ISceneObject* curObject = iter->second;
		visitor->Visit( curObject, true );
	}

	// Visit child nodes
	for( int i=0; i<8; i++)
	{
		if (m_Children[i])
				m_Children[i]->Visit( visitor, sortID );
	}
}

void COctreeSceneNode::VisitFrustum( IVisitor* visitor, CFrustum* frustum, DWORD sortID, INTERSECTION_RESULT parentResult )
{
	INTERSECTION_RESULT result;
	// if parent intersection was partial, this node's intersection is unknown
	if (parentResult == INTERSECTION_PARTIAL)
	{
		// test this node's bounding sphere
		int iPlaneBits = 0;
		bool bSphereIntersectionResult = frustum->IsSphereInView( m_Position, m_fLooseBoundingRadius, iPlaneBits );
		if (bSphereIntersectionResult)
		{
			// if bounding sphere passes, test this node's bounding box
			result = frustum->IsAABBInView( m_LooseBoundingMins, m_LooseBoundingMaxs, iPlaneBits );
		}
		else
		{
			result = INTERSECTION_OUTSIDE;
		}
	}
	else
	{
		// if parent intersection wasn't partial, this node must have the same intersection as the parent
		result = parentResult;
	}

	// If the node is in the frustum then visit it.
	if( result != INTERSECTION_OUTSIDE )
	{
		IDSCENEOBJECTMAP* nodeObjectList = GetObjSet( sortID );
		IDSCENEOBJECTMAP::iterator iter;
		if (result == INTERSECTION_PARTIAL)
		{
			// Test if each object is within the Frustum
			for( iter = nodeObjectList->begin(); iter != nodeObjectList->end(); iter++ )
			{
				ISceneObject* curObject = iter->second;
				
				// Get object's bounding sphere
				Vec3 objPosition;
				float objRadius;
				curObject->GetBoundingSphere( objPosition, objRadius );

				// If object is in view then visit it.
				if( frustum->IsSphereInView( objPosition, objRadius ) )
				{
					visitor->Visit( curObject, true );
				}
			}
		}
		else if (result == INTERSECTION_INSIDE)
		{
			// assume all objects are visible
			for( iter = nodeObjectList->begin(); iter != nodeObjectList->end(); iter++ )
			{
				ISceneObject* curObject = iter->second;
				visitor->Visit( curObject, true );
			}
		}

		// Visit child nodes
		for( int i=0; i<8; i++)
		{
			if (m_Children[i])
				m_Children[i]->VisitFrustum( visitor, frustum, sortID, result );
		}
	}
}

void COctreeSceneNode::VisitAABB( IVisitor* visitor, AABB* aabb, DWORD sortID, INTERSECTION_RESULT parentResult )
{
	INTERSECTION_RESULT result;
	// if parent intersection was partial, this node's intersection is unknown
	if (parentResult == INTERSECTION_PARTIAL)
	{
		// TODO: update aabb-aabb test to return INTERSECTION_RESULT instead of a bool
		// and make use of the three possible intersection results.
		AABB nodeAABB( m_BoundingMins, m_BoundingMaxs );
		if (nodeAABB.IntersectAABB( aabb->m_Bounds[0], aabb->m_Bounds[1] ))
			result = INTERSECTION_PARTIAL;
		else
			result = INTERSECTION_OUTSIDE;
	}
	else
	{
		// if parent intersection wasn't partial, this node must have the same intersection as the parent
		result = parentResult;
	}

	// If the node is in the AABB then visit it.
	if( result != INTERSECTION_OUTSIDE )
	{
		IDSCENEOBJECTMAP* nodeObjectList = GetObjSet( sortID );
		IDSCENEOBJECTMAP::iterator iter;
		if (result == INTERSECTION_PARTIAL)
		{
			for( iter = nodeObjectList->begin(); iter != nodeObjectList->end(); iter++ )
			{
				ISceneObject* curObject = iter->second;
				
				// Get object's bounding sphere
				Vec3 objPosition;
				float objRadius;
				curObject->GetBoundingSphere( objPosition, objRadius );

				// If object is in view then visit it.
				if( aabb->IntersectSphere( objPosition, objRadius ) )
				{
					visitor->Visit( curObject, true );
				}
			}
		}
		else if (result == INTERSECTION_INSIDE)
		{
			// assume all objects are visible
			for( iter = nodeObjectList->begin(); iter != nodeObjectList->end(); iter++ )
			{
				ISceneObject* curObject = iter->second;
				visitor->Visit( curObject, true );
			}
		}

		// Visit child nodes
		for( int i=0; i<8; i++)
		{
			if (m_Children[i])
				m_Children[i]->VisitAABB( visitor, aabb, sortID, result );
		}
	}
}

void COctreeSceneNode::VisitSphere( IVisitor* visitor, Vec3* position, float radius, DWORD sortID, INTERSECTION_RESULT parentResult )
{
	INTERSECTION_RESULT result;
	// if parent intersection was partial, this node's intersection is unknown
	if (parentResult == INTERSECTION_PARTIAL)
	{
		// TODO: update aabb-sphere test to return INTERSECTION_RESULT instead of a bool
		// and make use of the three possible intersection results.
		AABB nodeAABB( m_BoundingMins, m_BoundingMaxs );
		if (nodeAABB.IntersectSphere( *position, radius ))
			result = INTERSECTION_PARTIAL;
		else
			result = INTERSECTION_OUTSIDE;
	}
	else
	{
		// if parent intersection wasn't partial, this node must have the same intersection as the parent
		result = parentResult;
	}

	// If the node is in the sphere then visit it.
	if( result != INTERSECTION_OUTSIDE )
	{
		// Test if each object is within the AABB
		IDSCENEOBJECTMAP* nodeObjectList = GetObjSet( sortID );
		IDSCENEOBJECTMAP::iterator iter;
		if (result == INTERSECTION_PARTIAL)
		{
			for( iter = nodeObjectList->begin(); iter != nodeObjectList->end(); iter++ )
			{
				ISceneObject* curObject = iter->second;
				
				// Get object's bounding sphere
				Vec3 objPosition;
				float objRadius;
				curObject->GetBoundingSphere( objPosition, objRadius );

				// If object is in view then visit it.
				// If the light bounding sphere and object bounding sphere intersect
				float xDist = position->x - objPosition.x;
				float yDist = position->y - objPosition.y;
				float zDist = position->z - objPosition.z;
				float maxDist = radius + objRadius;
				float sqDist = ( (xDist * xDist) + (yDist * yDist) + (zDist * zDist) );

				if( (maxDist * maxDist) >= sqDist )
				{
					visitor->Visit( curObject, true );
				}
			}
		}
		else if (result == INTERSECTION_INSIDE)
		{
			// assume all objects are visible
			for( iter = nodeObjectList->begin(); iter != nodeObjectList->end(); iter++ )
			{
				ISceneObject* curObject = iter->second;
				visitor->Visit( curObject, true );
			}
		}

		// Visit child nodes
		for( int i=0; i<8; i++)
		{
			if (m_Children[i])
				m_Children[i]->VisitSphere( visitor, position, radius, sortID, result );
		}
	}
}

IDSCENEOBJECTMAP* COctreeSceneNode::GetObjSet( DWORD sortID )
{
	return &m_Objects[sortID];
}

void COctreeSceneNode::GetBoundingAABB( Vec3& min, Vec3& max )
{
	min = m_BoundingMins;
	max = m_BoundingMaxs;
}

void COctreeSceneNode::GetBoundingSphere( Vec3& position, float& radius )
{
	position = m_Position;
	radius = m_fBoundingRadius;
}

ISceneNode* COctreeSceneNode::GetParent()
{
	return (ISceneNode*)m_pParentNode;
}

ISceneNode* COctreeSceneNode::GetChild( int index )
{
	return (ISceneNode*)m_Children[index];
}

bool COctreeSceneNode::FitsInNode( Vec3& spherePos, float sphereRad, Vec3& nodePos, float nodeDim )
{
	float halfNodeDim = nodeDim * 0.5f;

	if (spherePos.x - sphereRad < nodePos.x - halfNodeDim ||
	    spherePos.x + sphereRad > nodePos.x + halfNodeDim ||
		spherePos.y - sphereRad < nodePos.y - halfNodeDim ||
	    spherePos.y + sphereRad > nodePos.y + halfNodeDim ||
	    spherePos.z - sphereRad < nodePos.z - halfNodeDim ||
	    spherePos.z + sphereRad > nodePos.z + halfNodeDim)
	{
		return false;
	}
	else
	{
		return true;
	}
}

int COctreeSceneNode::ClassifyObject( Vec3& position )
{
	NODEID childID;

	// Child is in the FRONT_LOWER_LEFT of the new parent octnode.
	if( position.x < m_Position.x &&
		position.y < m_Position.y &&
		position.z < m_Position.z )
	{
		childID = FRONT_LOWER_LEFT;
	}
	// Child is in the BACK_LOWER_LEFT of the new parent octnode.
	else if( position.x < m_Position.x &&
		position.y < m_Position.y &&
		position.z > m_Position.z )
	{
		childID = BACK_LOWER_LEFT;
	}
	// Child is in the FRONT_LOWER_RIGHT of the new parent octnode.
	else if( position.x > m_Position.x &&
		position.y < m_Position.y &&
		position.z < m_Position.z )
	{
		childID = FRONT_LOWER_RIGHT;
	}
	// Child is in the BACK_LOWER_RIGHT of the new parent octnode.
	else if( position.x > m_Position.x &&
		position.y < m_Position.y &&
		position.z > m_Position.z )
	{
		childID = BACK_LOWER_RIGHT;
	}
	// Child is in the BACK_LOWER_RIGHT of the new parent octnode.
	else if( position.x < m_Position.x &&
		position.y > m_Position.y &&
		position.z < m_Position.z )
	{
		childID = FRONT_UPPER_LEFT;
	}
	// Child is in the BACK_UPPER_LEFT of the new parent octnode.
	else if( position.x < m_Position.x &&
		position.y > m_Position.y &&
		position.z > m_Position.z )
	{
		childID = BACK_UPPER_LEFT;
	}
	// Child is in the FRONT_UPPER_RIGHT of the new parent octnode.
	else if( position.x > m_Position.x &&
		position.y > m_Position.y &&
		position.z < m_Position.z )
	{
		childID = FRONT_UPPER_RIGHT;
	}
	// Child is in the BACK_UPPER_RIGHT of the new parent octnode.
	else if( position.x > m_Position.x &&
		position.y > m_Position.y &&
		position.z > m_Position.z )
	{
		childID = BACK_UPPER_RIGHT;
	}
	else
	{
		return -1;
	}

	return childID;
}

NODEID COctreeSceneNode::GetOppositeNode( NODEID node )
{
	int oppositeNode = node + 4;
	if( oppositeNode >= 8 )
	{
		oppositeNode -= 8;
	}
	return (NODEID)oppositeNode;
}

void COctreeSceneNode::NewChild( int iChildOctant )
{
	// if child already exists
	if (m_Children[iChildOctant])
		return;

	int newDepth = m_Depth + 1;
	float halfDim = m_Dimensions * 0.5f;
	float quarterDim = m_Dimensions * 0.25f;

	Vec3 newCenter( quarterDim, quarterDim, quarterDim );
	newCenter *= m_OctantSignes[iChildOctant];
	newCenter += m_Position;

	m_Children[iChildOctant] = new COctreeSceneNode( m_pOwner, this, newCenter, halfDim, newDepth );
}

COctreeSceneNode* COctreeSceneNode::GrowOctnode( Vec3& spherePos )
{
	// pick a direction to grow the octnode
	NODEID childID;

	// If the object is on any of the boundaries then grow in the opposite direction
	// of the last node growth.
	if( spherePos.x == m_Position.x ||
		spherePos.y == m_Position.y ||
		spherePos.z == m_Position.z )
	{
		childID = GetOppositeNode( m_LastGrowth );
	}

	// Child is in the BACK_UPPER_RIGHT of the new parent octnode.
	if( spherePos.x < m_Position.x &&
		spherePos.y < m_Position.y &&
		spherePos.z < m_Position.z )
	{
		childID = BACK_UPPER_RIGHT;
	}
	// Child is in the FRONT_UPPER_RIGHT of the new parent octnode.
	if( spherePos.x < m_Position.x &&
		spherePos.y < m_Position.y &&
		spherePos.z > m_Position.z )
	{
		childID = FRONT_UPPER_RIGHT;
	}
	// Child is in the BACK_UPPER_LEFT of the new parent octnode.
	if( spherePos.x > m_Position.x &&
		spherePos.y < m_Position.y &&
		spherePos.z < m_Position.z )
	{
		childID = BACK_UPPER_LEFT;
	}
	// Child is in the FRONT_UPPER_LEFT of the new parent octnode.
	if( spherePos.x > m_Position.x &&
		spherePos.y < m_Position.y &&
		spherePos.z > m_Position.z )
	{
		childID = FRONT_UPPER_LEFT;
	}
	// Child is in the BACK_LOWER_RIGHT of the new parent octnode.
	if( spherePos.x < m_Position.x &&
		spherePos.y > m_Position.y &&
		spherePos.z < m_Position.z )
	{
		childID = BACK_LOWER_RIGHT;
	}
	// Child is in the FRONT_LOWER_RIGHT of the new parent octnode.
	if( spherePos.x < m_Position.x &&
		spherePos.y > m_Position.y &&
		spherePos.z > m_Position.z )
	{
		childID = FRONT_LOWER_RIGHT;
	}
	// Child is in the BACK_LOWER_LEFT of the new parent octnode.
	if( spherePos.x > m_Position.x &&
		spherePos.y > m_Position.y &&
		spherePos.z < m_Position.z )
	{
		childID = BACK_LOWER_LEFT;
	}
	// Child is in the FRONT_LOWER_LEFT of the new parent octnode.
	if( spherePos.x > m_Position.x &&
		spherePos.y > m_Position.y &&
		spherePos.z > m_Position.z )
	{
		childID = FRONT_LOWER_LEFT;
	}

	float doubleDim = m_Dimensions * 2.0f;
	float halfDim = m_Dimensions * 0.5f;

	NODEID oppositeID = GetOppositeNode( childID );
	Vec3 octnodeOffset = m_OctantSignes[oppositeID] * halfDim;
	octnodeOffset += m_Position;

	// Store off the direction we're growing in.
	m_LastGrowth = oppositeID;

	// create new parent octnode
	COctreeSceneNode* newParentNode = new COctreeSceneNode( m_pOwner, this, childID, octnodeOffset, doubleDim, m_Depth-1 );
	m_pParentNode = newParentNode;
	return newParentNode;
}


COctreeSceneNode* COctreeSceneNode::InsertObjectIntoChild( ISceneObject* object, Vec3* position, float radius )
{
	// Find out which octant the object is in
	Vec3 nodeSignes;
	int whichNode = ClassifyObject( *position );

	// The node doesn't straddle a boundary
	if( whichNode != -1 )
	{
		// calculate the position of the new octant
		float quarterDim = m_Dimensions * 0.25f;
		Vec3 octantPosition( quarterDim, quarterDim, quarterDim );
		octantPosition *= m_OctantSignes[whichNode];
		octantPosition += m_Position;
		// calculate the new octant dimensions
		float octantDimensions = m_Dimensions * 0.5f;
		
		// If the object fits in the child octant then recurse into the child octant
		if( FitsInNode( *position, radius, octantPosition, octantDimensions ) )
		{
			NewChild( whichNode );

			return m_Children[whichNode]->UpdateSceneObject( object, position, radius );
		}
	}

	return NULL;
}

int COctreeSceneNode::GetChildCount()
{
	int count = 0;
	for (int i=0; i < 8; i++)
	{
		if (m_Children[i])
			count++;
	}
	return count;
}

void COctreeSceneNode::DeleteIfEmpty()
{
	if (m_iObjectCount == 0 && GetChildCount() == 0)
	{
		if (m_pParentNode)
		{
			m_pParentNode->DeleteChild( m_iUniqueID );
		}
		else
		{
			// this is root node, clear the scene
			m_pOwner->ClearScene();
		}
	}
}

void COctreeSceneNode::DeleteChild( DWORD iChildUniqueID )
{
	int iChild = -1;
	for (int i=0; i < 8; i++)
	{
		if (m_Children[i])
		{
			if (m_Children[i]->GetUniqueID() == iChildUniqueID)
			{
				iChild = i;
				break;
			}
		}
	}

	// assert child was correctly linked
	assert( iChild >= 0 );

	SAFE_DELETE( m_Children[iChild] );
	DeleteIfEmpty();
}
