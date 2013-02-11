///==========================================================================
/// \file	CTerrainWaterRenderObject.cpp
/// \brief	Implementation of CTerrainWaterRenderObject for Terrain
/// \date	07-27-2005
/// \author	Brian Bazyk
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
///==========================================================================

#include "StdAfx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

float SampleHeightMapBilinearly( float fX, float fY, const float *pHeightMap, int iHeightMapSize );

REGISTER_COMPONENT(CTerrainWaterRenderObject);
REGISTER_MESSAGE_HANDLER(TerrainSectorResetVertexData, OnTerrainSectorResetVertexData, CTerrainWaterRenderObject);

IComponent *CTerrainWaterRenderObject::Create(int nArgs, va_list argptr)
{
	IObject *self;

	IHashString *name = va_arg(argptr, IHashString *);
	IHashString *parentName = va_arg(argptr, IHashString *);
	bool bAddToHier;
	// create the object however you must create it
	try
	{
		// try and create an instance
		if (nArgs == 2)
		{
			self = new CTerrainWaterRenderObject(parentName, name, true);
		}
		else if (nArgs == 3)
		{
			bAddToHier = va_arg(argptr, bool);
			self = new CTerrainWaterRenderObject(parentName, name, bAddToHier);
		}
	}
	catch(...)
	{
		return NULL;
	}

	return self;
}

CTerrainWaterRenderObject::CTerrainWaterRenderObject(IHashString *parentName, IHashString *name, bool bAddToHier) : 
	CRenderObject<>(_T("CTerrainManager"), _T("CTerrainWaterRenderObject"), parentName, name)
{
	m_bTransformIsDirty = true;
	m_bAddToHierarchy = bAddToHier;
	if (bAddToHier)
	{
		AddToHierarchy();
	}

	static CHashString TMTypeName(_T("CTerrainManager"));
	m_pTerrainMgr = dynamic_cast<CTerrainManager*>(m_ToolBox->GetComponent(&TMTypeName));

	GETRENDERERINTERFACEPARAMS renderInterface;
	static DWORD msgHash_GetRendererInterface = CHashString(_T("GetRendererInterface")).GetUniqueID();
	if (m_ToolBox->SendMessage(msgHash_GetRendererInterface, sizeof(GETRENDERERINTERFACEPARAMS), &renderInterface))
		m_pRenderer = renderInterface.m_RendererInterface;
	else
		m_pRenderer = NULL;

	m_pTerrainWaterMaterial = NULL;

	m_Position.Set( 0, 0, 0 );
	m_CenterPosition.Set( 0, 0, 0 );
	m_fWaterLevel    = 0.0f;
	m_iTesselation   = 4;

	// scene settings
	m_SceneSettings = NULL;
	static DWORD msgHash_CreateSceneSettings = CHashString(_T("CreateSceneSettings")).GetUniqueID();
	if (m_ToolBox->SendMessage(msgHash_CreateSceneSettings, sizeof( ISceneSettings ), &m_SceneSettings ) != MSG_HANDLED)
	{
		m_ToolBox->Log( LOGERROR, _T("TerrainWaterRenderObject failed to create scene settings."));
		return;
	}
	m_SceneSettings->SetShaderFlags( 0 );

	// geometry
	m_pGeometry = NULL;
	static DWORD msgHash_CreateGeometry = CHashString(_T("CreateGeometry")).GetUniqueID();
	if (m_ToolBox->SendMessage(msgHash_CreateGeometry, sizeof( IGeometry** ), &m_pGeometry ) != MSG_HANDLED)
	{
		m_ToolBox->Log( LOGERROR, _T("TerrainWaterRenderObject failed to create geometry."));
		return;
	}
	m_pGeometry->SetSceneSettings( m_SceneSettings );
}

CTerrainWaterRenderObject::~CTerrainWaterRenderObject()
{
	DeInit();

	// destroy scene settings
	if( m_SceneSettings != NULL )
	{
		static DWORD msgHash_DestroySceneSettings = CHashString(_T("DestroySceneSettings")).GetUniqueID();
		m_ToolBox->SendMessage(msgHash_DestroySceneSettings, sizeof( ISceneSettings ), &m_SceneSettings );
		m_SceneSettings = NULL;
	}

	// destroy geometry
	if( m_pGeometry != NULL )
	{
		static DWORD msgHash_DestroyGeometry = CHashString(_T("DestroyGeometry")).GetUniqueID();
		m_ToolBox->SendMessage(msgHash_DestroyGeometry, sizeof( IGeometry** ), &m_pGeometry );
		m_pGeometry = NULL;
	}

	if (m_bAddToHierarchy)
	{
		RemoveFromHierarchy();
	}
}

void CTerrainWaterRenderObject::DeInit()
{
	// remove bounding boxed object
	RemoveBoundingObject();

	static DWORD msgHash_ParentRemoveBoundingBox = CHashString(_T("ParentRemoveBoundingBox")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_ParentRemoveBoundingBox, 0, NULL, &m_ParentName);
}

void CTerrainWaterRenderObject::Init()
{
	DeInit();

	m_fSize = m_pTerrainMgr->GetMetrics().fSectorSize;
	m_fBoundingSphereRadius = sqrt(m_fSize*m_fSize*2.0f) * 0.5f;
	m_CenterPosition.Set( m_Position.x + m_fSize * 0.5f, m_Position.y, m_Position.z + m_fSize * 0.5f );

	TERRAINSECTORGETPOLYDATA terrainSectorGetPolyData;
	static DWORD msgHash_TerrainSectorGetPolyData = CHashString(_T("TerrainSectorGetPolyData")).GetUniqueID();
	if( m_ToolBox->SendMessage(msgHash_TerrainSectorGetPolyData, sizeof(TERRAINSECTORGETPOLYDATA), &terrainSectorGetPolyData, GetParentName() ) != MSG_HANDLED )
	{
		m_ToolBox->Log( LOGERROR, _T("CTerrainWaterRenderObject: Failed to get Terrain Polygon data.\n") );
	}

	if( terrainSectorGetPolyData.pVertexData == NULL || terrainSectorGetPolyData.pIndexData == NULL ||
		terrainSectorGetPolyData.iVertexCount == 0 || terrainSectorGetPolyData.iIndexCount == 0 )
	{
		m_ToolBox->Log( LOGERROR, _T("CTerrainWaterRenderObject: Terrain Polygon data is invalid.\n") );
	}

	m_pGeometry->SetIndexBufferAllocation( terrainSectorGetPolyData.indexAllocation );
	m_pGeometry->SetVertexBufferAllocation( terrainSectorGetPolyData.vertexAllocation );


	CHashString SName(m_szShaderName);
	// Load Effects
	m_pTerrainWaterMaterial = LoadMaterial( &SName );
	m_pGeometry->SetMaterial( m_pTerrainWaterMaterial );

	m_bTransformIsDirty = true;

	DetachAllLights();
}

bool CTerrainWaterRenderObject::Update()
{
	if (m_bTransformIsDirty)
 	{
		// set sector position
		Vec3 tempPosition;
		static DWORD msgHash_GetGlobalPosition = CHashString(_T("GetGlobalPosition")).GetUniqueID();
		if (m_ToolBox->SendMessage(msgHash_GetGlobalPosition, sizeof(Vec3), &tempPosition, GetParentName() ) == MSG_HANDLED)
			SetPosition( Vec3(tempPosition.x, tempPosition.y+m_fWaterLevel, tempPosition.z) );
		else
			SetPosition( Vec3(0,m_fWaterLevel,0) );

		m_bTransformIsDirty = false;
	}
	return true;
}

void CTerrainWaterRenderObject::Serialize(IArchive &ar)
{
	if (ar.IsReading())
	{
		// read data
		ar.Read( m_fWaterLevel, _T("WaterLevel") );
		ar.Read( m_iTesselation, _T("Tesselation") );
		ar.Read( m_fTransparencyDepth, _T("TransparencyDepth") );
		ar.Read( m_szShaderName, _T("Shader") );

		if (m_iTesselation < 1)
			m_iTesselation = 1;
	}
	else
	{
		// write data
		ar.Write( m_fWaterLevel, _T("WaterLevel") );
		ar.Write( m_iTesselation, _T("Tesselation") );
		ar.Write( m_fTransparencyDepth, _T("TransparencyDepth") );
		ar.Write( m_szShaderName, _T("Shader") );
	}
}

bool CTerrainWaterRenderObject::Render( UINT pass, IEffect * override )
{
	if (pass != 0)
		return false;

	if (m_pTerrainWaterMaterial == NULL)
		return false;

	// get parent sector's extents
	Vec3 mins, maxs;
	TERRAINSECTOREXTENTSPARAMS extents;
	extents.pMins = &mins;
	extents.pMaxs = &maxs;
	static DWORD msgHash_TerrainSectorGetExtents = CHashString(_T("TerrainSectorGetExtents")).GetUniqueID();
	if (m_ToolBox->SendMessage(msgHash_TerrainSectorGetExtents, sizeof(extents), &extents, GetParentName() ) != MSG_HANDLED)
		return false;

	// if bottom of terrain is above our water level, do nothing
	if (mins.y >= m_Position.y)
		return false;

	Matrix4x4 worldTransform;
	worldTransform.SetTranslation( m_Position );
	m_pRenderer->SetMatrix( WORLD_MATRIX, worldTransform.GetMatrix() );

	static CHashString hszWaterInfoVC(_T("WaterInfo"));
	m_pTerrainWaterMaterial->SetVertexConstant( &hszWaterInfoVC, m_fTransparencyDepth, m_fWaterLevel, 0.0f, 0.0f );
	m_pGeometry->Render( m_SceneSettings );

	return true;
}

DWORD CTerrainWaterRenderObject::OnTerrainSectorResetVertexData(DWORD size, void *in_params)
{
	VERIFY_MESSAGE_SIZE( size, 0 );

	if (!m_pGeometry)
		return MSG_HANDLED_PROCEED;

	TERRAINSECTORGETPOLYDATA terrainSectorGetPolyData;
	static DWORD msgHash_TerrainSectorGetPolyData = CHashString(_T("TerrainSectorGetPolyData")).GetUniqueID();
	if( m_ToolBox->SendMessage(msgHash_TerrainSectorGetPolyData, sizeof(TERRAINSECTORGETPOLYDATA), &terrainSectorGetPolyData, GetParentName() ) != MSG_HANDLED )
	{
		m_ToolBox->Log( LOGERROR, _T("CTerrainWaterRenderObject: Failed to get Terrain Polygon data.\n") );
	}

	m_pGeometry->SetIndexBufferAllocation( terrainSectorGetPolyData.indexAllocation );
	m_pGeometry->SetVertexBufferAllocation( terrainSectorGetPolyData.vertexAllocation );

	return MSG_HANDLED_PROCEED;
}

DWORD CTerrainWaterRenderObject::GetRenderPriority()
{
	return 0;
}

void CTerrainWaterRenderObject::SetPosition( const Vec3 &vec )
{
	m_Position = vec;
	m_CenterPosition.Set( m_Position.x + m_fSize * 0.5f, m_Position.y, m_Position.z + m_fSize * 0.5f );

	UpdateBoundingObject();
}

Vec3 &CTerrainWaterRenderObject::GetPosition()
{
	return m_Position;
}

void CTerrainWaterRenderObject::GetBoundingSphere( Vec3 &position, float &radius )
{
	position = m_CenterPosition;
	radius = m_fBoundingSphereRadius;
}

void CTerrainWaterRenderObject::AttachLight( ILightObject * light )
{	
	if (light->GetLightType() == AMBIENT_LIGHT)
	{
		m_pGeometry->AttachLight( light );
	}
	else
	{
		float fPriority = 0.0f;
		bool bAffected = light->GetLightPriority( m_CenterPosition, m_fBoundingSphereRadius, fPriority );
		if( !bAffected )
			return;

		// find light slot to insert
		int i = 0;
		for( i = 0; i < m_NumLights; i++ )
		{
			// push back the rest of the lights
			if( (m_LightPriorities[ i ] < fPriority) )
			{
				for( int j = MAX_LIGHTS - 1 ; j > i ; j-- )
				{
					m_Lights[ j ] = m_Lights[ j-1 ];
					m_LightPriorities[ j ] = m_LightPriorities[ j-1 ];
				}
				break;
			}
		}

		// if no slot available
		if (i == MAX_LIGHTS)
			return;

		// insert light
		m_Lights[ i ] = light;
		m_LightPriorities[ i ] = fPriority;
		if (m_NumLights < MAX_LIGHTS)
			m_NumLights++;

		// set lights in geometry in priority correct order
		m_pGeometry->ResetLightCount();
		for (int a=0; a < m_NumLights; a++)
			m_pGeometry->AttachLight( m_Lights[a], a );

		// update scene settings
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
	}
}

void CTerrainWaterRenderObject::DetachAllLights()
{
	m_NumLights = 0;
	memset( m_Lights, 0, sizeof( m_Lights ) );
	memset( m_LightPriorities, 0, sizeof( m_LightPriorities ) );

	m_pGeometry->DetachAllLights();
}
