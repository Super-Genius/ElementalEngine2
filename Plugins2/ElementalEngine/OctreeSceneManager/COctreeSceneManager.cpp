///==========================================================================
/// \file	COCtreeSceneManager.cpp
/// \brief	Manages COctreeScenes
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
#include "PerformanceProfiler.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// Register the singleton
REGISTER_COMPONENT_SINGLETON(COctreeSceneManager);

// ------------------ Messages ------------------ //
REGISTER_MESSAGE_HANDLER( SetActiveScene, OnSetActiveScene, COctreeSceneManager );
REGISTER_MESSAGE_HANDLER( VisitScene, OnVisitScene, COctreeSceneManager );
REGISTER_MESSAGE_HANDLER( VisitSceneFrustum, OnVisitSceneFrustum, COctreeSceneManager );
REGISTER_MESSAGE_HANDLER( VisitSceneAABB, OnVisitSceneAABB, COctreeSceneManager );
REGISTER_MESSAGE_HANDLER( VisitSceneSphere, OnVisitSceneSphere, COctreeSceneManager );

REGISTER_MESSAGE_HANDLER( AddSceneObject, OnAddSceneObject, COctreeSceneManager );
REGISTER_MESSAGE_HANDLER( UpdateBoundingBox, OnAddSceneObject, COctreeSceneManager );

REGISTER_MESSAGE_HANDLER( RemoveSceneObject, OnRemoveSceneObject, COctreeSceneManager );
REGISTER_MESSAGE_HANDLER( RemoveBoundingBoxedObject, OnRemoveSceneObject, COctreeSceneManager );

REGISTER_MESSAGE_HANDLER( ClearScene, OnClearScene, COctreeSceneManager );


COctreeSceneManager::COctreeSceneManager() : 
	CManager(_T("COctreeSceneManager"), HIERARCHYPRIORITY),
	m_hszCOctreeSceneType( _T("COctreeScene") )
{
	m_pActiveOctreeScene = NULL;
}

COctreeSceneManager::~COctreeSceneManager()
{
	DeInit();
}

IComponent *COctreeSceneManager::Create( int nArgs, va_list argptr )
{
  	return SINGLETONINSTANCE( COctreeSceneManager );
}

bool COctreeSceneManager::Init()
{
	return CManager::Init();
}

void COctreeSceneManager::Update(DWORD tickCount)
{
}

bool COctreeSceneManager::DeInit()
{
	return CManager::DeInit();
}

// ------------------ Messages ------------------ //

DWORD COctreeSceneManager::OnSetActiveScene(DWORD size, void* data)
{
	VERIFY_MESSAGE_SIZE( sizeof(GETSETACTIVESCENE), size );
	GETSETACTIVESCENE* pParams = (GETSETACTIVESCENE*)data;

	if (pParams->pSceneName == NULL)
		return MSG_ERROR;

	IDTOOBJECTMAP *pObjectMap = NULL;

	// generate unique name for new octree.
	StdString szNewOctreeName( _T("COctreeScene_") );
	szNewOctreeName += pParams->pSceneName->GetString();
	CHashString hszNewOctreeName( szNewOctreeName );
	DWORD octreeSceneNameID = hszNewOctreeName.GetUniqueID();

	// reset ptr
	m_pActiveOctreeScene = NULL;

	// Search the objectmap for the octree
	pObjectMap = GetObjectMap( &m_hszCOctreeSceneType );
	if( pObjectMap != NULL )
	{
		IDTOOBJECTMAP::iterator itr = pObjectMap->find(octreeSceneNameID);
		if (itr != pObjectMap->end())
			m_pActiveOctreeScene = dynamic_cast<COctreeScene*>(itr->second);
	}

	// If no octree was found in the objectmap create a new one.
    if (m_pActiveOctreeScene == NULL)
	{
		CREATEOBJECTPARAMS cop;
		cop.name = &hszNewOctreeName;
		cop.parentName = NULL;
		cop.typeName = &m_hszCOctreeSceneType;
		static DWORD msgHash_CreateObject = CHashString(_T("CreateObject")).GetUniqueID();
		m_ToolBox->SendMessage( msgHash_CreateObject, sizeof(cop), &cop );

		// look for the octree again (it should be there).
		pObjectMap = GetObjectMap( &m_hszCOctreeSceneType );
		if( pObjectMap != NULL )
		{
			IDTOOBJECTMAP::iterator itr = pObjectMap->find( octreeSceneNameID );
			if( itr != pObjectMap->end() )
			{
				m_pActiveOctreeScene = static_cast<COctreeScene*>(itr->second);
			}
		}
	}

	m_hszActiveSceneName.Init( pParams->pSceneName->GetString() );

	return MSG_HANDLED_PROCEED;
}

DWORD COctreeSceneManager::OnVisitScene( DWORD size, void* params )
{
	VERIFY_MESSAGE_SIZE( sizeof(VISITSCENEPARAMS), size );
	VISITSCENEPARAMS* vsp = (VISITSCENEPARAMS*)params;

	if( m_pActiveOctreeScene != NULL )
	{
		m_pActiveOctreeScene->Visit( vsp->visitor, vsp->sortID );
	}

	return MSG_HANDLED_PROCEED;
}

DWORD COctreeSceneManager::OnVisitSceneFrustum( DWORD size, void* params )
{
	VERIFY_MESSAGE_SIZE( sizeof(VISITSCENEFRSUTUMPARAMS), size );
	VISITSCENEFRSUTUMPARAMS* vsp = (VISITSCENEFRSUTUMPARAMS*)params;

	m_pActiveOctreeScene->VisitFrustum( vsp->visitor, vsp->frustum, vsp->sortID );

	return MSG_HANDLED_PROCEED;
}

DWORD COctreeSceneManager::OnVisitSceneAABB( DWORD size, void* params )
{
	VERIFY_MESSAGE_SIZE( sizeof(VISITSCENEAABBPARAMS), size );
	VISITSCENEAABBPARAMS* vsp = (VISITSCENEAABBPARAMS*)params;

	m_pActiveOctreeScene->VisitAABB( vsp->visitor, vsp->aabb, vsp->sortID );

	return MSG_HANDLED_PROCEED;
}

DWORD COctreeSceneManager::OnVisitSceneSphere( DWORD size, void* params )
{
	VERIFY_MESSAGE_SIZE( sizeof(VISITSCENESPHEREPARAMS), size );
	VISITSCENESPHEREPARAMS* vsp = (VISITSCENESPHEREPARAMS*)params;

	m_pActiveOctreeScene->VisitSphere( vsp->visitor, vsp->center, vsp->radius, vsp->sortID );

	return MSG_HANDLED_PROCEED;
}

DWORD COctreeSceneManager::OnAddSceneObject( DWORD size, void* params )
{
	VERIFY_MESSAGE_SIZE(sizeof(SCENEOBJECTPARAMS), size);
	SCENEOBJECTPARAMS *sop = (SCENEOBJECTPARAMS *)params;
	
	if (sop->bBoundedObject)
	{
		m_pActiveOctreeScene->AddSceneObject( sop->pObject );
	}
	else
	{
		m_pActiveOctreeScene->AddGlobalSceneObject( sop->pObject );
	}

	return MSG_HANDLED_PROCEED;
}

DWORD COctreeSceneManager::OnRemoveSceneObject( DWORD size, void* params )
{
	VERIFY_MESSAGE_SIZE(sizeof(SCENEOBJECTPARAMS), size);
	SCENEOBJECTPARAMS *sop = (SCENEOBJECTPARAMS *)params;

	if (sop->bBoundedObject)
	{
		ISceneNode *pSceneNode = sop->pObject->GetSceneNode();
		if (pSceneNode)
		{
			pSceneNode->RemoveObject( sop->pObject );
		}
	}
	else
	{
		IScene *pScene = sop->pObject->GetScene();
		if (pScene)
		{
			pScene->RemoveGlobalObject( sop->pObject );
		}
	}

	return MSG_HANDLED_PROCEED;
}

DWORD COctreeSceneManager::OnClearScene( DWORD size, void* params )
{
	m_pActiveOctreeScene->ClearScene();

	return MSG_HANDLED_PROCEED;
}