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

#include "StdAfx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

REGISTER_COMPONENT(COctreeScene);

COctreeScene::COctreeScene( IHashString *parentName, IHashString *name ) :
	OBJECTTEMPLATE( COctreeSceneManager, COctreeScene, IScene, parentName, name )
{
	m_pRootNode = NULL;

	Init();
}

COctreeScene::~COctreeScene()
{
	DeInit();
}

IComponent* COctreeScene::Create( int nArgs, va_list argptr )
{
	IHashString *name;
	IHashString *parentName;
	IObject *self;

	name = va_arg( argptr, IHashString* );
	parentName = va_arg( argptr, IHashString* );
	// create the object however you must create it
	try
	{
		// try and create an instance
		self = new COctreeScene( parentName, name );
	}
	catch(...)
	{
		return NULL;
	}

	return self;
}

void COctreeScene::Init()
{
	ClearScene();
}

void COctreeScene::DeInit()
{
	ClearScene();
}

void COctreeScene::Visit( IVisitor* visitor, DWORD sortID )
{
	VisitGlobalObjects( visitor, sortID );
	if( m_pRootNode != NULL )
	{
		m_pRootNode->Visit( visitor, sortID );
	}
}

void COctreeScene::VisitFrustum( IVisitor* visitor, CFrustum* frustum, DWORD sortID )
{
	VisitGlobalObjects( visitor, sortID );
	if( m_pRootNode != NULL )
	{
		m_pRootNode->VisitFrustum( visitor, frustum, sortID );
	}
}

void COctreeScene::VisitAABB( IVisitor* visitor, AABB* aabb, DWORD sortID )
{
	VisitGlobalObjects( visitor, sortID );
	if( m_pRootNode != NULL )
	{
		m_pRootNode->VisitAABB( visitor, aabb, sortID );
	}
}

void COctreeScene::VisitSphere( IVisitor* visitor, Vec3* position, float radius, DWORD sortID )
{
	VisitGlobalObjects( visitor, sortID );
	if( m_pRootNode != NULL )
	{
		m_pRootNode->VisitSphere( visitor, position, radius, sortID );
	}
}

void COctreeScene::VisitGlobalObjects( IVisitor* visitor, DWORD sortID )
{
	GLOBALOBJECTSET &globalObjectSet = m_GlobalObjectSetMap[sortID];
	GLOBALOBJECTSET::iterator itr;
	for (itr = globalObjectSet.begin(); itr != globalObjectSet.end(); itr++)
	{
		visitor->Visit( *itr, true );
	}
}

void COctreeScene::AddGlobalSceneObject( ISceneObject* object )
{
	assert( object->GetSceneNode() == NULL ); // global object's shouldn't have scene nodes

	DWORD sortID = object->GetSceneSortID();
	GLOBALOBJECTSET &globalObjectSet = m_GlobalObjectSetMap[sortID];
	globalObjectSet.insert( object );

	object->SetSceneNode( this, NULL );
}

void COctreeScene::RemoveGlobalObject( ISceneObject* object )
{
	assert( object->GetSceneNode() == NULL ); // global object's shouldn't have scene nodes

	DWORD sortID = object->GetSceneSortID();
	GLOBALOBJECTSET &globalObjectSet = m_GlobalObjectSetMap[sortID];

	size_t iObjectsErased = globalObjectSet.erase( object );
	assert( iObjectsErased > 0 );

	object->SetSceneNode( NULL, NULL );
}

void COctreeScene::AddSceneObject( ISceneObject *pObject )
{
	UpdateObject( pObject );
}

void COctreeScene::UpdateObject( ISceneObject* object )
{
	// if object belonged to a scene node
	ISceneNode *pPreviousSceneNode = object->GetSceneNode();
	if (pPreviousSceneNode)
	{
		ISceneNode *nodeInserted = pPreviousSceneNode->UpdateSceneObject( object );

		// and object is being inserted into a different node
		if (pPreviousSceneNode->GetUniqueID() != nodeInserted->GetUniqueID())
		{
			// remove object from previous node
			pPreviousSceneNode->RemoveObject( object );

			// set new node object belongs to
			object->SetSceneNode( this, nodeInserted );
		}
	}
	else
	{
		// Check if this is the first object to be inserted into the octree
		if( m_pRootNode == NULL )
		{
			// Get the object's bounding sphere 
			Vec3 objPosition;
			float objRadius;
			object->GetBoundingSphere( objPosition, objRadius );
			// Create root node
			// TODO: root octree node size is fairly subjective.
			// Some experiments should be done to see what makes for the most compact tree
			// under various seed conditions.
			float rootRadius = objRadius * (float)(OCTREENODE_TARGET_DENSITY / 2);
			rootRadius = max( rootRadius, 10.0f ); // set a minimum so we don't create a root of size 0
			m_pRootNode = new COctreeSceneNode( this, NULL, objPosition, rootRadius, 0 );
		}

		// Add object to the octree from root
		ISceneNode* nodeInserted;
		nodeInserted = m_pRootNode->UpdateSceneObject( object );

		// set new node object belongs to
		object->SetSceneNode( this, nodeInserted );
	}

	// Sanity check that the object belongs to the scene node that the object points to
//	assert( object->GetSceneNode()->GetObjSet( object->GetSceneSortID() )->find( object->GetName()->GetUniqueID() ) != 
//		object->GetSceneNode()->GetObjSet( object->GetSceneSortID() )->end() );
}

void COctreeScene::ClearScene()
{
	SAFE_DELETE( m_pRootNode );

	// remove global objects from scene
	GLOBALOBJECTSETMAP::iterator itrSets = m_GlobalObjectSetMap.begin();
	while (itrSets != m_GlobalObjectSetMap.end())
	{
		GLOBALOBJECTSET &globalObjectSet = itrSets->second;
		GLOBALOBJECTSET::iterator itrSet = globalObjectSet.begin();
		while (itrSet != globalObjectSet.end())
		{
			ISceneObject* object = *itrSet;
			object->SetSceneNode( NULL, NULL );
			++itrSet;
		}

		++itrSets;
	}
	m_GlobalObjectSetMap.clear();
}
