///==========================================================================
/// \file	Cal3DRenderObject.hpp
/// \brief	Implementation of Cal3DRenderObject interface
/// \date	3/09/2005
/// \author Marvin Gouw
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

#ifndef CAL_16BIT_INDICES
#define CAL_16BIT_INDICES //make sure we use 16 bit indices!!!
#endif

#include "StdAfx.h"
#include "Cal3DRenderObject.h"
#include "Cal3DModelManager.h"
#include "IEffect.h"
#include "IGeometry.h"
#include "../../include/ObjectCubeMap/ObjectCubemapDefines.h"

#include "PerformanceProfiler.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define RADTODEG 180.f/3.141592654f
#define BONEFLOATSPERMESH	MAXBONESPERMESH*12
REGISTER_COMPONENT(Cal3DRenderObject);

REGISTER_MESSAGE_HANDLER(SetCalScaleVariable, OnSetCalScaleVariable, Cal3DRenderObject );

REGISTER_MESSAGE_HANDLER(GetModelFileId, OnGetModelFileID, Cal3DRenderObject );

REGISTER_MESSAGE_HANDLER( GetControllerInterface, OnGetControllerInterface , Cal3DRenderObject );
REGISTER_MESSAGE_HANDLER( GetAnimationList, OnGetAnimationList , Cal3DRenderObject );
REGISTER_MESSAGE_HANDLER( PlayAnimation, OnPlayAnimation , Cal3DRenderObject );
REGISTER_MESSAGE_HANDLER( PlaySingleCycleAnimation, OnPlaySingleCycleAnimation, Cal3DRenderObject);
REGISTER_MESSAGE_HANDLER( StopAnimation, OnStopAnimation , Cal3DRenderObject );
REGISTER_MESSAGE_HANDLER( StopAllAnimation, OnStopAllAnimation, Cal3DRenderObject );
REGISTER_MESSAGE_HANDLER( StopAllAnimationActions, OnStopAllAnimationActions, Cal3DRenderObject );
REGISTER_MESSAGE_HANDLER( SetVisibility, OnSetVisibility, Cal3DRenderObject );
//REGISTER_MESSAGE_HANDLER( SetCallbackEvent, OnSetCallbackEvent, Cal3DRenderObject );
REGISTER_MESSAGE_HANDLER( SetMatrix, OnSetMatrix, Cal3DRenderObject );
REGISTER_MESSAGE_HANDLER( GetBoundingSphere,  OnGetBoundingSphere, Cal3DRenderObject );
REGISTER_MESSAGE_HANDLER( RegisterAnimationCallback,  OnRegisterAnimationCallback, Cal3DRenderObject );
REGISTER_MESSAGE_HANDLER( UnregisterAnimationCallback,  OnUnregisterAnimationCallback, Cal3DRenderObject );
REGISTER_MESSAGE_HANDLER( UnregisterAllAnimationCallbacks,  OnUnregisterAllAnimationCallbacks, Cal3DRenderObject );
REGISTER_MESSAGE_HANDLER( GetAnimationId,  OnGetAnimationId, Cal3DRenderObject );
REGISTER_MESSAGE_HANDLER( GetGlobalBoneInfo,  GetBoneInfo, Cal3DRenderObject );
REGISTER_MESSAGE_HANDLER( RemoveFromQuadTree, OnRemoveFromQuadTree, Cal3DRenderObject );
REGISTER_MESSAGE_HANDLER( AddToQuadTree, OnAddToQuadTree, Cal3DRenderObject );
REGISTER_MESSAGE_HANDLER( GetModelFileName, OnGetModelFileName, Cal3DRenderObject );
REGISTER_MESSAGE_HANDLER( GetGeometryAllocations, OnGetGeometryAllocations, Cal3DRenderObject );
REGISTER_MESSAGE_HANDLER( GetBoundingBox, OnGetBoundingBox, Cal3DRenderObject );
REGISTER_MESSAGE_HANDLER( SetLightmapData, OnSetLightmapData, Cal3DRenderObject );
REGISTER_MESSAGE_HANDLER( GetParentName, OnGetParentName, Cal3DRenderObject );
REGISTER_MESSAGE_HANDLER( SetSkeletonPose, OnSetSkeletonPose, Cal3DRenderObject );
REGISTER_MESSAGE_HANDLER( GetSkeletonPose, OnGetSkeletonPose, Cal3DRenderObject );
REGISTER_MESSAGE_HANDLER( EnablePhysicsAnimations, OnEnablePhysicsAnimations, Cal3DRenderObject );
REGISTER_MESSAGE_HANDLER( GetRenderObjectInterface, OnGetRenderObjectInterface, Cal3DRenderObject );
REGISTER_MESSAGE_HANDLER( SetDrawSkeleton, OnSetDrawSkeleton, Cal3DRenderObject );
REGISTER_MESSAGE_HANDLER( SetMaterialOverride, OnSetMaterialOverride, Cal3DRenderObject );
REGISTER_MESSAGE_HANDLER( ClearMaterialOverride, OnClearMaterialOverride, Cal3DRenderObject );
REGISTER_MESSAGE_HANDLER( SetRenderOffset, OnSetRenderOffset, Cal3DRenderObject );
REGISTER_MESSAGE_HANDLER( GetRenderOffset, OnGetRenderOffset, Cal3DRenderObject );
REGISTER_MESSAGE_HANDLER( PauseAnimations, OnPauseAnimations, Cal3DRenderObject );

REGISTER_MESSAGE_HANDLER( AttachSceneObject, OnAttachSceneObject, Cal3DRenderObject );

static CHashString hsGameTime(GAMEDELTATIMER);

float Cal3DRenderObject::m_BoundingTestThreshold = 2000;
///
/// Constructor / Destructor
///

Cal3DRenderObject::Cal3DRenderObject(IHashString *parentName, IHashString *name, bool bAddToHier) : 
	SCENEOBJECTTEMPLATE(Cal3DModelManager, Cal3DRenderObject, IRenderObject, parentName, name)
{
	m_pModelManager = (Cal3DModelManager*)m_Manager;

	GETRENDERERINTERFACEPARAMS renderInterfaceParam;
	static DWORD msgHash_GetRendererInterface = CHashString(_T("GetRendererInterface")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_GetRendererInterface, sizeof(GETRENDERERINTERFACEPARAMS), &renderInterfaceParam, NULL, NULL );
	m_IRenderer = renderInterfaceParam.m_RendererInterface;

	m_bPaused = false;
	m_CachedPosition.Set( 0,0,0 );
	m_CachedRadius = 0;
	m_calModel = NULL;
	m_bIsVisible = true;
	//m_Geometry = NULL;
	m_calHardwareModel = NULL;
	m_bEnablePhysicsAnimation = false;
	
	m_bSkinnedModel = false;
	m_NumLights = 0;
	for (int i = 0; i < MAX_LIGHTS; i++)
	{
		m_LightPriorities[ i ] = -99999.f;//this will essentially reset priorities quickly
	}
	m_SceneSettings = NULL;
	m_GeometryController = NULL;

	SerializedPosition.Set( 0, 0, 0 );
	SerializedScaling.Set( 1, 1, 1 );
	SerializedRotation.Set( 0, 0, 0 );

	m_RenderOffset = Vec3(0,0,0);
	m_PrevRenderOffset = Vec3(0,0,0);
	m_pRenderOffsetBone = NULL;
	m_PrevFrameRenderOffset.Set( 0,0,0 );
	m_DeltaRenderOffset.Set( 0,0,0 );

	m_fObjectAlpha = 1.0f;

	m_fElapsedTime = 0.0f;
	m_LastUpdateTick = 0.0f;

	m_bAddToHierarchy = bAddToHier;
	if (bAddToHier)
	{
		AddToHierarchy();
	}
	m_bCallbacksCalled = false;
	DeInit();	
}

Cal3DRenderObject::~Cal3DRenderObject()
{
	//remove from quad tree or any other manager that uses updateboundingbox params
	RemoveBoundingObject();

	static DWORD msgHash_ParentRemoveBoundingBox = CHashString(_T("ParentRemoveBoundingBox")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_ParentRemoveBoundingBox, 0, NULL, &m_ParentName);
	
	DeInit();	
	if (m_bAddToHierarchy)
	{
		RemoveFromHierarchy();
	}
}

/// \brief The Initialization for the Object
/// \return pointer to an IComponent
IComponent *Cal3DRenderObject::Create(int nArgs, va_list argptr)
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
		if (nArgs == 2)
		{
			self = new Cal3DRenderObject(parentName, name, true);
		}
		else if (nArgs == 3)
		{
			bAddToHier = va_arg(argptr, bool);
			self = new Cal3DRenderObject(parentName, name, bAddToHier);
		}
	}
	catch(...)
	{
		return NULL;
	}

	return self;
}

void Cal3DRenderObject::LoadCalHardwareModel()
{
	m_calHardwareVertexCount = 0;
	m_calHardwareIndexCount = 0;

	// create geometry for nonskinned mesh
	if( !m_bSkinnedModel )
	{
		CalCoreModel *coreModel = m_calModel->getCoreModel();
		for(int meshId = 0; meshId < coreModel->getCoreMeshCount(); meshId++)
		{
			CalCoreMesh *pCoreMesh = coreModel->getCoreMesh(meshId);
			CalMesh * pMesh = m_calModel->getMesh(meshId );
			int submeshCount= pCoreMesh->getCoreSubmeshCount();
			int submeshId;
			//create the geometry for this mesh
			for(submeshId = 0 ;submeshId < submeshCount ; submeshId++)
			{   
				CalCoreSubmesh *pCoreSubmesh = pCoreMesh->getCoreSubmesh(submeshId);
				CalSubmesh * pSubmesh = pMesh->getSubmesh( submeshId );

				int startIndex=m_calHardwareIndexCount;
				int NumVertices, NumIndices;
				
				//new geom
				NumVertices = pCoreSubmesh->getVertexCount();
				NumIndices = pCoreSubmesh->getFaceCount()*3;
				//for this submesh
				IGeometry * SubmeshGeometry = NULL;					
				static DWORD msgHash_CreateGeometry = CHashString(_T("CreateGeometry")).GetUniqueID();
				m_ToolBox->SendMessage(msgHash_CreateGeometry, sizeof( IGeometry** ), &SubmeshGeometry );
				if( !SubmeshGeometry )
				{
					m_ToolBox->Log( LOGERROR, _T("Cal3D could not create geometry instance, may be out of memory."));
					return;
				}
				SubmeshGeometry->SetController( (IController*)m_GeometryController );
				SubmeshGeometry->SetSceneSettings( m_SceneSettings );
				m_Geometry.push_back( (IGeometry*)SubmeshGeometry );
				//create new allocation sub unit with temp storage of vertex size and offsets
				BUFFERALLOCATIONSTRUCT tempalloc;
				tempalloc.m_Offset = m_calHardwareVertexCount;//current count
				tempalloc.m_Size = NumVertices;
				if( m_bSkinnedModel )
					tempalloc.VertexFormat = m_pModelManager->VertexFormat;
				else
					tempalloc.VertexFormat = m_pModelManager->VertexFormatNoSkin;
				SubmeshGeometry->SetVertexBufferAllocation( tempalloc );
				tempalloc.m_Offset = m_calHardwareIndexCount;//current count
				tempalloc.m_Size = NumIndices;
				SubmeshGeometry->SetIndexBufferAllocation( tempalloc );
				m_calHardwareVertexCount += NumVertices;
				m_calHardwareIndexCount += NumIndices;
				//Set the texture 
				// get the core material
				int mapId = pCoreSubmesh->getCoreMaterialThreadId();
				if( mapId < 0 ){ mapId = 0; }
				// check if the map id is valid
				if((mapId >= 0) && (mapId < (int)m_Materials.size()))
				{
					IMaterial * mat = m_Materials[ mapId ];
					SubmeshGeometry->SetMaterial( mat );
					if( mat && m_SceneSettings )
					{
						SHADER_RENDER_FLAG matFlags = mat->GetShaderFlags();
						m_SceneSettings->AddShaderFlag( matFlags );
					}
				}		
			}
		}
	}
}

bool Cal3DRenderObject::AllocateHardwareVertices()
{
	if( m_calHardwareModel )
	{
		{
			std::vector< CalHardwareModel::CalHardwareMesh > &hmeshes = m_calHardwareModel->getVectorHardwareMesh();
			CalCoreModel * pCoreModel = m_calModel->getCoreModel();
			
			int vertexoffset = 0;
	 		int IndexOffset = 0;
			int hardwareMeshId;		
			for(hardwareMeshId = 0; 
				hardwareMeshId < m_calHardwareModel->getHardwareMeshCount();
				hardwareMeshId++ )
				{
					m_calHardwareModel->selectHardwareMesh(hardwareMeshId);	
					//for this submesh
					IGeometry * SubmeshGeometry = NULL;					
					if( m_bSkinnedModel )
					{
						static DWORD msgHash_CreateGeometry = CHashString(_T("CreateGeometry")).GetUniqueID();
						m_ToolBox->SendMessage(msgHash_CreateGeometry, sizeof( IGeometry** ), &SubmeshGeometry );
						if( !SubmeshGeometry )
						{
							m_ToolBox->Log( LOGERROR, _T("Cal3D could not create geometry instance, may be out of memory."));
							return false;
						}
						SubmeshGeometry->SetController( (IController*)m_GeometryController );
					}else
					{
						SubmeshGeometry = m_Geometry[ hardwareMeshId ];
						if( !SubmeshGeometry )
						{
							continue;
						}
					}

					//init ib and vb allocation offsets
					BUFFERALLOCATIONSTRUCT tempalloc;
					SubmeshGeometry->GetVertexBufferAllocation( tempalloc );
					int VBsize = hmeshes[ hardwareMeshId ].vertexCount;
					tempalloc.m_Offset = vertexoffset;
					tempalloc.m_Size = VBsize;
					SubmeshGeometry->SetVertexBufferAllocation( tempalloc );
					SubmeshGeometry->GetIndexBufferAllocation( tempalloc );
					int IBsize = hmeshes[ hardwareMeshId ].faceCount * 3;		
					tempalloc.m_Offset = IndexOffset;
	 				tempalloc.m_Size = IBsize;
					SubmeshGeometry->SetIndexBufferAllocation( tempalloc );
					vertexoffset += VBsize;
 					IndexOffset += IBsize;
					
					//save the geometry
					if( m_bSkinnedModel )
					{
						m_Geometry.push_back( (IGeometry*)SubmeshGeometry );
						//load materials
						int submeshId = hmeshes[ hardwareMeshId ].submeshId;
						int meshId = hmeshes[ hardwareMeshId ].meshId;
						CalCoreMesh * pCoreMesh = pCoreModel->getCoreMesh( meshId );
						if( pCoreMesh )
						{
							CalCoreSubmesh *pCoreSubmesh = pCoreMesh->getCoreSubmesh(submeshId);
							if( pCoreSubmesh )
							{
								int mapId = pCoreSubmesh->getCoreMaterialThreadId();
								if( mapId < 0 ){ mapId = 0; }
								// check if the map id is valid
								if((mapId >= 0) && (mapId < (int)m_Materials.size()))
								{
									IMaterial * mat = m_Materials[ mapId ];
									SubmeshGeometry->SetMaterial( mat );
									if( mat && m_SceneSettings )
									{
										SHADER_RENDER_FLAG matFlags = mat->GetShaderFlags();
										m_SceneSettings->AddShaderFlag( matFlags );
									}
								}		
							}
						}
					}
				}
			m_calHardwareVertexCount = vertexoffset;
			m_calHardwareIndexCount = IndexOffset;				
		}
	}
	return true;
}

void Cal3DRenderObject::Init()
{
	m_bDebugShowSkeleton = false;
	m_Materials.clear();
	m_calModel = NULL;

	static DWORD msgHash_CreateController = CHashString(_T("CreateController")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_CreateController, sizeof( IController** ), &m_GeometryController );
	if( !m_GeometryController )
	{
		m_ToolBox->Log( LOGERROR, _T("Could not create Cal3D geometry controller!\n") );
		return;
	}

	m_GeometryController->SetTranslationOffset( SerializedPosition );
	m_GeometryController->SetRotationOffset( SerializedRotation );
	m_GeometryController->SetScaling( SerializedScaling );
	m_GeometryController->BuildRotation();

	CCal3DCoreModelCache *pModelCache = m_pModelManager->GetCoreModelCache( &m_FileHash );
	if (pModelCache && pModelCache->m_pCoreModel)
	{
		m_calModel = new CalModel( pModelCache->m_pCoreModel );
		m_calModel->setUserData( this );
		m_calModel->setMaterialSet(0); // set the material set of the whole model
		m_calModel->disableInternalData();
/*
		// BNB:1-5-08 - this shouldn't be needed because the coresubmesh contains correct tangents
		// that are generated upon caching the coremodel.  Here for posterity in case it is 
		// indeed needed.
		int meshCount = pModelCache->m_pCoreModel->getCoreMeshCount();
		for (int meshId=0; meshId < meshCount; meshId++)
		{
			CalMesh *pMesh = m_calModel->getMesh( meshId );
			if (pMesh)
			{
				int submeshCount = pMesh->getSubmeshCount();
				for (int submeshId=0; submeshId < submeshCount; submeshId++)
				{
					CalSubmesh *pSubmesh = pMesh->getSubmesh(submeshId);
					pSubmesh->enableTangents(0, true);
				}
			}
		}
*/
		for (int meshId = 0; meshId < pModelCache->m_pCoreModel->getCoreMeshCount(); meshId++)
		{
			m_calModel->attachMesh(meshId);
		}
		vector<IMaterial*> *pMaterials = m_pModelManager->GetModelMaterials( &m_FileHash );
		if (pMaterials)
			m_Materials = (*pMaterials);

		m_calHardwareModel = pModelCache->m_pCalHardwareModel;

		m_IndexBufferAllocation = pModelCache->m_IndexBufferAllocation;
		m_VertexBufferAllocation = pModelCache->m_VertexBufferAllocation;
		m_IndexBufferInterface = pModelCache->m_pIndexBufferInterface;
		m_VertexBufferInterface = pModelCache->m_pVertexBufferInterface;

		m_bSkinnedModel = (m_calModel->getSkeleton() != NULL);
	}

	if( m_calModel )
	{
		m_SceneSettings = NULL;
		static DWORD msgHash_CreateSceneSettings = CHashString(_T("CreateSceneSettings")).GetUniqueID();
		m_ToolBox->SendMessage(msgHash_CreateSceneSettings, sizeof( ISceneSettings ), &m_SceneSettings );
		m_SceneSettings->SetShaderFlags( 0 );
		if( m_bSkinnedModel )
		{
			m_SceneSettings->AddShaderFlag( BONES_1 );
		}

		LoadCalHardwareModel();

		int sizeVertex;
		if( m_bSkinnedModel )
		{
			sizeVertex = sizeof( VERTEX);
		}
		else
		{
			sizeVertex = sizeof( VERTEX_NOSKIN );
		}

		AllocateHardwareVertices();

		/////////////////////////////////////////////////////////////////////////////////////////////
		//have to create subgeometry for cal models. each geom has their own material.
		int hardwareMeshId;		
		for(hardwareMeshId = 0; 
			hardwareMeshId < m_calHardwareModel->getHardwareMeshCount();
			hardwareMeshId++ )
		{
			m_calHardwareModel->selectHardwareMesh(hardwareMeshId);
				
			//finalize the vertex buffer and index buffer handles
			IGeometry * ChildGeometry = m_Geometry[ hardwareMeshId ];
			if( ChildGeometry )
			{
				//do the proper allocation for the geometry
				BUFFERALLOCATIONSTRUCT tempalloc;
				ChildGeometry->GetVertexBufferAllocation( tempalloc );
				tempalloc.m_Stride = sizeVertex;
				//tempalloc.m_Offset = m_VertexBufferAllocation.m_Offset + tempalloc.m_Offset;
				tempalloc.m_Offset  = m_VertexBufferAllocation.m_Offset; //base vertex offset
				tempalloc.m_Size = m_calHardwareVertexCount;
				tempalloc.m_InterfaceHandle = m_VertexBufferAllocation.m_InterfaceHandle;
				tempalloc.m_AllocationHandle = m_VertexBufferAllocation.m_AllocationHandle;
				//vertex size and offset should be current
				ChildGeometry->SetVertexBufferAllocation( tempalloc );
				ChildGeometry->GetIndexBufferAllocation( tempalloc );
				tempalloc.m_Stride = sizeof( CalIndex );
				tempalloc.m_Offset = m_IndexBufferAllocation.m_Offset + tempalloc.m_Offset;
				tempalloc.m_InterfaceHandle = m_IndexBufferAllocation.m_InterfaceHandle;
				tempalloc.m_AllocationHandle = m_IndexBufferAllocation.m_AllocationHandle;
				//vertex size and offset should be current
				ChildGeometry->SetIndexBufferAllocation( tempalloc );
			}
		}
	}	

	m_BoneIdToHashMap.clear();
	BuildBoneNameLinks();

	// run a tick to update internal bounding box
	if (m_calModel && m_bSkinnedModel)
		m_calModel->update( 0 );

	// update the render object position to any manager that needs to know when it moves, i.e. the quad tree
	// manager
	UpdateBoundingObject();
}

void Cal3DRenderObject::BuildBoneNameLinks()
{
	//Immediate rendering of self for skinned models because bones have to be set up
	if( m_bSkinnedModel &&
		m_calModel )
	{
		CalSkeleton * pSkeleton = m_calModel->getSkeleton();
		if( pSkeleton )
		{
			CalCoreSkeleton * pCoreSkeleton = pSkeleton->getCoreSkeleton();
			if( pCoreSkeleton )
			{
				vector< CalBone * > &bones = pSkeleton->getVectorBone();
				for( int i = 0; i < (int)bones.size(); i++ )
				{
					CalBone * bone = bones[ i ];
					if( bone )
					{
						CalCoreBone * coreBone = bone->getCoreBone();
						if( coreBone )
						{
							string boneName = coreBone->getName();
							int boneId = pCoreSkeleton->getCoreBoneId( boneName );
							CHashString hashName( (const TCHAR*)boneName.c_str() );
							//boneId should equal i, but just in case
							DWORD hashValue = hashName.GetUniqueID();
							m_BoneIdToHashMap[ boneId ] = hashName;

							Matrix4x4 transformation;
							CalQuaternion cq = bone->getRotationBoneSpace();
							Quaternion q(cq.x, cq.y, cq.z, cq.w );
							q.GetMatrix( transformation );

							CalVector translationBoneSpace = bone->getTranslationBoneSpace();
							
							//Temp workaround for differing coordinate systems in max and this engine
							//the negation of the x axis solves the mirroring effects of models exported from max
							//into our engine. Until we can standardize a coordinate system, at that point,
							//models should be exported from max in our coordinate system so these matrices
							//should need no inversion
							//inverse rotation around x
							transformation.m[12]=	translationBoneSpace.x;
							transformation.m[13]=	translationBoneSpace.y;
							transformation.m[14]=	translationBoneSpace.z;
							transformation.SetTranspose();

							m_WorldBoneMatrices[ hashValue ] = transformation;
						}
					}
				}
			}
		}
	}
}

void Cal3DRenderObject::GetParentTransform( Matrix4x4& transform )
{
	Matrix4x4 parentTransform;
	Vec3 parentScale;

	static DWORD msgHash_GetGlobalTransform = CHashString(_T("GetGlobalTransform")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_GetGlobalTransform, sizeof(Matrix4x4), &parentTransform, GetParentName() );
	static DWORD msgHash_GetGlobalScale = CHashString(_T("GetGlobalScale")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_GetGlobalScale, sizeof(Vec3), &parentScale, GetParentName());

	if( m_GeometryController )
	{
		m_GeometryController->SetMatrix( &parentTransform );
		m_GeometryController->SetScaling(parentScale);
	}
}
   

/// \brief Update the object
/// \return true or false
bool Cal3DRenderObject::Update()
{
	m_bCallbacksCalled = false;
	if( m_bTransformIsDirty &&
		m_GeometryController )
 	{
		Matrix4x4 * controllerMatrix = m_GeometryController->GetMatrix();
		if( controllerMatrix )
		{
 			GetParentTransform( *controllerMatrix );
		}
		UpdateBoundingSphere();
 		m_bTransformIsDirty = false;
		// update the render object position to any manager that needs to know when it moves, i.e. the quad tree
		// manager
		UpdateBoundingObject();
 	}
 
	float fTimeDelta = m_Timer->GetTimeDelta( &hsGameTime );

	if( m_bSkinnedModel &&
		m_calModel &&
		m_calModel->getCoreModel())		
	{
		if (m_bPaused == false)
		{
			if (m_LastUpdateTick < m_Timer->GetTime())
			{
				m_LastUpdateTick = m_Timer->GetTime();

				m_calModel->update( fTimeDelta );				
				UpdateRenderOffset();
			}
		}
		else
		{
			UpdateRenderOffset();
		}
		// be sure to update attached objects!
		SetAttachmentObjectsDirty();
	}

	m_fElapsedTime += fTimeDelta;
	if (m_SceneSettings)
	{
		m_SceneSettings->SetElapsedTime(m_fElapsedTime);
	}

	return true;
}

/// \brief Update the render object( Render )
/// \return true or false
bool Cal3DRenderObject::Render( UINT pass, IEffect * override )
{	
	PERFORMANCE_PROFILER_AUTO(Cal3DRenderObject_Render);
	//Immediate rendering of self for skinned models because bones have to be set up
	if( m_bSkinnedModel &&
		m_calModel && 
		!m_bDebugShowSkeleton &&
		m_bIsVisible )
	{
		//RENDER
		//gotta loop through the geometry and just do the shader sets and render
		int i = 0;
		
		int meshid = 0;

		//Set up the world matrix
		Matrix4x4 * matworld =  m_GeometryController->GetMatrix();
		static Matrix4x4 matFinal;
		if( matworld )
		{
			matFinal = (*matworld);
			Vec3 offsetTranslation = matFinal * m_RenderOffset;
			matFinal.SetTranslation( offsetTranslation );
			m_IRenderer->SetMatrix( WORLD_MATRIX, (const float*)matFinal.GetMatrix() );
		}

		for(;meshid < (int)m_Geometry.size(); meshid++)
		{
			IGeometry* geom = m_Geometry[ meshid ];
			m_calHardwareModel->selectHardwareMesh( meshid );
			//set up vertex constants using controller matrix
			if( m_calHardwareModel && m_calHardwareModel->getBoneCount() > 0   )
			{
				static float tmBuffer[ BONEFLOATSPERMESH ];
				IMaterial * mat = geom->GetMaterial();
				int boneId;
				for(boneId = 0; boneId < m_calHardwareModel->getBoneCount() && boneId < MAXBONESPERMESH; boneId++)
				{
					static Matrix4x4 transformation;
					if( !m_bEnablePhysicsAnimation )
					{	//set the shader constant
						CalQuaternion cq = m_calHardwareModel->getRotationBoneSpace(boneId, m_calModel->getSkeleton());
						Quaternion q(cq.x, cq.y, cq.z, cq.w );
						q.GetMatrix( transformation );

						CalVector translationBoneSpace = m_calHardwareModel->getTranslationBoneSpace(boneId, m_calModel->getSkeleton());
						transformation.m[12]=translationBoneSpace.x;
						transformation.m[13]=translationBoneSpace.y;
						transformation.m[14]=translationBoneSpace.z;
						transformation.SetTranspose();
						memcpy( &tmBuffer[ boneId*12 ] , &transformation.m[0], sizeof( float )*12 );
					} //Not skeletal animation enabled, use own custom bone matrices(ragdoll for example)
					else
					{
						CHashString &hash = m_BoneIdToHashMap[ boneId ];
						DWORD hashValue = hash.GetUniqueID();
						Matrix4x4 &ref = m_WorldBoneMatrices[ hashValue ];			
						memcpy( &tmBuffer[ boneId*12 ] , ref.GetMatrix(), sizeof( float )*12 );
					}
				}
				mat->SetVertexConstant( C_BONE_START, (float*)&(tmBuffer[0]), 3*(boneId) );
			}
			//draw the geometry to save off for later if it has not been computed this frame
			geom->Render( m_SceneSettings, override );
		}
	}

	//render the skeleton
	if (m_bDebugShowSkeleton && m_bIsVisible && m_GeometryController)
	{
		int nBones = m_calModel->getSkeleton()->getVectorBone().size();
		if (nBones > 0)
		{
			Matrix4x4 * matworld =  m_GeometryController->GetMatrix();
			m_IRenderer->SetMatrix( WORLD_MATRIX, (const float*)matworld->GetMatrix() );

			float *pLines = new float[nBones*6];
			int nLines = m_calModel->getSkeleton()->getBoneLines(pLines);
			for (int iLine=0; iLine < nLines; iLine++)
			{
				m_IRenderer->Draw3DLine( 
					pLines[iLine*6 + 0], pLines[iLine*6 + 1], pLines[iLine*6 + 2], 
					pLines[iLine*6 + 3], pLines[iLine*6 + 4], pLines[iLine*6 + 5], 
					255, 255, 255, 1, false );
			}
			delete[] pLines;
		}
	}
	return true;
}
DWORD Cal3DRenderObject::OnGetBoundingSphere( DWORD size, void * params )
{
	VERIFY_MESSAGE_SIZE( size, sizeof( GETBOUNDINGSPHEREPARAMS ) );
	GETBOUNDINGSPHEREPARAMS * p = (GETBOUNDINGSPHEREPARAMS*)params;
	if( p && p->position)
	{
		GetBoundingSphere( *p->position, p->radius );
		return MSG_HANDLED_STOP;
	}
	return MSG_NOT_HANDLED;
}

void Cal3DRenderObject::UpdateBoundingSphere()
{
	if( m_GeometryController )
	{
		m_CachedPosition = m_GeometryController->GetTranslationOffset();
		if( m_calModel &&
			m_GeometryController )
		{
			Vec3 Scaling = m_GeometryController->GetScaling();
			float maxaxis = (Scaling.x > Scaling.y) ? Scaling.x : Scaling.y ;
			maxaxis = (maxaxis > Scaling.z) ? maxaxis : Scaling.z ; 
			m_CachedRadius = m_calModel->getBoundingRadius()*maxaxis;
		}
		else
		{
			m_CachedRadius = 99999.f;
		}
	}
}

void Cal3DRenderObject::GetBoundingSphere( Vec3 &position, float &radius )
{
	UpdateBoundingSphere();
	position = m_CachedPosition;
	radius = m_CachedRadius;
}

bool Cal3DRenderObject::IsAlphaObject()
{	
	//Check if amterials have alpha on them
	if( !m_bHasAlphaSet )
	{
		m_bHasAlphaSet = true;
		m_bHasAlpha = false;
		
		for (UINT i=0; i<m_Geometry.size(); i++)
		{
			if (m_Geometry[i]->HasAlpha())
			{
				m_bHasAlpha = true;
				break;
			}
		}
	}
	return m_bHasAlpha;
}

bool Cal3DRenderObject::IsLessThan( const IRenderObject * obj, DWORD sortMethod, int curRenderPass )
{
	Vec3 m_Position = m_GeometryController->GetTranslationOffset();
	//to speed things up these are static
    static Vec3 CamPos(0,0,0);
	Vec3 * pBPos;
	//One way to sort for now, models
	//fast compare for now, no checking if obj is of same type?
	Cal3DRenderObject * objB = (Cal3DRenderObject * )obj;
	bool bLessThan = false;
	switch( sortMethod )
	{
	case SORT_MODEL:
	case SORT_MATERIAL:
		{
		
			break;
		}
	case SORT_DISTANCE:
		{
			//get the camera position
			Vec3 CamPos;
			CONTEXTCAMVECTORPARAMS ccvp;
			ccvp.pRC = NULL;
			ccvp.vdata = &CamPos;
			static DWORD msgHash_GetActiveCameraPosition = CHashString(_T("GetActiveCameraPosition")).GetUniqueID();
			if( m_ToolBox->SendMessage(msgHash_GetActiveCameraPosition, sizeof( CONTEXTCAMVECTORPARAMS), &ccvp, NULL, NULL ) != MSG_NOT_HANDLED )
			{
				
				pBPos = &objB->GetPosition();
				float ADist = fabs( m_Position.x - CamPos.x ) + fabs( m_Position.y - CamPos.y ) + fabs( m_Position.z - CamPos.z );
				float BDist = fabs( pBPos->x - CamPos.x ) + fabs( pBPos->y - CamPos.y ) + fabs( pBPos->z - CamPos.z );
				bLessThan = ( ADist < BDist );
			}
			break;
		}
	case SORT_REVERSEDISTANCE:
		{
			//get the camera position
			Vec3 CamPos;
			CONTEXTCAMVECTORPARAMS ccvp;
			ccvp.pRC = NULL;
			ccvp.vdata = &CamPos;
			static DWORD msgHash_GetActiveCameraPosition = CHashString(_T("GetActiveCameraPosition")).GetUniqueID();
			if( m_ToolBox->SendMessage(msgHash_GetActiveCameraPosition, sizeof( CONTEXTCAMVECTORPARAMS), &ccvp, NULL, NULL ) != MSG_NOT_HANDLED )
			{
				pBPos = &objB->GetPosition();
				float ADist = fabs( m_Position.x - CamPos.x ) + fabs( m_Position.y - CamPos.y ) + fabs( m_Position.z - CamPos.z );
				float BDist = fabs( pBPos->x - CamPos.x ) + fabs( pBPos->y - CamPos.y ) + fabs( pBPos->z - CamPos.z );
				bLessThan = ( ADist > BDist );
			}
			break;
		}
	default:
		break;
	};
	return bLessThan;
}

void Cal3DRenderObject::DeInit()
{
	UnregisterAllAnimationCallbacks();

	/*
	// Delete any materials that the object uses. 
	// Kyle: Work in progress. Putting aside for Precaching system.
	for( unsigned int i = 0; i < GetNumMaterial(); i++ )
	{
		IMaterial* material = GetMaterial( i );
		// ShaderManager->OnRemoveMaterial()
		CREATEMATERIALINTERFACE removeMaterialMsg;
		removeMaterialMsg.m_Name = material->GetName();
		
		static DWORD msg_RemoveMaterial = CHashString(_T("RemoveMaterial")).GetUniqueID();
		m_ToolBox->SendMessage( msg_RemoveMaterial, sizeof(CREATEMATERIALINTERFACE), &removeMaterialMsg );
	}
	*/

	m_bIsVisible = true;
	m_bHasAlphaSet = false;
	
	if( m_calModel != NULL )
		delete m_calModel;    
	m_calModel = NULL;

	m_calHardwareModel = NULL;

	//Release geometries here
	for( unsigned int i = 0; i < m_Geometry.size(); i++ )
	{
		if( m_Geometry[i] != NULL )
		{
			static DWORD msgHash_DestroyGeometry = CHashString(_T("DestroyGeometry")).GetUniqueID();
			m_ToolBox->SendMessage(msgHash_DestroyGeometry, sizeof( IGeometry** ), &m_Geometry[ i ] );
			m_Geometry[i] = NULL;
		}
	}
	m_Geometry.clear();

	if( m_GeometryController != NULL )
	{
		static DWORD msgHash_DestroyController = CHashString(_T("DestroyController")).GetUniqueID();
		m_ToolBox->SendMessage(msgHash_DestroyController, sizeof( IController** ), &m_GeometryController );
		m_GeometryController = NULL;
	}

	if( m_SceneSettings != NULL )
	{
		static DWORD msgHash_DestroySceneSettings = CHashString(_T("DestroySceneSettings")).GetUniqueID();
		m_ToolBox->SendMessage(msgHash_DestroySceneSettings, sizeof( ISceneSettings ), &m_SceneSettings );
		m_SceneSettings = NULL;
	}


	m_IndexBufferAllocation.m_Offset = 0;
	m_IndexBufferAllocation.m_Size = 0;
	m_IndexBufferAllocation.m_Stride = 0;
	m_IndexBufferAllocation.m_InterfaceHandle = 0;
	m_IndexBufferAllocation.m_AllocationHandle = 0;
	m_IndexBufferAllocation.m_BaseVertexOffset = 0;

	m_VertexBufferAllocation.m_Offset = 0;
	m_VertexBufferAllocation.m_Size = 0;
	m_VertexBufferAllocation.m_Stride = 0;
	m_VertexBufferAllocation.m_InterfaceHandle = 0;
	m_VertexBufferAllocation.m_AllocationHandle = 0;
	m_VertexBufferAllocation.m_BaseVertexOffset = 0;
}

/// \brief either reads or writes the entity information to a file
/// Currently Loads up the entity completely if reading(inside vision too)
void Cal3DRenderObject::Serialize(IArchive &ar)
{
	//create
	//Need to save off:
	//position
	//rotation
	//scaling
	//animation keyframe? harder
	//file name
	//color tints and any other attributes
		
	if (ar.IsReading())
	{
		// Get the data's version number
		CHashString hszVersion = _T("");

		float version;
		static DWORD msgHash_GetFileVersion = CHashString(_T("GetFileVersion")).GetUniqueID();
		DWORD retval = m_ToolBox->SendMessage(msgHash_GetFileVersion, sizeof(IHashString), &hszVersion);
		if (retval != MSG_HANDLED)
		{
			m_ToolBox->Log(LOGERROR, _T("Cal3D Render Object Serialize: Could not get file version!"));
			assert(0);
			return;
		}

		float atofVersion = (float)_tstof(hszVersion.GetString());

		version = atofVersion;

		ar.Read( m_Filename );
		
		m_Filename.MakeSafeFileName();
		m_FileHash.Init(m_Filename);

		if( version < 2.5 )
		{
			//now read the basic stuff
			ar.Read( SerializedPosition, _T("Position") );
			ar.Read( SerializedRotation, _T("Rotation") );
			ar.Read( SerializedScaling, _T("Scaling") );
			ar.Read( m_ShaderFile );
		}
	}
	else
	{
		//ar.StartClass(_T("Cal3DRenderObject"));
		if( m_GeometryController )
		{
			SerializedPosition = m_GeometryController->GetTranslationOffset();
			SerializedRotation = m_GeometryController->GetRotationOffset();
			SerializedScaling = m_GeometryController->GetScaling();
		}
		ar.Write( m_Filename, _T("FileName") );
		/*ar.Write( SerializedPosition, _T("Position") );
		ar.Write( SerializedRotation, _T("Rotation") );
		ar.Write( SerializedScaling, _T("Scaling") );
		ar.Write( m_ShaderFile, _T("Shader"));*/
		//ar.EndClass();
	}
}

IHashString * Cal3DRenderObject::GetBaseSortClass()
{
	static CHashString CRO(_T("Cal3DRenderObject"));
	return &CRO;
}

IMeshObject * Cal3DRenderObject::GetMeshObject()
{
	//TODO:
	return NULL;
}

DWORD Cal3DRenderObject::OnGetModelFileID(DWORD size, void *params)
{
	//Fast ID return of the hash since this will be a much used function when
	//object cubemap and LOD managers are hooked up
	VERIFY_MESSAGE_SIZE(sizeof(DWORD), size);
	DWORD* pFileHash = (DWORD*) params;
	if( pFileHash )
	{
		*pFileHash =  m_FileHash.GetUniqueID();
	}else
	{
		*pFileHash = 0;
	}
	return MSG_HANDLED_STOP;
}

DWORD Cal3DRenderObject::OnGetAnimationList( DWORD size, void * params )
{
	VERIFY_MESSAGE_SIZE( sizeof( GETANIMATIONLISTPARAMS ), size );
	GETANIMATIONLISTPARAMS * parm = (GETANIMATIONLISTPARAMS*)params;

	DWORD retVal = MSG_NOT_HANDLED;
	CalCoreModel * coreModel = m_calModel->getCoreModel();
	if (parm && coreModel && parm->AnimationList)
	{
		// Number of Animations
		parm->numAnimations = coreModel->getCoreAnimationCount();

		// Generate list of names
		CalCoreAnimation* coreAnim = NULL;
		for (int i = 0; i < parm->numAnimations; i++)
		{
			coreAnim = coreModel->getCoreAnimation(i);
			if (coreAnim)
			{
				parm->AnimationList->push_back(StdString(coreAnim->getName()));
			}
		}
		retVal = MSG_HANDLED;
		return retVal;
	}
	else
		return retVal;
}

DWORD Cal3DRenderObject::OnPlayAnimation( DWORD size, void * params )
{
	VERIFY_MESSAGE_SIZE( sizeof( RENDEROBJECTANIMATIONPARAMS ), size );
	RENDEROBJECTANIMATIONPARAMS * parm = (RENDEROBJECTANIMATIONPARAMS*)params;

	DWORD retVal = MSG_NOT_HANDLED;
	if( parm &&
		m_bSkinnedModel && 
		m_calModel && 
		m_calModel->getCoreModel() )
	{
		m_bTransformIsDirty = true;
		CalCoreModel * coreModel = m_calModel->getCoreModel();
		if( parm->AnimationName )
		{
			parm->AnimationID = coreModel->getCoreAnimationId( parm->AnimationName->GetString() );
		}
		// check if the time has come to blend to the next animation
		if( parm->AnimationID >= 0 &&
			parm->AnimationID < coreModel->getCoreAnimationCount() )
		{
			// fade in the new animation
			CalMixer * mixer = m_calModel->getMixer();
			if( mixer )
			{
				if( parm->bLoop == true )
				{
					mixer->blendCycle( parm->AnimationID, 1.0f, parm->BlendIn );
					retVal = MSG_HANDLED_STOP;

					CActiveAnimation anim;
					anim.m_AnimID = parm->AnimationID;
					m_ActiveAnimationCycles.push_back( anim );
				}
				else
				{
					mixer->executeAction(parm->AnimationID, parm->BlendIn, parm->BlendOut, 1.f, parm->LockLastFrame );
					retVal = MSG_HANDLED_STOP;

					ResetRenderOffsetDelta();
				}
			}
		}
	}

	return retVal;
}


DWORD Cal3DRenderObject::OnPlaySingleCycleAnimation( DWORD size, void * params )
{
	VERIFY_MESSAGE_SIZE( sizeof( RENDEROBJECTANIMATIONPARAMS ), size );
	RENDEROBJECTANIMATIONPARAMS * parm = (RENDEROBJECTANIMATIONPARAMS*)params;

	StopAllAnimationCycles( parm->BlendOut );
	parm->bLoop = true; // force cyclic animation
	return OnPlayAnimation( size, params );
}


DWORD Cal3DRenderObject::OnSetVisibility( DWORD size, void * params )
{
	bool * bparams;
	VERIFY_MESSAGE_SIZE( sizeof( bool ), size );
	bparams = (bool*)params;
	m_bIsVisible = *bparams;
	return MSG_NOT_HANDLED;
}

DWORD Cal3DRenderObject::OnStopAnimation( DWORD size, void * params )
{
	VERIFY_MESSAGE_SIZE( sizeof( RENDEROBJECTANIMATIONPARAMS ), size );
	RENDEROBJECTANIMATIONPARAMS * parm = (RENDEROBJECTANIMATIONPARAMS*)params;
	
	DWORD retVal = MSG_NOT_HANDLED;

	if( parm &&
		m_bSkinnedModel && 
		m_calModel && 
		m_calModel->getCoreModel() )
	{
		CalCoreModel * coreModel = m_calModel->getCoreModel();
		if( parm->AnimationName )
		{
			parm->AnimationID = coreModel->getCoreAnimationId( parm->AnimationName->GetString() );
		}
		// fade in the new animation
		CalMixer * mixer = m_calModel->getMixer();
		if( mixer )
		{
			m_bTransformIsDirty = true;
			bool bResult;
			if (!(bResult = mixer->removeAction(parm->AnimationID)))
			{
				bResult = mixer->clearCycle(parm->AnimationID, parm->BlendOut);
			}
			retVal = MSG_HANDLED_STOP;
		}
	}

	return retVal;
}

DWORD Cal3DRenderObject::OnStopAllAnimation( DWORD size, void * params )
{
	StopAllAnimationCycles( 0.0f );

	return MSG_HANDLED_STOP;
}

DWORD Cal3DRenderObject::OnStopAllAnimationActions( DWORD size, void * params )
{
	StopAllAnimationActions();

	return MSG_HANDLED_STOP;
}

bool Cal3DRenderObject::IsKindOf( IHashString * compType )
{	
	static CHashString  CCal3dROCOMP(_T("Cal3DRenderObject") );
	
	return (compType->GetUniqueID() == CCal3dROCOMP.GetUniqueID());
}

DWORD Cal3DRenderObject::OnRemoveFromQuadTree( DWORD size, void * params )
{
	RemoveBoundingObject();

	return MSG_HANDLED_STOP;
}
DWORD Cal3DRenderObject::OnAddToQuadTree( DWORD size, void * params )
{
	UpdateBoundingObject();

	return MSG_HANDLED_STOP;
}



DWORD Cal3DRenderObject::OnSetMatrix( DWORD size, void * params )
{
	VERIFY_MESSAGE_SIZE( size, sizeof( Matrix4x4 ) );
	Matrix4x4 * m = (Matrix4x4*)params;
	if( m  )
	{
		m_GeometryController->SetMatrix( m );

		UpdateBoundingObject();
	}
	return MSG_HANDLED_STOP;
}

DWORD Cal3DRenderObject::OnSetCalScaleVariable( DWORD size, void * params )
{
	VERIFY_MESSAGE_SIZE( size, sizeof( Vec3 ) );
	Vec3 * scaling = (Vec3*)params;
	if( scaling &&
		m_GeometryController )
	{
		m_GeometryController->SetScaling( *scaling );
	}
	return MSG_HANDLED_STOP;
}

/// Grabs the geometry pieces of the render object if there is any uses it for render instead
/// of the render object render(might need to be changed)
IGeometry* Cal3DRenderObject::GetGeometry( int index )
{	
	//static models allow system to break up the model into pieces and render as efficiently as
	//possible
	if( !m_bSkinnedModel )
	{
		int i = 0;
		int size = m_Geometry.size();
		if( index >= 0 && index < size )
		{
			return m_Geometry[ index ];
		}
	}
	return NULL;
}

/// Attaches a light object to this Render object
void Cal3DRenderObject::AttachLight( ILightObject * light )
{	
	// Ambient Lights
	if (light->GetLightType() == AMBIENT_LIGHT)
	{
		float fColorIntensity[4];
		light->GetColorIntensity(fColorIntensity);
		m_AmbientLightTerm[0] += fColorIntensity[0];
		m_AmbientLightTerm[1] += fColorIntensity[1];
		m_AmbientLightTerm[2] += fColorIntensity[2];
		m_AmbientLightTerm[3] += fColorIntensity[3];
		int size = m_Geometry.size();
		for( int b = 0; b < size; b++ )
		{
			if( m_Geometry[ b ] )
			{
				m_Geometry[ b ]->AttachLight( light );
			}
		}
	}
	// Omni Lights
	else
	{
		float priority = 0.0f;
		Vec3 position = m_GeometryController->GetTranslationOffset();
		bool affected = light->GetLightPriority( position, m_CachedRadius, priority );
		if( affected )
		{
			//check radius
			if( m_CachedRadius > Cal3DRenderObject::m_BoundingTestThreshold )
			{
				//test light attenuation against bounding box for better culling
			}
			for( int i = 0; i < MAX_LIGHTS; i++ )
			{
				if( m_LightPriorities[ i ] < priority )
				{
					//push back rest and insert
					for( int j = MAX_LIGHTS - 1; j > i ; j--)
					{
						m_Lights[ j ] = m_Lights[ j-1 ];
						m_LightPriorities[ j ] = m_LightPriorities[ j-1 ];
					}
					m_Lights[ i ] = light;
					m_LightPriorities[ i ] = priority;
					// If space exists for more lights, add light.
					if( m_NumLights < MAX_LIGHTS )
					{					
						m_NumLights++;
					}
					//static models allow system to break up the model into pieces and render as efficiently as
					//possible, unskinned models still need to attach lights to individual geometry
					{
						int size = m_Geometry.size();
						if( size > 0 )
						{
							for( int b = 0; b < size; b++ )
							{
								if( m_Geometry[ b ] )
								{
									m_Geometry[ b ]->ResetLightCount();
									for( int a = 0; a < m_NumLights; a++ )
									{
										m_Geometry[ b ]->AttachLight( m_Lights[ a ], a );
									}
								}
							}
						}
					}		

					if( m_SceneSettings )
					{
						if( m_SceneSettings->GetShaderFlags() & (SHADOW_RECEIVE) )
						{
							//get highest priority light that can cast shadows:
							for( int a = 0; a < m_NumLights; a++ )
							{
								ILightObject * curLight = m_Lights[ a ];
								if( curLight && curLight->CanCastShadows() )
								{
									//clear receiver flags
									m_SceneSettings->RemoveShaderFlag( SHADOW_RECEIVE_HEMI | SHADOW_RECEIVE_OMNI |
															SHADOW_RECEIVE_DIR );
									//check what type of light is affecting it:
									if( curLight->GetLightType() == OMNI_POINT_LIGHT )
									{
										m_SceneSettings->AddShaderFlag( SHADOW_RECEIVE_OMNI );
									}
									else if( curLight->GetLightType() == HEMI_POINT_LIGHT )
									{
										m_SceneSettings->AddShaderFlag( SHADOW_RECEIVE_HEMI );
									}
									else if( curLight->GetLightType() == DIRECTIONAL_LIGHT )
									{
										m_SceneSettings->AddShaderFlag( SHADOW_RECEIVE_DIR );
									}	
									break;
								}
							}
						}
					}
					break;
				}
			}		
		}
	}
	if( m_SceneSettings )
	{
		m_SceneSettings->SetActiveLightCount( m_NumLights );
	}
}

/// Signals a detach of all light objects
void Cal3DRenderObject::DetachAllLights()
{
	memset( m_Lights, 0, sizeof( m_Lights ) );
	m_NumLights = 0;
	for (int i = 0; i < MAX_LIGHTS; i++)
	{
		m_LightPriorities[ i ] = -99999.f;//this will essentially reset priorities quickly
	}
	memset( m_AmbientLightTerm, 0, sizeof(m_AmbientLightTerm) );
		
		//static models allow system to break up the model into pieces and render as efficiently as
	//possible	
	{
		int size = m_Geometry.size();
		for( int i = 0; i < size; i++ )
		{
			if( m_Geometry[ i ] )
			{
				m_Geometry[ i ]->DetachAllLights();
			}
		}
	}	

	if (m_SceneSettings)
		m_SceneSettings->SetActiveLightCount( 0 );
}
/// Gets the number of Geometry elements this renderobject possesses or if it uses geometry at all	
UINT Cal3DRenderObject::GetNumGeometry(){ 
	//Skinned model requires immediate rendering control for bone constant set up
	if( m_bSkinnedModel )
	{
		return 0;
	}
	//static models allow system to break up the model into pieces and render as efficiently as
	//possible
	else
	{
		return m_Geometry.size(); 
	}
}

DWORD Cal3DRenderObject::ProcessAnimationCallbackEvent( DWORD size, void * params, bool addModel )
{
	VERIFY_MESSAGE_SIZE( sizeof( REGISTERCAL3DANIMATIONCALLBACK ), size );
	REGISTERCAL3DANIMATIONCALLBACK * parm = (REGISTERCAL3DANIMATIONCALLBACK*)params;

	if( parm &&
		m_bSkinnedModel && 
		m_calModel && 
		m_calModel->getCoreModel() &&
		parm->StateObjectName &&
		parm->EventName )
	{
		CalCoreModel * coreModel = m_calModel->getCoreModel();
		if( parm->AnimationName )
		{
			parm->AnimationId = coreModel->getCoreAnimationId( parm->AnimationName->GetString() );
		}
		// check if the time has come to blend to the next animation
		if( parm->AnimationId >= 0 &&
			parm->AnimationId < coreModel->getCoreAnimationCount() )
		{
			//check for existing callback
			CallbackInfo callback;
			callback.animid = parm->AnimationId;
			callback.triggeroncomplete = parm->bTriggerOnComplete;
			callback.triggeronstop = parm->bTriggerOnStop;
			callback.timetocall = parm->AnimationTriggerTime;
			callback.eventname.Init( parm->EventName->GetString() );
			callback.statename.Init( parm->StateObjectName->GetString() );
			callback.m_CustomEvent = parm->CustomMessageName;
			callback.archive = parm->EventParamsArchive;		
			callback.renderobject = this;
			GETCAL3DCALLBACK cbmsg;
			cbmsg.anim =  coreModel->getCoreAnimation( parm->AnimationId );
			static DWORD msgHash_GetCal3dCallback = CHashString(_T("GetCal3dCallback")).GetUniqueID();
			m_ToolBox->SendMessage(msgHash_GetCal3dCallback, sizeof(GETCAL3DCALLBACK), &cbmsg, NULL, NULL );
			if( cbmsg.instance )
			{
				//got a callback back
				Cal3dCallback *  cb = cbmsg.instance;
				if( addModel )
				{
					cb->AddModel( callback, m_calModel );
				}
				else
				{
					cb->RemoveModel( callback, m_calModel );
				}
				return MSG_HANDLED_STOP;
			}	
		}
	}

	return MSG_NOT_HANDLED;
}

DWORD Cal3DRenderObject::OnRegisterAnimationCallback( DWORD size, void * params )
{
	return ProcessAnimationCallbackEvent( size, params, true );
}

DWORD Cal3DRenderObject::OnUnregisterAnimationCallback( DWORD size, void * params )
{
	return ProcessAnimationCallbackEvent( size, params, false );
}

DWORD Cal3DRenderObject::OnUnregisterAllAnimationCallbacks( DWORD size, void * params )
{
	UnregisterAllAnimationCallbacks();
	return MSG_HANDLED_STOP;
}

DWORD Cal3DRenderObject::OnGetAnimationId( DWORD size, void * params )
{
	VERIFY_MESSAGE_SIZE( sizeof( ANIMATIONID ), size );
	ANIMATIONID * parm = (ANIMATIONID*)params;
	if( parm &&
		m_bSkinnedModel && 
		m_calModel && 
		m_calModel->getCoreModel()  )
	{
		CalCoreModel * coreModel = m_calModel->getCoreModel();
		if( coreModel )
		{
			parm->AnimationId = coreModel->getCoreAnimationId( (const TCHAR*)parm->AnimationName );
			return MSG_HANDLED_STOP;
		}
	}
	parm->AnimationId  = -1;
	return MSG_ERROR;
	
}

DWORD Cal3DRenderObject::GetBoneInfo( DWORD size, void * params )
{
	VERIFY_MESSAGE_SIZE( sizeof( BONEINFO ), size );
	BONEINFO * parm = (BONEINFO*)params;	
	if( m_calModel &&
		parm &&
		parm->bonename &&		
		m_bSkinnedModel )
	{
		//Update transforms to be current with new animation frame coming up
		if( m_bTransformIsDirty &&
			m_GeometryController )
 		{
			Matrix4x4 * controllerMatrix = m_GeometryController->GetMatrix();
			if( controllerMatrix )
			{
 				GetParentTransform( *controllerMatrix );
			}
			UpdateBoundingSphere();
 			m_bTransformIsDirty = false;
			// update the render object position to any manager that needs to know when it moves, i.e. the quad tree
			// manager
			UpdateBoundingObject();
 		}
		if( m_bPaused == false )
		{
			if (m_LastUpdateTick < m_Timer->GetTime())
			{
				m_LastUpdateTick = m_Timer->GetTime();

				float fTimeDelta = m_Timer->GetTimeDelta( &hsGameTime );
				m_calModel->update( fTimeDelta );
				UpdateRenderOffset();
			}
		}
		else
		{
			UpdateRenderOffset();
		}
		// be sure to update attached objects!
		SetAttachmentObjectsDirty();

		CalBone *bone = GetBone( parm->bonename->GetString() );
		if( bone )
		{
			Matrix4x4 * matworld =  m_GeometryController->GetMatrix();
			if (matworld && m_calHardwareModel && m_calModel)
			{
				Matrix4x4 transformation;
				CalQuaternion cq;
				cq = bone->getRotationAbsolute();
				Quaternion q(cq.x, cq.y, cq.z, cq.w );
				q.GetMatrix( transformation );

				CalVector translationBoneSpace = bone->getTranslationAbsolute();
				
				Vec3 position(translationBoneSpace.x, translationBoneSpace.y, translationBoneSpace.z);

				if( parm->m_DisplacedInfo == false )//don't offset this bone, it actually comes out offset
					position = (*matworld) * (position + m_RenderOffset);
				else
					position = (*matworld) * (position);

				if (parm->m_Position)
				{
					parm->m_Position->Set( position );
				}

				transformation = (*matworld)*transformation;
				EulerAngle rotation = transformation.GetRotation();
				if (parm->m_Rotation)
				{
					parm->m_Rotation->Set( rotation );
				}
				if (parm->m_Matrix)
				{
					*parm->m_Matrix = transformation;
					parm->m_Matrix->SetTranslation( position );
				}
			}
			return MSG_HANDLED_STOP;
		}
	}
	return MSG_NOT_HANDLED;
}


DWORD Cal3DRenderObject::OnGetModelFileName( DWORD size, void * params )
{
	VERIFY_MESSAGE_SIZE( size, sizeof( CHashString ) );
	CHashString * parms = (CHashString *)params;
	if( parms &&
		_tcslen( (const TCHAR*) m_Filename ) > 0 )
	{
		parms->Init( (const TCHAR *) m_Filename );
		return MSG_HANDLED_STOP;
	}
	return MSG_ERROR;
}

DWORD Cal3DRenderObject::OnGetGeometryAllocations( DWORD size, void * params )
{
	VERIFY_MESSAGE_SIZE( size, sizeof( GETRENDERBUFFERALLOCATIONMSG ) );
	GETRENDERBUFFERALLOCATIONMSG * parms = (GETRENDERBUFFERALLOCATIONMSG *)params;
	if( parms )
	{
		//Loop through geometry and add allocations:
		int size = (int)m_Geometry.size();
		for( int i = 0; i < size; i++ )
		{
			RENDERBUFFERALLOCATION curAlloc;
			IGeometry * geom = m_Geometry[ i ];
			if( geom )
			{
				geom->GetIndexBufferAllocation( curAlloc.m_IndexAllocation );
				geom->GetVertexBufferAllocation( curAlloc.m_VertexAllocation );
				if( m_bSkinnedModel )
				{
					curAlloc.m_VertexAllocation.VertexFormat = m_pModelManager->VertexFormat;
				}
				else
				{
					curAlloc.m_VertexAllocation.VertexFormat = m_pModelManager->VertexFormatNoSkin;
				}
				curAlloc.m_IndexBufferInterface = geom->GetIndexBufferInterface();
				curAlloc.m_VertexBufferInterface = geom->GetVertexBufferInterface();
				IMaterial * mat = geom->GetMaterial();				
				if( mat )
				{
					curAlloc.m_Material = mat;
				}
			}
			parms->m_GeometryAllocationsOut.push_back( curAlloc );
		}
		return MSG_HANDLED_STOP;
	}
	return MSG_ERROR;
}

DWORD Cal3DRenderObject::OnGetBoundingBox( DWORD size, void* params )
{
	VERIFY_MESSAGE_SIZE( size, sizeof( GETBOUNDINGBOXPARAMS ) );
	GETBOUNDINGBOXPARAMS * p = (GETBOUNDINGBOXPARAMS*)params;
	if( p && p->position && p->extents && p->rotation )
	{
		if (GetBoundingBox( *p->position, *p->extents, *p->rotation ))
			return MSG_HANDLED_STOP;
	}
	return MSG_ERROR;
}

/// \brief	Get the bounding box of the object
bool Cal3DRenderObject::GetBoundingBox( Vec3 &position, Vec3 &extents, EulerAngle &rotation )
{
	if( m_GeometryController )
	{
		// position
		position = m_GeometryController->GetTranslationOffset();
		//rotation
		rotation = m_GeometryController->GetRotationOffset();
	}

	Vec3 maxs(0,0,0), mins(0,0,0);
	if (m_calModel)
	{
		CalVector points[8];
		CalBoundingBox &bbox = m_calModel->getBoundingBox();
		bbox.computePoints(points);
		mins.Set( points[0].x, points[0].y, points[0].z );
		maxs.Set( points[7].x, points[7].y, points[7].z );
	}

	// calculate the extents
	extents.x = abs( (maxs.x-mins.x) * 0.5f );
	extents.y = abs( (maxs.y-mins.y) * 0.5f );
	extents.z = abs( (maxs.z-mins.z) * 0.5f );

	// position describes the center of the bounding box such that mins/maxs can be recreated from position and the extents
	position += (maxs+mins) * 0.5f;

	return true;
}

DWORD Cal3DRenderObject::OnGetParentName( DWORD size, void * params )
{
	GETCALPARENT * p;
	VERIFY_MESSAGE_SIZE( size, sizeof( GETCALPARENT ));
	p = (GETCALPARENT*)params;
	if( p )
	{
		p->parent  = GetParentName();
	}
	return MSG_HANDLED_STOP;
}

DWORD Cal3DRenderObject::OnSetLightmapData( DWORD size, void* params )
{
	VERIFY_MESSAGE_SIZE( size, sizeof( SETLIGHTMAPDATAMSG ) );
	SETLIGHTMAPDATAMSG * parms = (SETLIGHTMAPDATAMSG*)params;
	if( parms &&
		parms->cache &&
		m_calModel &&
		m_VertexBufferInterface )
	{
		//for all geometry
		LightmapCache * cache = parms->cache;
		if( cache->m_bRemapped == true )//not remapped
		{
			//was remapped, we have to reallocate our vertices!!!!
			//not THAT bad since indices stays the same and we're already only using one vertex allocation anyway
			//shared between the entire mesh
			BUFFERALLOCATIONSTRUCT  newAllocation;
			int newSize = cache->m_NumVertices;
			int sizeVertex = 0;

			//create new allocation
			ALLOCATERENDERBUFFERMSG msg;
			//Skinned and nonskinned versions
			if( m_calModel->getSkeleton() )
			{
				msg.ElementsToAllocate = newSize;
				msg.SizeVertexFormat = sizeof( VERTEX );
				msg.VertexFormat = m_pModelManager->VertexFormat;
				sizeVertex = sizeof( VERTEX );
			}else
			{
				msg.ElementsToAllocate = newSize;
				msg.SizeVertexFormat = sizeof( VERTEX_NOSKIN );
				msg.VertexFormat = m_pModelManager->VertexFormatNoSkin;
				sizeVertex = sizeof( VERTEX_NOSKIN );
			}

			static DWORD msgHash_AllocateVertices = CHashString(_T("AllocateVertices")).GetUniqueID();
			DWORD res = m_ToolBox->SendMessage(msgHash_AllocateVertices, sizeof( msg ), &msg, NULL, NULL );

			if( res != MSG_HANDLED )
			{
				return MSG_ERROR;
			}
			newAllocation.m_Offset = msg.m_ReturnOffset;
			newAllocation.m_AllocationHandle = msg.m_ReturnHandle;
			newAllocation.m_InterfaceHandle = msg.m_ReturnInterfaceHandle;
			newAllocation.m_Size = newSize;
			newAllocation.m_Stride = msg.SizeVertexFormat;
			IVertexBufferObject * newVB = (IVertexBufferObject*)msg.m_InterfacePointer;
			BYTE * buffer = new BYTE[ sizeVertex*newSize ];
			BYTE * oldVerts;
			BYTE * newVerts;
			//lock current VB
			if( m_VertexBufferInterface->Lock( m_VertexBufferAllocation.m_Offset, m_VertexBufferAllocation.m_Size,
				 (void**)&oldVerts ) )
			{
				memcpy( buffer, oldVerts, sizeVertex*newSize );
				m_VertexBufferInterface->Unlock();
				//Now lock the new VB and do the actual remapping
				if( newVB &&
					newVB->Lock( newAllocation.m_Offset, newSize, (void**)&newVerts ) )
				{					
					for( int i = 0; i < newSize; i++ )
					{
						int index = cache->m_RemappedData[ i ];
						BYTE * oldVertex = oldVerts;
						//seek
						oldVertex += index*sizeVertex;
						//copy
						memcpy( newVerts, oldVertex, sizeVertex );
#if( 0 )
						if( !m_calModel->getSkeleton() )
						{
							//for the new uv, let's flatten the geometry as a test based on uv
							float u = cache->m_UVData[ i*2 ];
							float v = cache->m_UVData[ i*2 + 1 ];
							VERTEX_NOSKIN * V = (VERTEX_NOSKIN*)newVerts;
							//map uv to xy
							V->pos[ 0 ] = u*500.f;
							V->pos[ 1 ] = v*500.f;
							V->pos[ 2 ] = 0.f;

						}
#endif						
						//copy uvs
						if( !m_calModel->getSkeleton() )
						{
							VERTEX_NOSKIN * V = (VERTEX_NOSKIN*)newVerts;
							V->lu = cache->m_UVData[ i*2 ];
							V->lv = cache->m_UVData[ i*2 + 1 ];
						}
						newVerts += sizeVertex;
					}
					newVB->Unlock();
				}
			}
			delete [] buffer;	
			//remove old allocation
			static DWORD msgHash_RemoveVertexAllocation = CHashString(_T("RemoveVertexAllocation")).GetUniqueID();
			m_ToolBox->SendMessage(msgHash_RemoveVertexAllocation, sizeof( m_VertexBufferAllocation ), &m_VertexBufferAllocation );
			//reset allocation and VB
			m_VertexBufferAllocation = newAllocation;
			m_VertexBufferInterface = newVB;
			//Now reassign the indices, same indices so just lock
			if( m_IndexBufferInterface &&
				m_IndexBufferAllocation.m_Size == cache->m_NumIndices )
			{
				CalIndex * pIndices = NULL;
				if( m_IndexBufferInterface->Lock( m_IndexBufferAllocation.m_Offset, m_IndexBufferAllocation.m_Size,
					(void**)&pIndices ) )
				{
					//copy
					for( int i = 0; i < cache->m_NumIndices; i++ )
					{
						pIndices[ i ] = (CalIndex) cache->m_RemappedIndices[ i ];
					}
					m_IndexBufferInterface->Unlock();
				}
			}
		}
		//Handle UV data streams
		int size = m_Geometry.size();
		for( int i = 0; i < size; i++ )
		{
			if( m_Geometry[ i ] )
			{
				m_Geometry[ i ]->SetVertexBufferAllocation( m_VertexBufferAllocation );
				//m_Geometry[ i ]->SetLightmapUVStream( (float*)cache->m_UVData, cache->m_NumVertices*2 );

				// Copy cache UVData to vertex stream
				if( !m_calModel->getSkeleton() && cache->m_UVData )
				{
					//lock current VB
					BYTE *oldVerts;
					if( m_VertexBufferInterface->Lock( m_VertexBufferAllocation.m_Offset, m_VertexBufferAllocation.m_Size, (void**)&oldVerts ) )
					{
						VERTEX_NOSKIN *V = (VERTEX_NOSKIN*)oldVerts;
						for (int iVert=0; iVert < m_VertexBufferAllocation.m_Size; iVert++)
						{
							V[iVert].lu = cache->m_UVData[ iVert*2 ];
							V[iVert].lv = cache->m_UVData[ iVert*2 + 1 ];
						}
						m_VertexBufferInterface->Unlock();
					}
				}

				m_Geometry[ i ]->SetLightmapTexture( parms->pTexture );
			}
		}
	}
	return MSG_HANDLED_STOP;
}


DWORD Cal3DRenderObject::OnSetSkeletonPose( DWORD size, void* params )
{
	//get same bones and set their positions
	VERIFY_MESSAGE_SIZE( size, sizeof( SKELETONINFO ) );
	SKELETONINFO * msg = (SKELETONINFO*)params;
	if( msg )
	{
		int numMsgBones = (int)msg->bones.size();
		BONELIST::iterator iter = msg->bones.begin();
		for( ; iter != msg->bones.end(); ++iter )
		{
			BONEINFO &ref = (*iter);
			if( 
				ref.bonename )
			{
				//get hash and set
				DWORD id = ref.bonename->GetUniqueID();
				if( ref.m_Matrix )
				{
					float * pMat = ref.m_Matrix->GetMatrix();
					m_WorldBoneMatrices[ id ].SetFrom4x4( pMat );
				}
			}			
		}
	}
	return MSG_HANDLED_STOP;
}

DWORD Cal3DRenderObject::OnGetSkeletonPose( DWORD size, void * params )
{
	//get same bones and set their positions
	VERIFY_MESSAGE_SIZE( size, sizeof( SKELETONINFO ) );
	SKELETONINFO * msg = (SKELETONINFO*)params;
	if( msg )
	{
		BuildBoneNameLinks();
		msg->bones.clear();
		map< DWORD, CHashString >::iterator iter = m_BoneIdToHashMap.begin();
		for(; iter != m_BoneIdToHashMap.end(); ++iter )
		{
			CHashString &refHash = iter->second;
			BONEINFO newInfo;
			newInfo.bonename = &refHash;
			DWORD hash = refHash.GetUniqueID();
			newInfo.m_Matrix = &( m_WorldBoneMatrices[ hash ] );
			msg->bones.push_back( newInfo );
		}
	}
	return MSG_HANDLED_STOP;
}

DWORD Cal3DRenderObject::OnEnablePhysicsAnimations( DWORD size, void* params )
{
	//get same bones and set their positions
	VERIFY_MESSAGE_SIZE( size, sizeof( bool ) );
	bool * msg = (bool*)params;
	if( msg )
	{
		m_bEnablePhysicsAnimation = *msg;
	}
	return MSG_HANDLED_STOP;
}

DWORD Cal3DRenderObject::OnGetRenderObjectInterface(DWORD size, void *param)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(IRenderObject*));
	IRenderObject **ppInterface = (IRenderObject**)param;
	if (ppInterface)
		*ppInterface = this;

	return MSG_HANDLED_STOP;
}

DWORD Cal3DRenderObject::OnGetControllerInterface( DWORD size, void * param )
{
	VERIFY_MESSAGE_SIZE(size, sizeof(IController *));
	IController **ppInterface = (IController**)param;
	if (ppInterface)
		*ppInterface = m_GeometryController;

	return MSG_HANDLED_STOP;
}

DWORD Cal3DRenderObject::OnSetDrawSkeleton(DWORD size, void *param)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(bool));
	m_bDebugShowSkeleton = *(bool*)param;
	return MSG_HANDLED_STOP;
}

DWORD Cal3DRenderObject::OnSetMaterialOverride(DWORD size, void *param)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(MATERIALOVERRIDEPARAMS));
	MATERIALOVERRIDEPARAMS mop = *(MATERIALOVERRIDEPARAMS*) param;

	SetMaterialOverrideValues(mop.hsName, mop.hsType, mop.hsValue, mop.vecValue, mop.hsMaterialName, false);
	m_bHasAlphaSet = false;

	return MSG_HANDLED_PROCEED;
}

DWORD Cal3DRenderObject::OnClearMaterialOverride(DWORD size, void *param)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(CLEARMATOVERRIDEPARAMS));
	CLEARMATOVERRIDEPARAMS cmop = *(CLEARMATOVERRIDEPARAMS*)param;

	SetMaterialOverrideValues(NULL, NULL, NULL, NULL, cmop.hsMaterialName, true);
	m_bHasAlphaSet = false;

	return MSG_HANDLED_PROCEED;
}

void Cal3DRenderObject::SetMaterialOverrideValues(IHashString *hsName, IHashString *hsType, IHashString *hsValue, 
		Vec4* vecValue, IHashString *hsMaterial, bool bClear)
{	
	for (UINT i=0; i<m_Geometry.size(); i++)
	{
		if (m_Geometry[i])
		{
			IMaterial *mat = m_Geometry[i]->GetMaterial();
			if (mat)
			{
				if (hsMaterial)
				{
					IHashString *matName = mat->GetName();
					
					if (matName)
					{
						// check for a substr, since the materials are full paths
						// and also not case sensitive
						StdString tempIn = hsMaterial->GetString();
						tempIn.tolower();

						StdString tempMat = matName->GetString();
						tempMat.tolower();

						if (_tcsstr(tempMat, tempIn) == NULL)
						{
							// skip this material
							continue;
						}
					}
				}

                if (bClear)
				{
					m_Geometry[i]->ClearMaterialOverride();
				}
				else
				{
					if (hsName && hsType)
					{
						if (vecValue)
						{
							m_Geometry[i]->SetMaterialOverride(hsType->GetUniqueID(), 
								hsName->GetUniqueID(), *vecValue);
						}
						else if (hsValue)
						{
							StdString value = hsValue->GetString();
							m_Geometry[i]->SetMaterialOverride(hsType->GetUniqueID(), 
								hsName->GetUniqueID(), value);
						}
					}
				}
			}
		}
	}	
}

DWORD Cal3DRenderObject::OnAttachSceneObject(DWORD size, void *param)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(ATTACHSCENEOBJECTPARAMS));
	ATTACHSCENEOBJECTPARAMS asop = *(ATTACHSCENEOBJECTPARAMS*)param;

	if (asop.hsAttachmentObject)
	{
		// if a node is NOT specified, hrc doesn't handle it
		// otherwise...
		if ((asop.hsNodeAttachName != NULL) && (m_bSkinnedModel))
		{
			CalSkeleton * skel = m_calModel->getSkeleton();
			CalCoreSkeleton * coreskeleton = skel->getCoreSkeleton();
			int boneid = coreskeleton->getCoreBoneId( asop.hsNodeAttachName->GetString() );
			if( boneid >= 0 )
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

void Cal3DRenderObject::GetAttachTransform(IHashString *hsNodeName, Matrix4x4 &attachXForm)
{
	BONEINFO bi;
	bi.bonename = hsNodeName;
	bi.m_Matrix = &attachXForm;
	GetBoneInfo(sizeof(BONEINFO), &bi);
}

DWORD Cal3DRenderObject::OnSetRenderOffset( DWORD size, void * param )
{
	VERIFY_MESSAGE_SIZE(size, sizeof(IHashString));
	IHashString *pNodeName = (IHashString*)param;
	m_pRenderOffsetBone = GetBone( pNodeName->GetString() );
	if (m_pRenderOffsetBone)
	{
		m_bResetRenderOffsetDelta = true;
	}
	m_RenderOffset.Set( 0,0,0 );
	m_PrevRenderOffset.Set( 0,0,0 );
	m_DeltaRenderOffset.Set( 0,0,0 );

	return MSG_HANDLED_STOP;
}

DWORD Cal3DRenderObject::OnGetRenderOffset( DWORD size, void * param )
{
	VERIFY_MESSAGE_SIZE(size, sizeof(Vec3));
	Vec3 *pVecOffset = (Vec3*)param;
	*pVecOffset = m_DeltaRenderOffset;
	pVecOffset->SetY( 0 );

	return MSG_HANDLED_STOP;
}

DWORD Cal3DRenderObject::OnPauseAnimations( DWORD size, void * param )
{
	VERIFY_MESSAGE_SIZE( size, sizeof( bool ) );
	bool * value = (bool*)param;
	if( value )
	{
		m_bPaused = *value;
	}
	return MSG_HANDLED_PROCEED;
}

void Cal3DRenderObject::StopAllAnimationCycles( float fDelay )
{
	if (m_calModel && m_calModel->getMixer())
	{
		m_bTransformIsDirty = true;
		CalMixer * mixer = m_calModel->getMixer();
		list<CActiveAnimation>::iterator itr = m_ActiveAnimationCycles.begin();
		while (itr != m_ActiveAnimationCycles.end())
		{
			CActiveAnimation *pAnim = &(*itr);
			mixer->clearCycle( pAnim->m_AnimID, fDelay );
			itr++;
		}
		m_ActiveAnimationCycles.clear();
	}
}

void Cal3DRenderObject::StopAllAnimationActions()
{
	if (m_calModel && m_calModel->getMixer())
	{
		m_bTransformIsDirty = true;
		CalCoreModel * coreModel =  m_calModel->getCoreModel();
		CalMixer * mixer = m_calModel->getMixer();
		for (int i=0; i < coreModel->getCoreAnimationCount(); i++)
		{
			mixer->removeAction(i);
		}
	}
}

void Cal3DRenderObject::UnregisterAllAnimationCallbacks()
{
	if( m_calModel )
	{
		CalCoreModel * coreModel =  m_calModel->getCoreModel();
		for( int i = 0; i < (int) coreModel->getCoreAnimationCount(); i++ )
		{
			CalCoreAnimation * anim = coreModel->getCoreAnimation( i );
			if( anim )
			{
				GETCAL3DCALLBACK cbmsg;
				cbmsg.anim = anim;
				static DWORD msgHash_GetCal3dCallback = CHashString(_T("GetCal3dCallback")).GetUniqueID();
				m_ToolBox->SendMessage(msgHash_GetCal3dCallback, sizeof(GETCAL3DCALLBACK), &cbmsg, NULL, NULL );
				if( cbmsg.instance )
				{
					//got a callback back
					Cal3dCallback *  cb = cbmsg.instance;
					cb->RemoveModel( m_calModel );
				}	
			}
		}
	}
}

CalBone* Cal3DRenderObject::GetBone( const TCHAR *szBoneName )
{
	if (!m_calModel)
		return NULL;

	CalSkeleton *pSkeleton = m_calModel->getSkeleton();
	if (!pSkeleton)
		return NULL;

	CalCoreSkeleton *pCoreSkeleton = pSkeleton->getCoreSkeleton();
	std::string name = szBoneName;
	int boneid = pCoreSkeleton->getCoreBoneId( name );
	if (boneid >= 0)
	{
		CalBone *pBone = pSkeleton->getBone( boneid );
		return pBone;
	}
	else
		return NULL;
}

Vec3 Cal3DRenderObject::GetBoneGlobalPosition( CalBone *pCalBone, bool bUseRootBoneOffset )
{
	CalVector calV = pCalBone->getTranslationAbsolute();
	Vec3 v( calV.x, calV.y, calV.z );
	if (!m_GeometryController)
		return v;
	Matrix4x4 *pTransform = m_GeometryController->GetMatrix();
	if (bUseRootBoneOffset)
		v = (*pTransform) * (v + m_RenderOffset);
	else
		v = (*pTransform) * (v);
	return v;
}

Vec3 Cal3DRenderObject::GetBoneGlobalPositionNoTranslation( CalBone *pCalBone, bool bUseRootBoneOffset )
{
	CalVector calV = pCalBone->getTranslationAbsolute();
	Vec3 v( calV.x, calV.y, calV.z );
	if (!m_GeometryController)
		return v;
	Matrix4x4 transform = *m_GeometryController->GetMatrix();
	transform.SetTranslation( Vec3(0,0,0) );
	if (bUseRootBoneOffset)
		v = (transform) * (v + m_RenderOffset);
	else
		v = (transform) * (v);
	return v;
}

Vec3 Cal3DRenderObject::GetBoneModelPosition( CalBone *pCalBone )
{
	CalVector calV = pCalBone->getTranslationAbsolute();
	Vec3 v( calV.x, calV.y, calV.z );
	return v;
}

void Cal3DRenderObject::UpdateRenderOffset()
{
	if (m_pRenderOffsetBone)
	{
		m_RenderOffset = -GetBoneModelPosition( m_pRenderOffsetBone );
		// TODO: make a multiplier set in the SetRenderOffset msg so we can choose which components to mask out.
		// Currently we mask out y because we needed to choose a node in the pelvis to center our physics on.
		m_RenderOffset.SetY( 0 );

		if (m_bResetRenderOffsetDelta)
		{
			m_PrevFrameRenderOffset = GetBoneGlobalPositionNoTranslation( m_pRenderOffsetBone, false );
			m_DeltaRenderOffset.Set( 0,0,0 );
			m_bResetRenderOffsetDelta = false;
		}
		else
		{
			Vec3 v = GetBoneGlobalPositionNoTranslation( m_pRenderOffsetBone, false );
			m_DeltaRenderOffset = (v - m_PrevFrameRenderOffset);
			m_PrevFrameRenderOffset = v;
		}
	}
}
