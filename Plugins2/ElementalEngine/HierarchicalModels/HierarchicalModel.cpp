///============================================================================
/// \note   Elemental Engine
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
///============================================================================

#include "stdafx.h"

#include "PerformanceProfiler.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define DEGTOEULER 3.14159265f / 180.f
#define RADTODEG 180.f/3.14159265f

REGISTER_COMPONENT(CHierarchicalModel);

//REGISTER_MESSAGE_HANDLER(SetChildPositionOffset,	OnSetChildPositionOffset, CHierarchicalModel );
//REGISTER_MESSAGE_HANDLER(SetChildRotationOffset,	OnSetChildRotationOffset, CHierarchicalModel );
//REGISTER_MESSAGE_HANDLER(SetChildScaleOffset,		OnSetChildScaleOffset, CHierarchicalModel );
//REGISTER_MESSAGE_HANDLER(GetChildPositionOffset,	OnGetChildPositionOffset, CHierarchicalModel );
//REGISTER_MESSAGE_HANDLER(GetChildRotationOffset,	OnGetChildRotationOffset, CHierarchicalModel );
//REGISTER_MESSAGE_HANDLER(GetChildScaleOffset,		OnGetChildScaleOffset, CHierarchicalModel );
//REGISTER_MESSAGE_HANDLER(GetChildGlobalRotation,	OnGetChildGlobalRotation, CHierarchicalModel );
//REGISTER_MESSAGE_HANDLER(GetChildGlobalPosition,	OnGetChildGlobalPosition, CHierarchicalModel );
//REGISTER_MESSAGE_HANDLER(GetChildGlobalScale,		OnGetChildGlobalScale, CHierarchicalModel );
//REGISTER_MESSAGE_HANDLER(GetChildGlobalMatrix,		OnGetChildGlobalMatrix, CHierarchicalModel );

REGISTER_MESSAGE_HANDLER(GetGlobalBoneInfo, OnGetGlobalBoneInfo, CHierarchicalModel);
REGISTER_MESSAGE_HANDLER(SetGlobalBoneInfo, OnSetGlobalBoneInfo, CHierarchicalModel);
REGISTER_MESSAGE_HANDLER(GetOffsetBoneInfo, OnGetOffsetBoneInfo, CHierarchicalModel);
REGISTER_MESSAGE_HANDLER(SetOffsetBoneInfo, OnSetOffsetBoneInfo, CHierarchicalModel);

REGISTER_MESSAGE_HANDLER( PlayMatrixAnimation, OnPlayAnimation, CHierarchicalModel );
//REGISTER_MESSAGE_HANDLER( GetChildID, OnGetChildID, CHierarchicalModel );

REGISTER_MESSAGE_HANDLER( GetBoundingSphere,  OnGetBoundingSphere, CHierarchicalModel );

REGISTER_MESSAGE_HANDLER( GetGeometryAllocations, OnGetGeometryAllocations, CHierarchicalModel );
REGISTER_MESSAGE_HANDLER( GetGeometryAllocationByName, OnGetGeometryAllocationByName, CHierarchicalModel );
REGISTER_MESSAGE_HANDLER( GetBoundingBox, OnGetBoundingBox, CHierarchicalModel );

// forwarded to parent
REGISTER_MESSAGE_HANDLER( GetGlobalPosition, OnGetGlobalPosition, CHierarchicalModel );
REGISTER_MESSAGE_HANDLER( GetGlobalEuler, OnGetGlobalEuler, CHierarchicalModel );
REGISTER_MESSAGE_HANDLER( GetGlobalScale, OnGetGlobalScale, CHierarchicalModel );
REGISTER_MESSAGE_HANDLER( GetGlobalTransform, OnGetGlobalTransform, CHierarchicalModel );

REGISTER_MESSAGE_HANDLER( SetSkeletonPose, OnSetSkeletonPose, CHierarchicalModel );
REGISTER_MESSAGE_HANDLER( EnablePhysicsAnimations, OnEnablePhysicsAnimations, CHierarchicalModel );

REGISTER_MESSAGE_HANDLER( GetParentName, OnGetParentName, CHierarchicalModel );

REGISTER_MESSAGE_HANDLER( SetMaterialOverride, OnSetMaterialOverride, CHierarchicalModel );
REGISTER_MESSAGE_HANDLER( ClearMaterialOverride, OnClearMaterialOverride, CHierarchicalModel );

REGISTER_MESSAGE_HANDLER( AttachSceneObject, OnAttachSceneObject, CHierarchicalModel );
REGISTER_MESSAGE_HANDLER( AlignToSceneObject, OnAlignToSceneObject, CHierarchicalModel );

IComponent *CHierarchicalModel::Create(int nArgs, va_list argptr)
{
	IHashString *name;
	IHashString *parentName;
	IObject *self;
	bool bAddToHier;

	name = va_arg(argptr, IHashString *);
	parentName = va_arg(argptr, IHashString *);
	// create the object however you must create it
	try
	{
		// try and create an instance
		//EE2FIX
		if (nArgs == 2)
		{
			self = new CHierarchicalModel( parentName, name, true);
		}
		else if (nArgs == 3)
		{
			bAddToHier = va_arg(argptr, bool);
			self = new CHierarchicalModel( parentName, name, bAddToHier);
		}
	}
	catch(...)
	{
		return NULL;
	}

	return self;
}


CHierarchicalModel::CHierarchicalModel(IHashString *parentName, IHashString *name, bool bAddToHier) :
	SCENEOBJECTTEMPLATE(CHModelManager, CHierarchicalModel, IRenderObject, parentName, name),
	m_hierAnimTypeName(_T("HierarchicalAnimation"))
{
	m_HierarchicalModelManager = SINGLETONINSTANCE(CHModelManager);

	m_bUseGlobalOffsetTransforms = false;
	m_bAddToHierarchy = bAddToHier;
	if (bAddToHier)
	{
		AddToHierarchy();
	}
	m_pCachedRootNode = NULL;
	m_pHierType = NULL;
	
	GETRENDERERINTERFACEPARAMS renderInterface;
	DWORD msgHash_GetRendererInterface = CHashString(_T("GetRendererInterface")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_GetRendererInterface, 
					sizeof(GETRENDERERINTERFACEPARAMS), 
					&renderInterface, 
					NULL, 
					NULL);
	m_IRenderer = renderInterface.m_RendererInterface;
	static DWORD msgHash_CreateController = CHashString(_T("CreateController")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_CreateController, sizeof( IController** ), &m_RootController );	
	m_AnimTime = 0.f;
	m_CurrentAnimation = NULL;
	m_CoreHierarchy = NULL;
	m_bPlaying = false;
	m_MaxAnimTime = 0;
	m_AnimSpeed = 0;
	m_bAnimLoop = false;
	m_bFirstUpdate = true;
	m_bFreezeLastFrame = false;
	m_bTransformIsDirty = true;

	m_bEnablePhysicsAnimation = false;

	SerializedPosition.Set( 0, 0, 0 );
	SerializedScaling.Set( 1, 1, 1 );
	SerializedRotation.Set( 0, 0, 0 );
}
void CHierarchicalModel::ResetAnimation()
{
	m_AnimTime = 0.f;
	m_bPlaying = false;
	m_MaxAnimTime = 0;
	m_AnimSpeed = 0;
}

void CHierarchicalModel::Init()
{
	//retrieve the hierarchical template from the core model
	if (m_HierarchicalModelManager)
	{
		// Try to retrieve model from Manager to see if loaded.
		m_pHierType = m_HierarchicalModelManager->GetHierarchicalModelType(&m_CoreModel);
		// Has not been loaded, load file.
		if (!m_pHierType)
		{
			m_ToolBox->Log( LOGINFORMATION, _T("HIERARCHICALMODEL: PERFORMANCE: %s\n\tNot Precached.\n"), (TCHAR*)m_CoreModel.GetString() );
			//maybe no model? try loading it
			LOADFILEEXTPARAMS lfep;
			StdString szFileName = m_CoreModel.GetString();
			szFileName.MakeSafeFileName();
			CHashString filename( szFileName  );
			lfep.fileName = (TCHAR*)filename.GetString();
			lfep.bInternalLoad = true;
			static DWORD msgHash_LoadFileByExtension = CHashString(_T("LoadFileByExtension")).GetUniqueID();
			DWORD retval = EngineGetToolBox()->SendMessage(msgHash_LoadFileByExtension, sizeof(LOADFILEEXTPARAMS), &lfep);
			if ( retval != MSG_HANDLED )
			{
				m_ToolBox->Log( LOGWARNING, _T("HIERARCHICALMODEL: %s\n\tLoad Failed.\n"), m_CoreModel.GetString() );
			}
		}
		// Try to retrieve model from Manager again, should be loaded now.
		m_pHierType = m_HierarchicalModelManager->GetHierarchicalModelType(&m_CoreModel);
		if (m_pHierType)
		{
			HNODEVECTOR *list = m_pHierType->GetHierarchyTemplate();
			InitHierarchicalNodeData( *list );
			BuildInternalRenderObjects();
		}
		else
		{
			m_ToolBox->Log( LOGERROR, _T("HIERARCHICALMODEL: %s\n\tFailed to get model from manager.\n"), m_CoreModel.GetString() );
		}
	}
	else
	{
		m_ToolBox->Log( LOGFATALERROR, _T("HIERARCHICALMODEL: NULL Manager\n"));
	}
}

CHierarchicalModel::~CHierarchicalModel()
{
	//delete all render objects that belong to this
	ClearHierarchicalNodeData();
	if (m_bAddToHierarchy)
	{
		RemoveFromHierarchy();
	}

	if( m_RootController != NULL )
	{
		static DWORD msgHash_DestroyController = CHashString(_T("DestroyController")).GetUniqueID();
		m_ToolBox->SendMessage(msgHash_DestroyController, sizeof( IController** ), &m_RootController );
	}
	m_RootController = NULL;



	DeInit();
}


void CHierarchicalModel::DeInit(  )
{
}

void CHierarchicalModel::Serialize(IArchive &ar)
{	
	if( ar.IsReading() )
	{
		// Get the data's version number
		CHashString hszVersion = _T("");

		float version;
		static DWORD msgHash_GetFileVersion = CHashString(_T("GetFileVersion")).GetUniqueID();
		DWORD retval = m_ToolBox->SendMessage(msgHash_GetFileVersion, sizeof(IHashString), &hszVersion);
		if (retval != MSG_HANDLED)
		{
			m_ToolBox->Log(LOGERROR, _T("HierchicalModel Serialize: Could not get file version!"));
			assert(0);
			return;
		}
		float atofVersion = (float)_tstof(hszVersion.GetString());
		version = atofVersion;

		StdString filename;
		ar.Read( filename );
		filename.MakeSafeFileName();
		m_CoreModel.Init( filename );
		//now read the basic stuff
		if( version < 2.5 )
		{
			ar.Read( SerializedPosition, _T("Position") );
			ar.Read( SerializedRotation, _T("Rotation") );
			ar.Read( SerializedScaling, _T("Scaling") );
			if( m_RootController )
			{
				m_RootController->SetTranslationOffset( SerializedPosition );
				m_RootController->SetRotationOffset( SerializedRotation );
				m_RootController->SetScaling( SerializedScaling );
				m_RootController->BuildRotation();
			}
			// if you're serializing positional data, this should be true in case we need to get the data
			SetDirtyTransformFlag();
		}

	}else
	{
		ar.Write( m_CoreModel.GetString() );
	}
}

DWORD CHierarchicalModel::OnPlayAnimation(DWORD size, void *params)
{
	VERIFY_MESSAGE_SIZE( size, sizeof( HIERARCHICALANIMATION ) );
	HIERARCHICALANIMATION * p = (HIERARCHICALANIMATION*)params;
	if( !p->animtemplate )
		return MSG_ERROR;


	StdString szFileName = p->animtemplate->GetString();
	szFileName.MakeSafeFileName();
	CHashString loweranim( szFileName );

	//retrieve the hierarchical template from the core model
	GETHIERARCHYTEMPLATEPARAM msg;
	HIERARCHICALMODELGETANIMLENGTH len;
	len.length = 0;
	static DWORD msgHash_GetAnimationTime = CHashString(_T("GetAnimationTime")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_GetAnimationTime, sizeof( HIERARCHICALMODELGETANIMLENGTH), &len, 
		&loweranim, &m_hierAnimTypeName );	
	static DWORD msgHash_GetHierarchyTemplate = CHashString(_T("GetHierarchyTemplate")).GetUniqueID();
	DWORD retval = m_ToolBox->SendMessage(msgHash_GetHierarchyTemplate, sizeof( GETHIERARCHYTEMPLATEPARAM), &msg, 
		&loweranim, &m_hierAnimTypeName );	
	if ((retval == MSG_HANDLED) && (msg.list != NULL))
	{
		ExecuteAnimation( msg.list, len.length, p->speed );
		m_bAnimLoop = p->loop;
		m_bFreezeLastFrame = p->freezeOnLastFrame;
	}else
	{
		m_ToolBox->Log( LOGINFORMATION, _T("Animation was not precached, now trying to load: %s.\n"), szFileName.c_str() );
        //maybe no model? try loading it
		LOADFILEEXTPARAMS lfep;
		CHashString filename( szFileName  );
		lfep.fileName = (TCHAR*)szFileName.c_str();
		lfep.bInternalLoad = true;
		static DWORD msgHash_LoadFileByExtension = CHashString(_T("LoadFileByExtension")).GetUniqueID();
		DWORD retval = EngineGetToolBox()->SendMessage(msgHash_LoadFileByExtension, sizeof(LOADFILEEXTPARAMS), &lfep);
		static DWORD msgHash_GetAnimationTime = CHashString(_T("GetAnimationTime")).GetUniqueID();
		m_ToolBox->SendMessage(msgHash_GetAnimationTime, sizeof( HIERARCHICALMODELGETANIMLENGTH), &len, 
			&loweranim, &m_hierAnimTypeName );	
		static DWORD msgHash_GetHierarchyTemplate = CHashString(_T("GetHierarchyTemplate")).GetUniqueID();
		retval = m_ToolBox->SendMessage(msgHash_GetHierarchyTemplate, sizeof( GETHIERARCHYTEMPLATEPARAM), &msg, 
			&loweranim, &m_hierAnimTypeName );	
		if ((retval == MSG_HANDLED) && (msg.list != NULL))
		{
			ExecuteAnimation( msg.list, len.length, p->speed );
			m_bAnimLoop = p->loop;
			m_bFreezeLastFrame = p->freezeOnLastFrame;
   		}else
		{
			m_ToolBox->Log( LOGERROR, _T("Error, file was not loaded: %s"), szFileName.c_str() );
			return MSG_ERROR;
		}
	}
	return MSG_HANDLED_STOP;
}

void CHierarchicalModel::ExecuteAnimation(  HNODEVECTOR * list, float time, float speed )
{
	ResetAnimation();
	if( list )
	{
		m_CurrentAnimation = list;
		m_pCachedRootNode = NULL;
		m_bPlaying = true;		
		m_AnimSpeed = speed;
		m_MaxAnimTime = time/m_AnimSpeed;
	}
}



bool CHierarchicalModel::Update()
{	
	PERFORMANCE_PROFILER_AUTO(HierarchicalModel_Update);
	static CHashString hsGameTime(GAMEDELTATIMER);

	if( !m_CurrentAnimation ||
		!m_CoreHierarchy ||
		!m_RootController )
		return true;

	if ( m_bFirstUpdate )
	{
		SetDirtyTransformFlag();
		m_bFirstUpdate = false;
	}

	// If we are playing an animation:
	if( m_bPlaying )
	{
		float TimeDeltaSeconds = m_Timer->GetTimeDelta( &hsGameTime );
		m_AnimTime += TimeDeltaSeconds;
		// End of Animation Reached
		if( m_AnimTime > m_MaxAnimTime )
		{
			// Freeze Last Frame. 
			if( m_bFreezeLastFrame )
			{
				m_AnimTime = m_MaxAnimTime;
				m_bPlaying = false;
			}
			// Loop Back to Beginning.
			else if( m_bAnimLoop )
			{
				m_AnimTime = 0.f;
			}
			else // Freeze First Frame
			{
				m_AnimTime = 0.f;
				m_bPlaying = false;
			}
			SetDirtyTransformFlag();
		}
	}

	CheckDirtyTransform();

	return true;
}

void CHierarchicalModel::RecurseSetupTransforms( HierarchicalNode * node)
{
	static Vec3			NodeOffsetScale;
	static EulerAngle	NodeOffsetRotation;
	static Vec3			NodeOffsetTranslation;

	int index = node->m_Index;
	// Check to see if ValidChild ONCE!
	if (!IsValidChild( index ))
	{
		// This is caused by a bad data file, code now survives but objects are illhappen.
		assert(0);
		return;
	}
	float curtime = m_AnimTime*m_AnimSpeed;
	m_HierarchicalNodeData[ index ].UpdateRecurse(curtime, node);

	//recurse through children
	set< HierarchicalNode * >::iterator iter = node->m_Children.begin();
	for( ; iter != node->m_Children.end(); iter++ )
	{
		HierarchicalNode *  child = (*iter);
		RecurseSetupTransforms( child );
	}
}

bool CHierarchicalModel::Render( UINT Pass, IEffect * override)
{
	return true;
}

bool CHierarchicalModel::IsLessThan( const IRenderObject * obj, DWORD sortMethod, int curRenderPass )
{
	return true;
}

void CHierarchicalModel::SetPosition( const Vec3 &vec )
{
}

void CHierarchicalModel::GetBoundingSphere( Vec3 &position, float &radius )
{
	//might want to only do this once every few frames
	radius = 0.f;
	RecalculateBoundingSphere( position, radius );
}

bool CHierarchicalModel::GetBoundingBox( Vec3 &pos, Vec3 &dim, EulerAngle &rot )
{
	RecalculateBoundingBox( pos, dim, rot );
	return true;
}

void CHierarchicalModel::RecalculateBoundingSphere( Vec3 &position, float &radius )
{
	static CHashString hsRenderObjectType(_T("Cal3DRenderObject"));
	if( m_RootController )
	{
		CheckDirtyTransform();	
		//slow function
		GETBOUNDINGSPHEREPARAMS parm;
		Vec3 curpos;
		parm.position = &curpos;
		parm.radius = 0.f;
		DWORD retval;
		float maxradius = 0;
		for( int i = 0; i < (int)m_HierarchicalNodeData.size(); i++ )
		{
			static DWORD msgHash_GetBoundingSphere = CHashString(_T("GetBoundingSphere")).GetUniqueID();
			retval = m_ToolBox->SendMessage(msgHash_GetBoundingSphere, sizeof( parm ), &parm, &m_HierarchicalNodeData[ i ].m_RenderObject, &hsRenderObjectType );
			if( retval == MSG_HANDLED )
			{
				//add it
				Vec3 dif = curpos - m_RootController->GetTranslationOffset();
				float maxdistancefromcenter = parm.radius + dif.Length();
				maxradius = MAX( maxradius, maxdistancefromcenter );
			}
		}
		radius = maxradius;
		position = m_RootController->GetTranslationOffset();	
	}
}

void CHierarchicalModel::RecalculateBoundingBox( Vec3& position, Vec3& extents, EulerAngle& rotation )
{
	if( m_RootController )
	{
		//slow function
		GETBOUNDINGBOXPARAMS parm;
		Vec3 curpos;
		Vec3 curext;
		Vec3 maxextents;
		Vec3 minextents;
		EulerAngle currot;
		parm.position = &curpos;
		parm.extents = &curext;
		parm.rotation = &currot;

		DWORD retval;
		float maxX = 0;
		float maxY = 0;
		float maxZ = 0;
		float minX = 99999999.f;
		float minY = 99999999.f;
		float minZ = 99999999.f;
		// Grab the bounding box for each render node
		for( int i = 0; i < (int)m_HierarchicalNodeData.size(); i++ )
		{
			static DWORD msgHash_GetBoundingBox = CHashString(_T("GetBoundingBox")).GetUniqueID();
			static CHashString inputManager(_T("Cal3DRenderObject"));
			retval = m_ToolBox->SendMessage(msgHash_GetBoundingBox, sizeof(parm), &parm, &m_HierarchicalNodeData[i].m_RenderObject, &inputManager );
			if( retval == MSG_HANDLED )
			{
				// check if the extents of the sub-object are larger then the current total extents
				maxX = MAX( curext.x + curpos.x, maxX );
				maxY = MAX( curext.y + curpos.y, maxY );
				maxZ = MAX( curext.z + curpos.z, maxZ );
				minX = MIN( -curext.x + curpos.x, minX );
				minY = MIN( -curext.y + curpos.y, minY );
				minZ = MIN( -curext.z + curpos.z, minZ );
			}
		}
		maxextents.Set( maxX, maxY, maxZ );
		minextents.Set( minX, minY, minZ );
		extents = (maxextents - minextents) * 0.5f;
		position = minextents + extents;
		rotation = m_RootController->GetRotationOffset();
	}
}

DWORD CHierarchicalModel::GetRenderPriority()
{
	return RENDERPRIORITY_OPAQUE;
}

bool CHierarchicalModel::IsKindOf( IHashString * compType )
{	
	static CHashString  CTSROCOMP(_T("CHierarchicalModel") );
	return (compType->GetUniqueID() == CTSROCOMP.GetUniqueID());
}


void CHierarchicalModel::InitHierarchicalNodeData( HNODEVECTOR &Template )
{
	UINT size = Template.size();
	m_HierarchicalNodeData.resize( size );
	for(unsigned int i = 0; i < m_HierarchicalNodeData.size(); i++)
	{
		m_HierarchicalNodeData[i].SetFileNodeData(Template[i]);// TODO: THIS IS NOT CORRECT DATA
		m_HierarchicalNodeData[i].SetController(m_RootController);
	}
	for(unsigned int i = 0; i < Template.size(); i++)
	{
		set< HierarchicalNode * >::iterator iter = Template[i]->m_Children.begin();
		for( ; iter != Template[i]->m_Children.end(); iter++ )
		{
			HierarchicalNode *  child = (*iter);
			m_HierarchicalNodeData[child->m_Index].SetParent(&m_HierarchicalNodeData[i]);
		}
	}
	m_CoreHierarchy = &Template;
	m_CurrentAnimation = m_CoreHierarchy;//start pose
	m_pCachedRootNode = NULL;
}

void CHierarchicalModel::ClearHierarchicalNodeData()
{
	DELETEOBJECTPARAMS dop;
	UINT size = m_HierarchicalNodeData.size();
	for( UINT i = 0; i < size; i++ )
	{
		dop.name = &m_HierarchicalNodeData[ i ].m_RenderObject;
		static DWORD msgHash_DeleteObject = CHashString(_T("DeleteObject")).GetUniqueID();
		m_ToolBox->SendMessage(msgHash_DeleteObject, sizeof(DELETEOBJECTPARAMS), &dop, NULL, NULL);
	}
	m_HierarchicalNodeData.clear();
}

void CHierarchicalModel::BuildInternalRenderObjects()
{
	UINT size = m_HierarchicalNodeData.size();
	if( m_CoreHierarchy &&
		size == m_CoreHierarchy->size() )
	{
		HNODEVECTOR::iterator iter = m_CoreHierarchy->begin();
		for( UINT i = 0; i < size; i++, ++iter )
		{
			HierarchicalNode * node = (*iter);		
			if( node->GetObjectType() == NodeType_Cal3D )
			{
				if( !m_HierarchicalNodeData[ i ].m_bInitialized )
				{
					HierarchicalNodeData &rnode =  m_HierarchicalNodeData[ i ];
					CreateCalRenderObject( rnode, node, i );
					m_HierarchicalNodeData[ i ].m_bInitialized = true;
					m_HierarchicalNodeData[ i ].m_hsNodeName.Init(node->GetName());
					m_HierarchicalNodeData[ i ].m_bPhysicsControlled = false;
				}
			}
			else if( node->GetObjectType() == NodeType_Cloth )
			{
				if( !m_HierarchicalNodeData[ i ].m_bInitialized )
				{
					HierarchicalNodeData &rnode =  m_HierarchicalNodeData[ i ];
					CreateClothRenderObject( rnode, node, i );
					m_HierarchicalNodeData[ i ].m_bInitialized = true;
					m_HierarchicalNodeData[ i ].m_hsNodeName.Init(node->GetName());
					m_HierarchicalNodeData[ i ].m_bPhysicsControlled = false;
				}
			}
		}
	}
}

void CHierarchicalModel::CreateCalRenderObject( HierarchicalNodeData &rnode, HierarchicalNode * node, int idd )
{
	IHashString * pname = GetName();
	if( !pname )return;
	
	CHashString hszParentName( pname->GetString() );
	CHashString hszTypeName(_T("Cal3DRenderObject"));	
	TCHAR newname[1024];
	_stprintf( newname, "%s%s%d\0", pname->GetString(), (const TCHAR*) node->GetName(), idd );
	rnode.m_RenderObject.Init( newname );
	//m_ToolBox->Log( LOGINFORMATION, _T("Loading Cal3d object: %s\n"), (const TCHAR *)newname );

	IComponent *rendComp = m_ToolBox->CreateComponent(&hszTypeName, 3, &rnode.m_RenderObject, &hszParentName, false);
	if( !rendComp )
	{
		m_ToolBox->Log( LOGERROR, _T("Could not load render object for hierarchical model: %s\n"), pname->GetString() );
		return;
	}

	OFACTORYADDPARAMS ofap;
	ofap.name = &rnode.m_RenderObject;
	ofap.component = rendComp;
	static DWORD msgHash_AddObjectToFactory = CHashString(_T("AddObjectToFactory")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_AddObjectToFactory, sizeof(OFACTORYADDPARAMS), &ofap);

	IObject * RendObj = (IObject * )rendComp;
	IArchive *MemArchive;
	CHashString memType(_T("Memory"));

	CREATEARCHIVE caOut;
	caOut.mode = STREAM_MODE_WRITE | STREAM_MODE_READ;
	caOut.streamData = NULL;
	caOut.streamSize = 0;
	caOut.streamType = &memType;
	static DWORD msgHash_CreateArchive = CHashString(_T("CreateArchive")).GetUniqueID();
	if (m_ToolBox->SendMessage(msgHash_CreateArchive, sizeof(CREATEARCHIVE), &caOut) != MSG_HANDLED)
	{
		return;
	}
	Vec3 Position;
	EulerAngle Rotation;
	Vec3 Scaling;
	if( m_RootController )
	{
		node->GetScaling( Scaling );
		node->GetRotation( Rotation );
		node->GetTranslation( Position );
	}
	MemArchive = caOut.archive;
	//MemArchive->Write( node->GetClassName(), _T("FileName") );
	MemArchive->Write( node->GetFilePath(), _T("FileName") );
	MemArchive->Write( Position, _T("Position"));
	MemArchive->Write( Rotation, _T("Rotation") );
	MemArchive->Write( Scaling, _T("Scaling") );		
	MemArchive->Write( _T("Shaders/Cal3DEffect_Noskin.efx") , _T("Shader"));

	MemArchive->SetIsWriting(false);
	MemArchive->SeekTo(0);

	RendObj->Serialize( *MemArchive );
	RendObj->Init();

	MemArchive->Close();

	rnode.SetInterfaces();

}

void CHierarchicalModel::CreateClothRenderObject( HierarchicalNodeData &rnode, HierarchicalNode * node, int idd )
{
	IHashString * pname = GetName();
	if( !pname )return;
	
	CHashString hszParentName( pname );
	CHashString hszTypeName(_T("CClothRenderObject"));	
	TCHAR newname[1024];
	_stprintf( newname, "%s%s%d\0", pname->GetString(), (const TCHAR*) node->GetName(), idd );
	rnode.m_RenderObject.Init( newname );
	//m_ToolBox->Log( LOGINFORMATION, _T("Loading Cal3d object: %s\n"), (const TCHAR *)newname );

	IComponent *rendComp = m_ToolBox->CreateComponent(&hszTypeName, 3, &rnode.m_RenderObject, &hszParentName, false);
	if( !rendComp )
	{
		m_ToolBox->Log( LOGERROR, _T("Could not load render object for hierarchical model: %s\n"), pname->GetString() );
		return;
	}

	OFACTORYADDPARAMS ofap;
	ofap.name = &rnode.m_RenderObject;
	ofap.component = rendComp;
	static DWORD msgHash_AddObjectToFactory = CHashString(_T("AddObjectToFactory")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_AddObjectToFactory, sizeof(OFACTORYADDPARAMS), &ofap);

	IObject * RendObj = (IObject * )rendComp;
	IArchive *MemArchive;
	CHashString memType(_T("Memory"));

	CREATEARCHIVE caOut;
	caOut.mode = STREAM_MODE_WRITE | STREAM_MODE_READ;
	caOut.streamData = NULL;
	caOut.streamSize = 0;
	caOut.streamType = &memType;
	static DWORD msgHash_CreateArchive = CHashString(_T("CreateArchive")).GetUniqueID();
	if (m_ToolBox->SendMessage(msgHash_CreateArchive, sizeof(CREATEARCHIVE), &caOut) != MSG_HANDLED)
	{
		return;
	}
	Vec3 Position;
	EulerAngle Rotation;
	Vec3 Scaling;
	if( m_RootController )
	{
		node->GetScaling( Scaling );
		node->GetRotation( Rotation );
		node->GetTranslation( Position );
	}
	MemArchive = caOut.archive;
	MemArchive->Write( node->GetFilePath(), _T("FileName") );
	UINT iParentIndex = node->GetParentIndex();
	if( !m_HierarchicalNodeData[iParentIndex].m_bInitialized )
	{
		m_ToolBox->Log( LOGFATALERROR, _T("CHierarchicalModel: Failed to Initialize Cloth Object (Invalid Parent)!\n") );
		return;
	}
    MemArchive->Write( m_HierarchicalNodeData[iParentIndex].m_RenderObject.GetString() );
    MemArchive->Write( node->GetName() );
	
	MemArchive->SetIsWriting(false);
	MemArchive->SeekTo(0);

	RendObj->Serialize( *MemArchive );
	RendObj->Init();

	MemArchive->Close();
}

DWORD CHierarchicalModel::OnSetChildPositionOffset( DWORD size, void * params )
{
	VERIFY_MESSAGE_SIZE( size, sizeof( HIERARCHICALOFFSETPARAM ) );
	HIERARCHICALOFFSETPARAM * p = (HIERARCHICALOFFSETPARAM*)params;
	if( p &&
		p->m_Offset &&
		IsValidChild( p->m_Child ) )
	{
		m_HierarchicalNodeData[ p->m_Child ].SetOffsetTranslation( *p->m_Offset );
	}
	SetDirtyTransformFlag();

	return MSG_HANDLED_STOP;
}

DWORD CHierarchicalModel::OnSetChildRotationOffset( DWORD size, void * params )
{
	VERIFY_MESSAGE_SIZE( size, sizeof( HIERARCHICALOFFSETPARAM ) );
	HIERARCHICALOFFSETPARAM * p = (HIERARCHICALOFFSETPARAM*)params;
	if( p &&
		p->m_RotationOffset &&
		IsValidChild( p->m_Child ) )
	{
		m_HierarchicalNodeData[ p->m_Child ].SetOffsetRotation( (*p->m_RotationOffset) );
	}
	SetDirtyTransformFlag();
	return MSG_HANDLED_STOP;
}

DWORD CHierarchicalModel::OnSetChildScaleOffset( DWORD size, void * params )
{
	VERIFY_MESSAGE_SIZE( size, sizeof( HIERARCHICALOFFSETPARAM ) );
	HIERARCHICALOFFSETPARAM * p = (HIERARCHICALOFFSETPARAM*)params;
	if( p &&
		p->m_Offset &&
		IsValidChild( p->m_Child ) )
	{
		m_HierarchicalNodeData[ p->m_Child ].SetOffsetScale( *p->m_Offset );
	}
	SetDirtyTransformFlag();
	return MSG_HANDLED_STOP;
}

DWORD CHierarchicalModel::OnGetChildPositionOffset( DWORD size, void * params )
{
	VERIFY_MESSAGE_SIZE( size, sizeof( HIERARCHICALOFFSETPARAM ) );
	HIERARCHICALOFFSETPARAM * p = (HIERARCHICALOFFSETPARAM*)params;
	if( p &&
		p->m_Offset &&
		IsValidChild( p->m_Child ) )
	{
		*p->m_Offset = m_HierarchicalNodeData[ p->m_Child ].GetOffsetTranslation( );
	}
	return MSG_HANDLED_STOP;
}
DWORD CHierarchicalModel::OnGetChildRotationOffset( DWORD size, void * params )
{
	VERIFY_MESSAGE_SIZE( size, sizeof( HIERARCHICALOFFSETPARAM ) );
	HIERARCHICALOFFSETPARAM * p = (HIERARCHICALOFFSETPARAM*)params;
	if( p &&
		p->m_RotationOffset &&
		IsValidChild( p->m_Child ) )
	{
		*p->m_RotationOffset = m_HierarchicalNodeData[ p->m_Child ].GetOffsetRotation( );
	}
	return MSG_HANDLED_STOP;
}
DWORD CHierarchicalModel::OnGetChildScaleOffset( DWORD size, void * params )
{
	VERIFY_MESSAGE_SIZE( size, sizeof( HIERARCHICALOFFSETPARAM ) );
	HIERARCHICALOFFSETPARAM * p = (HIERARCHICALOFFSETPARAM*)params;
	if( p &&
		p->m_Offset &&
		IsValidChild( p->m_Child ) )
	{
		*p->m_Offset = m_HierarchicalNodeData[ p->m_Child ].GetOffsetScale( );
	}
	return MSG_HANDLED_STOP;
}
DWORD CHierarchicalModel::OnGetChildID( DWORD size, void * params )
{
	VERIFY_MESSAGE_SIZE( size, sizeof( HIERARCHICALMODELGETCHILDID ) );
	HIERARCHICALMODELGETCHILDID * p = (HIERARCHICALMODELGETCHILDID*)params;
	if( p )
	{
		if (m_pHierType)
		{
			p->m_RetID = m_pHierType->GetChildID(p->m_Childname);
		}
	}
	return MSG_HANDLED_STOP;
}

DWORD CHierarchicalModel::OnGetChildGlobalRotation( DWORD size, void * params )
{
	CheckDirtyTransform();
	VERIFY_MESSAGE_SIZE( size, sizeof( HIERARCHICALOFFSETPARAM ) );
	HIERARCHICALOFFSETPARAM * p = (HIERARCHICALOFFSETPARAM*)params;
	if( p &&
		p->m_RotationOffset &&
		IsValidChild( p->m_Child ) )
	{
		//*p->m_Offset = m_GlobalTransforms[ p->m_Child ].m_Rotation*RADTODEG;
		MathUtil math;
		Matrix3x3 matrot;
		matrot.SetFrom4x4( m_HierarchicalNodeData[ p->m_Child ].m_GlobalMatrix.GetMatrix() );
		math.MatrixToEuler( matrot, (*p->m_RotationOffset) );
	}
	return MSG_HANDLED_STOP;
}

DWORD CHierarchicalModel::OnGetChildGlobalPosition( DWORD size, void * params )
{
	CheckDirtyTransform();
	VERIFY_MESSAGE_SIZE( size, sizeof( HIERARCHICALOFFSETPARAM ) );
	HIERARCHICALOFFSETPARAM * p = (HIERARCHICALOFFSETPARAM*)params;
	if( p &&
		p->m_Offset &&
		IsValidChild( p->m_Child ) )
	{
		Vec3 * v = p->m_Offset;
		//*p->m_Offset = m_GlobalTransforms[ p->m_Child ].m_Translation;
		//have to  rebuild mats
		//grab position
		v->x = m_HierarchicalNodeData[ p->m_Child ].m_GlobalMatrix.m[ 12 ];
		v->y = m_HierarchicalNodeData[ p->m_Child ].m_GlobalMatrix.m[ 13 ];
		v->z = m_HierarchicalNodeData[ p->m_Child ].m_GlobalMatrix.m[ 14 ];
	}
	return MSG_HANDLED_STOP;
}

DWORD CHierarchicalModel::OnGetChildGlobalMatrix( DWORD size, void * params )
{
	CheckDirtyTransform();
	VERIFY_MESSAGE_SIZE( size, sizeof( HIERARCHICALPOSITIONMATRIX ) );
	HIERARCHICALPOSITIONMATRIX * p = (HIERARCHICALPOSITIONMATRIX*)params;
	if( p &&
		p->m_OutMatrix &&
		IsValidChild( p->m_Child ) )
	{
		Matrix3x3 * matout = p->m_OutMatrix;
		matout->SetFrom4x4( m_HierarchicalNodeData[ p->m_Child ].m_GlobalMatrix.GetMatrix() );
	}
	return MSG_HANDLED_STOP;
}

DWORD CHierarchicalModel::OnGetChildGlobalScale( DWORD size, void * params )
{
	CheckDirtyTransform();
	VERIFY_MESSAGE_SIZE( size, sizeof( HIERARCHICALOFFSETPARAM ) );
	HIERARCHICALOFFSETPARAM * p = (HIERARCHICALOFFSETPARAM*)params;
	if( p &&
		p->m_Offset &&
		IsValidChild( p->m_Child ) )
	{
		*p->m_Offset = m_HierarchicalNodeData[ p->m_Child ].m_GlobalScale;
	}
	return MSG_HANDLED_STOP;
}

HierarchicalNode * CHierarchicalModel::GetRoot()
{
	if (m_pCachedRootNode)
		return m_pCachedRootNode;
	else
	{
		if (m_CurrentAnimation)
		{
			HNODEVECTOR::iterator iter = m_CurrentAnimation->begin();
			for( ;iter != m_CurrentAnimation->end(); ++iter)
			{		
				HierarchicalNode * node = (*iter);
				if( node->IsRoot() )
				{
					m_pCachedRootNode = node;
					return m_pCachedRootNode;
				}
			}
		}
		return NULL;
	}
}
void CHierarchicalModel::CheckDirtyTransform()
{
	if( m_bTransformIsDirty )
	//if( m_bTransformIsDirty || m_bEnablePhysicsAnimation )
	{
		if( !m_CurrentAnimation ||
			!m_CoreHierarchy ||
			!m_RootController )
			return;

		m_bTransformIsDirty = false;

		Matrix4x4* RootMatrix = m_RootController->GetMatrix();
		if( RootMatrix )
		{
			GetParentTransform( *RootMatrix );
		}
		HierarchicalNode * node = GetRoot();
		if( node )
		{
			RecurseSetupTransforms( node );			
		}
	}
}

void CHierarchicalModel::GetParentTransform( Matrix4x4& transform )
{
	static DWORD msgHash_GetGlobalTransform = CHashString(_T("GetGlobalTransform")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_GetGlobalTransform, sizeof(Matrix4x4), &transform, GetParentName() );
}

DWORD CHierarchicalModel::OnGetBoundingSphere(DWORD size, void *param)
{
	VERIFY_MESSAGE_SIZE( size, sizeof( GETBOUNDINGSPHEREPARAMS ) );
	GETBOUNDINGSPHEREPARAMS * p = (GETBOUNDINGSPHEREPARAMS*)param;
	if( p && p->position)
	{
		GetBoundingSphere( *p->position, p->radius );
		return MSG_HANDLED_STOP;
	}
	return MSG_NOT_HANDLED;
}

DWORD CHierarchicalModel::OnGetGeometryAllocations( DWORD size, void * params )
{
	VERIFY_MESSAGE_SIZE( size, sizeof( GETRENDERBUFFERALLOCATIONMSG ) );
	GETRENDERBUFFERALLOCATIONMSG * parms = (GETRENDERBUFFERALLOCATIONMSG *)params;
	if( parms )
	{
		//Loop through cal objects and add allocations:
		for( int i = 0; i < (int)m_HierarchicalNodeData.size(); i++ )
		{
			//add geom allocations
			static DWORD msgHash_GetGeometryAllocations = CHashString(_T("GetGeometryAllocations")).GetUniqueID();
			m_ToolBox->SendMessage(msgHash_GetGeometryAllocations, sizeof(GETRENDERBUFFERALLOCATIONMSG),
				 params, &m_HierarchicalNodeData[ i ].m_RenderObject );
		}
		return MSG_HANDLED_STOP;
	}
	return MSG_ERROR;
}

DWORD CHierarchicalModel::OnGetGeometryAllocationByName( DWORD size, void * params )
{
	VERIFY_MESSAGE_SIZE( size, sizeof( GETRENDERBUFFERALLOCATIONBYNAMEMSG ) );
	GETRENDERBUFFERALLOCATIONBYNAMEMSG * parms = (GETRENDERBUFFERALLOCATIONBYNAMEMSG *)params;
	if( parms )
	{
		int childID = m_pHierType->GetChildID(parms->m_hszName);
		if( m_HierarchicalNodeData[childID].m_bInitialized )
		{
			CHashString tmp;
			tmp.Init( m_HierarchicalNodeData[childID].m_RenderObject.GetString() );
			static DWORD msgHash_GetGeometryAllocations = CHashString(_T("GetGeometryAllocations")).GetUniqueID();
			DWORD result = m_ToolBox->SendMessage(msgHash_GetGeometryAllocations, sizeof(GETRENDERBUFFERALLOCATIONMSG), params, &m_HierarchicalNodeData[ childID ].m_RenderObject );
			return MSG_HANDLED_STOP;
		}
	}
	return MSG_ERROR;
}

DWORD CHierarchicalModel::OnGetBoundingBox( DWORD size, void* param )
{
	VERIFY_MESSAGE_SIZE( size, sizeof(GETBOUNDINGBOXPARAMS) );
	GETBOUNDINGBOXPARAMS * p = (GETBOUNDINGBOXPARAMS*)param;
	if( p && p->position && p->extents && p->rotation )
	{
		GetBoundingBox( *p->position, *p->extents, *p->rotation );
		return MSG_HANDLED_STOP;
	}
	return MSG_NOT_HANDLED;
}


DWORD CHierarchicalModel::OnGetGlobalPosition(DWORD size, void *params)
{
	static DWORD msgHash_GetGlobalPosition = CHashString(_T("GetGlobalPosition")).GetUniqueID();
	return m_ToolBox->SendMessage(msgHash_GetGlobalPosition, size, params, GetParentName());
}
	
DWORD CHierarchicalModel::OnGetGlobalEuler(DWORD size, void *params)
{
	static DWORD msgHash_GetGlobalEuler = CHashString(_T("GetGlobalEuler")).GetUniqueID();
	return m_ToolBox->SendMessage(msgHash_GetGlobalEuler, size, params, GetParentName());
}

DWORD CHierarchicalModel::OnGetGlobalScale(DWORD size, void *params)
{
	static DWORD msgHash_GetGlobalScale = CHashString(_T("GetGlobalScale")).GetUniqueID();
	return m_ToolBox->SendMessage(msgHash_GetGlobalScale, size, params, GetParentName());
}

DWORD CHierarchicalModel::OnGetGlobalTransform(DWORD size, void *params)
{
	static DWORD msgHash_GetGlobalTransform = CHashString(_T("GetGlobalTransform")).GetUniqueID();
	return m_ToolBox->SendMessage(msgHash_GetGlobalTransform, size, params, GetParentName());
}

DWORD CHierarchicalModel::OnSetSkeletonPose( DWORD size, void* param )
{
	//get same bones and set their positions
	VERIFY_MESSAGE_SIZE( size, sizeof( SKELETONINFO ) );
	SKELETONINFO * msg = (SKELETONINFO*)param;
	if( msg )
	{
		for( int i = 0; i < (int)m_HierarchicalNodeData.size(); i++ )
		{
			m_HierarchicalNodeData[ i ].m_bPhysicsControlled = false;
		}
		int numMsgBones = (int)msg->bones.size();
		BONELIST::iterator iter = msg->bones.begin();
		for( ; iter != msg->bones.end(); ++iter )
		{
			BONEINFO &ref = (*iter);
			if( ref.bonename )
			{
				HIERARCHICALMODELGETCHILDID childID;
				childID.m_Childname = ref.bonename;
				if (m_pHierType)
				{
					childID.m_RetID = m_pHierType->GetChildID(childID.m_Childname);
				}

				// Decompose the matrix into pos/rot
				Matrix4x4* transform = ref.m_Matrix;
				Vec3 position = transform->GetTranslation();
				EulerAngle rotation = transform->GetRotation();

				
				HIERARCHICALOFFSETPARAM offset;
				offset.m_Child = childID.m_RetID;
				
				offset.m_Offset = &position;
				OnSetChildPositionOffset( sizeof(HIERARCHICALOFFSETPARAM), &offset );

				offset.m_RotationOffset = &rotation;
				OnSetChildRotationOffset( sizeof(HIERARCHICALOFFSETPARAM), &offset );

				if( offset.m_Child >= 0 &&
					offset.m_Child < (int)m_HierarchicalNodeData.size() )
				{
					m_HierarchicalNodeData[ offset.m_Child ].m_bPhysicsControlled = true;
				}else
				{
					m_ToolBox->Log( LOGERROR, _T("Hierarchical Model Could not find child to bind to physicsm child %d/%d\n"), 
						offset.m_Child, m_HierarchicalNodeData.size() );
				}
			}			
		}
	}
	return MSG_HANDLED_STOP;
}

DWORD CHierarchicalModel::OnEnablePhysicsAnimations( DWORD size, void * params )
{
	//get same bones and set their positions
	VERIFY_MESSAGE_SIZE( size, sizeof( bool ) );
	bool * msg = (bool*)params;
	if( msg )
	{
		m_bEnablePhysicsAnimation = *msg;
		m_bUseGlobalOffsetTransforms = *msg;
	}

	if( m_bUseGlobalOffsetTransforms == false )
	{
		Matrix4x4 identity;
		// reset offsets to the identity matrix
		for(int i = 0; i < (int)m_HierarchicalNodeData.size(); i++)
		{
			Vec3 identity( 0, 0, 0 );

			HIERARCHICALOFFSETPARAM offset;
			offset.m_Child = i;
			offset.m_Offset = &identity;

			OnSetChildPositionOffset( sizeof(HIERARCHICALOFFSETPARAM), &offset );
			OnSetChildRotationOffset( sizeof(HIERARCHICALOFFSETPARAM), &offset );
		}
	}

	return MSG_HANDLED_PROCEED;
}

DWORD CHierarchicalModel::OnGetGlobalBoneInfo(DWORD size, void *param)
{
	CheckDirtyTransform();

	VERIFY_MESSAGE_SIZE(size, sizeof(BONEINFO));
	BONEINFO *boneInfo = (BONEINFO*)param;

	if ((boneInfo) && (boneInfo->bonename) && (m_pHierType))
	{
		int childID = m_pHierType->GetChildID(boneInfo->bonename);

		if (IsValidChild((UINT)childID))
		{

			if (boneInfo->m_Position)
			{
				boneInfo->m_Position->x = m_HierarchicalNodeData[ childID ].m_GlobalMatrix.m[ 12 ];
				boneInfo->m_Position->y = m_HierarchicalNodeData[ childID ].m_GlobalMatrix.m[ 13 ];
				boneInfo->m_Position->z = m_HierarchicalNodeData[ childID ].m_GlobalMatrix.m[ 14 ];
			}
			
			if (boneInfo->m_Rotation)
			{
				MathUtil math;
				Matrix3x3 matrot;
				matrot.SetFrom4x4( m_HierarchicalNodeData[ childID ].m_GlobalMatrix.GetMatrix() );
				math.MatrixToEuler( matrot, (*boneInfo->m_Rotation) );
			}

			if (boneInfo->m_Matrix)
			{
				boneInfo->m_Matrix->SetFrom4x4( m_HierarchicalNodeData[ childID ].m_GlobalMatrix.GetMatrix() );		
			}	
			return MSG_HANDLED_STOP;
		}
	}

	return MSG_NOT_HANDLED;
}

DWORD CHierarchicalModel::OnGetOffsetBoneInfo(DWORD size, void *param)
{
	CheckDirtyTransform();
	VERIFY_MESSAGE_SIZE(size, sizeof(BONEINFO));
	BONEINFO *boneInfo = (BONEINFO*)param;

	if ((boneInfo) && (boneInfo->bonename) && (m_pHierType))
	{
		int childID = m_pHierType->GetChildID(boneInfo->bonename);

		if (IsValidChild((UINT)childID))
		{
			if (boneInfo->m_Position)
			{
				*boneInfo->m_Position = m_HierarchicalNodeData[ childID ].GetOffsetTranslation( );
			}
			
			if (boneInfo->m_Rotation)
			{
				*boneInfo->m_Rotation = m_HierarchicalNodeData[ childID ].GetOffsetRotation( );
			}

			if (boneInfo->m_Matrix)
			{
				// offset matrix not implemented
			}	
			return MSG_HANDLED_STOP;
		}
	}

	return MSG_NOT_HANDLED;
}

DWORD CHierarchicalModel::OnSetOffsetBoneInfo(DWORD size, void *param)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(BONEINFO));
	BONEINFO *boneInfo = (BONEINFO*)param;

	if ((boneInfo) && (boneInfo->bonename) && (m_pHierType))
	{
		int childID = m_pHierType->GetChildID(boneInfo->bonename);

		if (IsValidChild((UINT)childID))
		{
			if (boneInfo->m_Position)
			{
				m_HierarchicalNodeData[ childID ].SetOffsetTranslation( *boneInfo->m_Position );		
			}
			
			if (boneInfo->m_Rotation)
			{
				m_HierarchicalNodeData[ childID ].SetOffsetRotation( (*boneInfo->m_Rotation) );
			}

			if (boneInfo->m_Matrix)
			{
				// offset matrix not implemented
			}	
			SetDirtyTransformFlag();
			return MSG_HANDLED_STOP;
		}
	}

    return MSG_NOT_HANDLED;
}

DWORD CHierarchicalModel::OnSetGlobalBoneInfo(DWORD size, void *param)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(BONEINFO));
	BONEINFO *boneInfo = (BONEINFO*)param;

	if ((boneInfo) && (boneInfo->bonename) && (m_pHierType))
	{
		int childID = m_pHierType->GetChildID(boneInfo->bonename);
		if (IsValidChild((UINT)childID))
		{
			if (boneInfo->m_Position)
			{
				m_HierarchicalNodeData[ childID ].m_GlobalTranslation = *boneInfo->m_Position;

//				m_HierarchicalNodeData[ childID ].SetOffsetTranslation( m.GetTranslation());
			}

			if (boneInfo->m_Rotation)
			{
				//EulerAngle diff = (*boneInfo->m_Rotation - m_HierarchicalNodeData[ childID ].m_GlobalRotation);
				m_HierarchicalNodeData[ childID ].m_GlobalRotation = *boneInfo->m_Rotation;

				//EulerAngle offsetRotation;
				//offsetRotation = m_HierarchicalNodeData[ childID ].GetOffsetRotation( );

				//m_HierarchicalNodeData[ childID ].SetOffsetRotation( diff + offsetRotation );
			}

			if (boneInfo->m_Matrix)
			{
				// offset matrix not implemented
			}	
			SetDirtyTransformFlag();
			return MSG_HANDLED_STOP;
		}
	}



    return MSG_NOT_HANDLED;
}

DWORD CHierarchicalModel::OnGetParentName(DWORD size, void *param)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(IHashString*));
	IHashString **ppName = (IHashString**)param;
	*ppName = GetParentName();

	return MSG_HANDLED_STOP;
}

DWORD CHierarchicalModel::OnSetMaterialOverride(DWORD size, void *param)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(MATERIALOVERRIDEPARAMS));
	MATERIALOVERRIDEPARAMS hmop = *(MATERIALOVERRIDEPARAMS*)param;

	for (UINT i=0; i<m_HierarchicalNodeData.size(); i++)
	{
		if (hmop.hsNodeName)
		{
			if (hmop.hsNodeName->GetUniqueID() != m_HierarchicalNodeData[i].m_hsNodeName.GetUniqueID())
			{
				continue;
			}
		}
		static CHashString hsCal3DType(_T("Cal3DRenderObject"));
		static DWORD msgSetMaterialOverride = CHashString(_T("SetMaterialOverride")).GetUniqueID();
		m_ToolBox->SendMessage(msgSetMaterialOverride, sizeof(MATERIALOVERRIDEPARAMS), &hmop, &m_HierarchicalNodeData[i].m_RenderObject, &hsCal3DType);
	}	

	return MSG_HANDLED_PROCEED;
}

DWORD CHierarchicalModel::OnClearMaterialOverride(DWORD size, void *param)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(CLEARMATOVERRIDEPARAMS));
	CLEARMATOVERRIDEPARAMS cmop = *(CLEARMATOVERRIDEPARAMS*)param;

	for (UINT i=0; i<m_HierarchicalNodeData.size(); i++)
	{
		if (cmop.hsNodeName)
		{
			if (cmop.hsNodeName->GetUniqueID() != m_HierarchicalNodeData[i].m_hsNodeName.GetUniqueID())
			{
				continue;
			}
		}
		static CHashString hsCal3DType(_T("Cal3DRenderObject"));
		static DWORD msgClearMaterialOverride = CHashString(_T("ClearMaterialOverride")).GetUniqueID();
		m_ToolBox->SendMessage(msgClearMaterialOverride, sizeof(CLEARMATOVERRIDEPARAMS), &cmop, &m_HierarchicalNodeData[i].m_RenderObject, &hsCal3DType);
	}	

	return MSG_HANDLED_PROCEED;
}

DWORD CHierarchicalModel::OnAlignToSceneObject(DWORD size, void *param)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(ATTACHSCENEOBJECTPARAMS));
	ATTACHSCENEOBJECTPARAMS asop = *(ATTACHSCENEOBJECTPARAMS*)param;

	if (asop.hsAttachmentObject && asop.hsNodeAttachName )
	{
		int childID = m_pHierType->GetChildID(asop.hsNodeAttachName);
		if (IsValidChild((UINT)childID))
		{
			Matrix4x4 transform;
			GetAttachTransform(asop.hsNodeAttachName, transform);

			static DWORD msgHash_SetGlobalTransform = CHashString(_T("SetGlobalTransform")).GetUniqueID();
			static CHashString CEntityType(_T("CEntity"));
			m_ToolBox->SendMessage(msgHash_SetGlobalTransform, sizeof(Matrix4x4), &transform, asop.hsAttachmentObject, &CEntityType );

			return MSG_HANDLED_STOP;
		}
	}

	return MSG_NOT_HANDLED;
}

DWORD CHierarchicalModel::OnAttachSceneObject(DWORD size, void *param)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(ATTACHSCENEOBJECTPARAMS));
	ATTACHSCENEOBJECTPARAMS asop = *(ATTACHSCENEOBJECTPARAMS*)param;

	if (asop.hsAttachmentObject)
	{
		// if a node is NOT specified, hrc doesn't handle it
		// otherwise...
		if (asop.hsNodeAttachName != NULL)
		{
			int childID = m_pHierType->GetChildID(asop.hsNodeAttachName);
			if (IsValidChild((UINT)childID))
			{
				static DWORD msgRegisterSceneAttachObjects = CHashString(_T("RegisterSceneAttachObjects")).GetUniqueID();
				asop.hsBaseObject = GetName();
				m_ToolBox->SendMessage(msgRegisterSceneAttachObjects, sizeof(ATTACHSCENEOBJECTPARAMS), &asop);
				return MSG_HANDLED_STOP;
			}
		}
	}

	return MSG_NOT_HANDLED;
}

void CHierarchicalModel::GetAttachTransform(IHashString *hsNodeName, Matrix4x4 &attachXForm)
{
	CheckDirtyTransform();
	int childID = m_pHierType->GetChildID(hsNodeName);
	if (IsValidChild((UINT)childID))
	{
		attachXForm.SetFrom4x4((m_HierarchicalNodeData[ childID ].m_GlobalMatrix.GetMatrix()));			
	}
}