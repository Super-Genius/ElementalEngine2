///==========================================================================
/// \file	CTerrainDecalRenderObject.cpp
/// \brief	Implementation of CTerrainDecalRenderObject for Terrain
/// \date	04-24-2005
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
#include "ShaderConstants.h"
#include "PerformanceProfiler.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

REGISTER_COMPONENT(CTerrainDecalRenderObject);
REGISTER_MESSAGE_HANDLER(SetPosition, OnSetPosition, CTerrainDecalRenderObject);
REGISTER_MESSAGE_HANDLER(SetDecalTexture, OnSetDecalTexture, CTerrainDecalRenderObject);
REGISTER_MESSAGE_HANDLER(SetDecalSize, OnSetDecalSize, CTerrainDecalRenderObject);
REGISTER_MESSAGE_HANDLER(SetDecalColor, OnSetDecalColor, CTerrainDecalRenderObject);

IComponent *CTerrainDecalRenderObject::Create(int nArgs, va_list argptr)
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
			self = new CTerrainDecalRenderObject(parentName, name, true);
		}
		else if (nArgs == 3)
		{
			bAddToHier = va_arg(argptr, bool);
			self = new CTerrainDecalRenderObject(parentName, name, bAddToHier);
		}
	}
	catch(...)
	{
		return NULL;
	}

	return self;
}

CTerrainDecalRenderObject::CTerrainDecalRenderObject(IHashString *parentName, IHashString *name, bool bAddToHier)
:	CRenderObject<>(_T("CTerrainManager"), _T("CTerrainDecalRenderObject"), parentName, name)
{
	m_bTransformIsDirty = true;
	m_bAddToHierarchy = bAddToHier;
	if (bAddToHier)
	{
		AddToHierarchy();
	}

	CHashString TMTypeName(_T("CTerrainManager"));
	m_pTerrainMgr = dynamic_cast<CTerrainManager*>(m_ToolBox->GetComponent(&TMTypeName));

	GETRENDERERINTERFACEPARAMS renderInterface;
	static DWORD msgHash_GetRendererInterface = CHashString(_T("GetRendererInterface")).GetUniqueID();
	if (m_ToolBox->SendMessage(msgHash_GetRendererInterface, sizeof(GETRENDERERINTERFACEPARAMS), &renderInterface))
		m_pRenderer = renderInterface.m_RendererInterface;
	else
		m_pRenderer = NULL;

//	m_VertexBufferInterface = NULL;
//	m_IndexBufferInterface = NULL;

	m_pDecalEffect = NULL;
	m_pDecalTexture = NULL;

	m_fSize = 1.0f;
	m_fBoundingSphereRadius = m_fSize * 0.5f;
	m_Color.Set( 1.0f, 1.0f, 1.0f, 1.0f );

	m_fLingerTime = -1.0f;
	m_fFadeTime = 1.0f;
	m_fDecalET = 0.0f;
}

CTerrainDecalRenderObject::~CTerrainDecalRenderObject()
{
	// remove bounding boxed object
	RemoveBoundingObject();

	static DWORD msgHash_ParentRemoveBoundingBox = CHashString(_T("ParentRemoveBoundingBox")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_ParentRemoveBoundingBox, 0, NULL, &m_ParentName);

/*	static DWORD msgHash_RemoveVertexAllocation = CHashString(_T("RemoveVertexAllocation")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_RemoveVertexAllocation, sizeof(m_VertexBufferAllocation), &m_VertexBufferAllocation );
	m_VertexBufferAllocation.m_Size = 0;
	static DWORD msgHash_RemoveIndexAllocation = CHashString(_T("RemoveIndexAllocation")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_RemoveIndexAllocation, sizeof(m_IndexBufferAllocation), &m_IndexBufferAllocation );
	m_IndexBufferAllocation.m_Size = 0;
*/
	DeInit();

	if (m_bAddToHierarchy)
	{
		RemoveFromHierarchy();
	}
}

void CTerrainDecalRenderObject::DeInit()
{	
	// Remove Decal Texture
	if (m_pDecalTexture)
	{
		TEXTUREOBJECTPARAMS tobj;
		tobj.bLoad = false;
		tobj.Name = &m_szDecalTexture;
		tobj.TextureObjectInterface = NULL;
		static DWORD msgHash_RemoveTexture = CHashString(_T("RemoveTexture")).GetUniqueID();
		m_ToolBox->SendMessage(msgHash_RemoveTexture, sizeof(tobj), &tobj );
		m_pDecalTexture = NULL;
		m_szDecalTexture = _T("");
	}
}

void CTerrainDecalRenderObject::Init()
{
	static CHashString DecalName(_T("shaders/TerrainDecal.efx"));
	// Load Decal Effect
	if (!m_pDecalEffect)
		m_pDecalEffect = LoadEffect( &DecalName );

	// Load Decal Texture
	if (!m_pDecalTexture)
		m_pDecalTexture = dynamic_cast<ITextureObject*>( LoadTexture( m_szDecalTexture.GetString() ) );
}

void CTerrainDecalRenderObject::Serialize(IArchive &ar)
{
	IArchive *pArchive = &ar;
	if (pArchive->IsReading())
	{
		StdString szDecalTexture;
		pArchive->Read( szDecalTexture, _T("Texture") );
		pArchive->Read( m_fSize, _T("Size") );
		pArchive->Read( m_Color, _T("Color") );
		pArchive->Read( m_fLingerTime, _T("LingerTime") );
		pArchive->Read( m_fFadeTime, _T("FadeTime") );
		m_szDecalTexture = szDecalTexture;
	}
	else
	{
		pArchive->Write( m_szDecalTexture.GetString(), _T("Texture") );
		pArchive->Write( m_fSize, _T("Size") );
		pArchive->Write( m_Color, _T("Color") );
		pArchive->Write( m_fLingerTime, _T("LingerTime") );
		pArchive->Write( m_fFadeTime, _T("FadeTime") );
	}
}

bool CTerrainDecalRenderObject::Update()
{
	if (m_bTransformIsDirty)
 	{
		m_bTransformIsDirty = false;

		// set sector position
		Matrix4x4 tempTransform;
		static DWORD msgHash_GetGlobalTransform = CHashString(_T("GetGlobalTransform")).GetUniqueID();
		if (m_ToolBox->SendMessage(msgHash_GetGlobalTransform, sizeof(Matrix4x4), &tempTransform, GetParentName() ) == MSG_HANDLED)
		{
			SetPosition( Vec3(tempTransform.m[12], tempTransform.m[13], tempTransform.m[14]) );
			tempTransform.SetScale( Vec3(1.0,1.0,1.0) );
			m_TexTransform.SetFrom4x4( tempTransform.GetMatrix() );
		}
 	}

	if (m_fLingerTime != -1.0f)
	{
		m_fDecalET += m_Timer->GetTimeDelta();
		if (m_fDecalET > m_fLingerTime)
		{
			if (m_fDecalET > m_fLingerTime + m_fFadeTime)
			{
				// kill self
				DELETEOBJECTPARAMS dop;
				dop.name = GetName();
				DWORD msgDelayedDeleteObject = CHashString(_T("DelayedDeleteObject")).GetUniqueID();				

				m_ToolBox->SendMessage(msgDelayedDeleteObject, sizeof(DELETEOBJECTPARAMS), &dop);
			}
			else
			{
				float fadeVal = m_fDecalET - m_fLingerTime;
				
				if (m_fFadeTime == 0.0f)
				{
					fadeVal = 0.0f;
				}
				else
				{
					fadeVal = 1.0f - (fadeVal / m_fFadeTime);					
				}
				
				m_Color.w = fadeVal;
			}
		}
	}

	return true;
}

bool CTerrainDecalRenderObject::Render( UINT pass, IEffect * override )
{
	if (pass != 0)
		return false;

	if( !m_pDecalEffect )
	{
		return false;
	}

	// get sectors affected by cursor
	std::list<IHashString*> SectorList;
	GETAFFECTEDOBJECTSPARAMS info;
	info.fRadius = m_fBoundingSphereRadius;
	info.pPosition = &m_vPosition;
	info.pAffectedObjectsList = &SectorList;
	static DWORD msgHash_GetAffectedTerrainSectors = CHashString(_T("GetAffectedTerrainSectors")).GetUniqueID();
	if (m_ToolBox->SendMessage(msgHash_GetAffectedTerrainSectors, sizeof(info), &info ) != MSG_HANDLED)
		return false;

	Matrix4x4 texTransformMat;
	texTransformMat.SetFrom3x3( m_TexTransform.GetMatrix() );

	m_pDecalEffect->Apply( pass,NULL, NULL );	
	m_pDecalEffect->SetVertexConstant( 20, m_Color.x, m_Color.y, m_Color.z, m_Color.w );
	m_pDecalEffect->SetVertexConstant( 22, texTransformMat.GetMatrix(), 4 );
	m_pRenderer->SetDepthWrite( false );
	m_pRenderer->SetDepthTest( true );
	m_pRenderer->SetAlphaTest( false );
	m_pRenderer->SetRenderState( RENDERSTATE_ALPHABLENDENABLE, RENDERSTATEPARAM_TRUE );
	m_pRenderer->SetRenderState( RENDERSTATE_CULLMODE, RENDERSTATEPARAM_CULLREGULAR );
	m_pRenderer->SetTexture( 0, m_pDecalTexture );

	for (list<IHashString*>::iterator itr = SectorList.begin(); itr != SectorList.end(); itr++)
	{
		TERRAINSECTORGETPOLYDATA tsgpd;
		static DWORD msgHash_TerrainSectorGetPolyData = CHashString(_T("TerrainSectorGetPolyData")).GetUniqueID();
		if (m_ToolBox->SendMessage(msgHash_TerrainSectorGetPolyData, sizeof(tsgpd), &tsgpd, *itr ) != MSG_HANDLED)
			continue;

		TERRAINSECTORPOSITIONPARAMS tsgp;
		static DWORD msgHash_TerrainSectorGetPosition = CHashString(_T("TerrainSectorGetPosition")).GetUniqueID();
		if (m_ToolBox->SendMessage(msgHash_TerrainSectorGetPosition, sizeof(tsgp), &tsgp, *itr ) != MSG_HANDLED)
			continue;

		Matrix4x4 worldTransform;
		worldTransform.SetTranslation( tsgp.SectorCorner );
		m_pRenderer->SetMatrix( WORLD_MATRIX, worldTransform.GetMatrix() );

		Vec3 tadd = (tsgp.SectorCorner - m_vPosition) / tsgp.fSectorSize;
		m_pDecalEffect->SetVertexConstant( 21, tadd.x, tadd.z, tsgp.fSectorSize, m_fSize );

		m_pRenderer->RenderIndexBuffer( tsgpd.indexInterface, tsgpd.vertexInterface, 0, tsgpd.iIndexCount/3 );
	}

	m_pRenderer->SetDepthWrite( true );
	m_pRenderer->SetDepthTest( true );

	return true;
}

void CTerrainDecalRenderObject::SetPosition( const Vec3 &vec )
{
	m_vPosition = vec;

	UpdateBoundingObject();
}

Vec3 &CTerrainDecalRenderObject::GetPosition()
{
	return m_vPosition;
}

void CTerrainDecalRenderObject::GetBoundingSphere( Vec3 &position, float &radius )
{
	position = m_vPosition;;
	radius = m_fBoundingSphereRadius;
}

DWORD CTerrainDecalRenderObject::OnSetPosition( DWORD size, void* params )
{
	VERIFY_MESSAGE_SIZE( size, sizeof(Vec3) );
	Vec3 *pParams = (Vec3*)params;
	SetPosition( *pParams );

	// if we're explicitly setting position, transform is not dirty
	m_bTransformIsDirty = false;

	return MSG_HANDLED_STOP;
}

DWORD CTerrainDecalRenderObject::OnSetDecalTexture( DWORD size, void* params )
{
	VERIFY_MESSAGE_SIZE( size, sizeof(IHashString) );
	IHashString *pParams = (IHashString*)params;
	m_szDecalTexture = pParams->GetString();

	if (m_pDecalTexture)
	{
		if (m_pDecalTexture->GetTextureName()->GetUniqueID() != m_szDecalTexture.GetUniqueID())
		{
			// Remove Decal Texture
			TEXTUREOBJECTPARAMS tobj;
			tobj.bLoad = false;
			tobj.Name = NULL;
			tobj.TextureObjectInterface = m_pDecalTexture;
			static DWORD msgHash_RemoveTexture = CHashString(_T("RemoveTexture")).GetUniqueID();
			m_ToolBox->SendMessage(msgHash_RemoveTexture, sizeof(tobj), &tobj );
			m_pDecalTexture = NULL;
		}
	}

	// load decal texture
	if (!m_pDecalTexture)
		m_pDecalTexture = dynamic_cast<ITextureObject*>( LoadTexture( m_szDecalTexture.GetString() ) );

	return MSG_HANDLED_STOP;
}

DWORD CTerrainDecalRenderObject::OnSetDecalSize( DWORD size, void* params )
{
	VERIFY_MESSAGE_SIZE( size, sizeof(float) );
	float *pParams = (float*)params;
	m_fSize = *pParams;

	// if this relation is always true, why do we store it in a different variable?
	m_fBoundingSphereRadius = m_fSize * 0.5f;

	return MSG_HANDLED_STOP;
};

DWORD CTerrainDecalRenderObject::OnSetDecalColor( DWORD size, void* params )
{
	VERIFY_MESSAGE_SIZE( size, sizeof(Vec4) );
	Vec4 *pParams = (Vec4*)params;
	m_Color = *pParams;

	return MSG_HANDLED_STOP;
};

bool CTerrainDecalRenderObject::IsKindOf( IHashString * compType )
{	
	return (compType->GetUniqueID() == GetComponentType()->GetUniqueID());
}
/*
bool CTerrainDecalRenderObject::AllocateVertices( int iVertexCount )
{
	if (m_VertexBufferAllocation.m_Size >= iVertexCount)
		return true;

	CHANNELDESCRIPTORLIST VertexFormat;
	VertexFormat.push_back( MESH_STREAM_VERTEX_POSITION3 );
	VertexFormat.push_back( MESH_STREAM_VERTEX_TEXTURECOORD1 );
	VertexFormat.push_back( MESH_STREAM_VERTEX_TEXTURECOORD2 );
	VertexFormat.push_back( MESH_STREAM_VERTEX_NORMAL );
	VertexFormat.push_back( MESH_STREAM_VERTEX_COLORCRGBA );
	VertexFormat.push_back( MESH_STREAM_VERTEX_TEXTURECOORD3 );

	m_VertexBufferAllocation.VertexFormat = VertexFormat;
	// remove previous allocation
	if (m_VertexBufferAllocation.m_Size != 0)
	{
		static DWORD msgHash_RemoveVertexAllocation = CHashString(_T("RemoveVertexAllocation")).GetUniqueID();
		m_ToolBox->SendMessage(msgHash_RemoveVertexAllocation, sizeof(m_VertexBufferAllocation), &m_VertexBufferAllocation );
		m_VertexBufferAllocation.m_Size = 0;
	}

	// create new internal buffers
	SAFE_DELETE_ARRAY( m_pVertexData );
	m_pVertexData = new Vec3[iVertexCount];
	m_iVertexCount = iVertexCount;

	//create new allocation
	ALLOCATERENDERBUFFERMSG msg;
	msg.ElementsToAllocate = iVertexCount;
	msg.SizeVertexFormat = sizeof( TERRAINVERTEX );
	msg.VertexFormat = VertexFormat;
	msg.m_AllowSharedBuffer = false;
	static DWORD msgHash_AllocateVertices = CHashString(_T("AllocateVertices")).GetUniqueID();
	DWORD res = m_ToolBox->SendMessage(msgHash_AllocateVertices, sizeof(msg), &msg );

	if (res == MSG_HANDLED)
	{
		m_VertexBufferAllocation.m_Offset = msg.m_ReturnOffset;
		m_VertexBufferAllocation.m_AllocationHandle = msg.m_ReturnHandle;
		m_VertexBufferAllocation.m_InterfaceHandle = msg.m_ReturnInterfaceHandle;
		m_VertexBufferAllocation.m_Size = iVertexCount;
		m_VertexBufferAllocation.m_Stride = msg.SizeVertexFormat;
		m_VertexBufferInterface = (IVertexBufferObject*)msg.m_InterfacePointer;
		return true;
	}
	else
	{	
		m_VertexBufferInterface = NULL;
		return false;
	}
}

bool CTerrainDecalRenderObject::AllocateIndices( int iIndexCount )
{
	if (m_IndexBufferAllocation.m_Size >= iIndexCount)
		return true;

	// remove previous allocation
	if (m_IndexBufferAllocation.m_Size != 0)
	{
		static DWORD msgHash_RemoveIndexAllocation = CHashString(_T("RemoveIndexAllocation")).GetUniqueID();
		m_ToolBox->SendMessage(msgHash_RemoveIndexAllocation, sizeof(m_IndexBufferAllocation), &m_IndexBufferAllocation );
		m_IndexBufferAllocation.m_Size = 0;
	}

	//create new allocation
	ALLOCATERENDERBUFFERMSG msg;
	msg.m_AllowSharedBuffer = false;
	msg.ElementsToAllocate = iIndexCount;
	msg.SizeVertexFormat = sizeof(unsigned short);

	static DWORD msgHash_AllocateIndices = CHashString(_T("AllocateIndices")).GetUniqueID();
	DWORD res = m_ToolBox->SendMessage(msgHash_AllocateIndices, sizeof( msg ), &msg, NULL, NULL );
	if( res == MSG_HANDLED )
	{
		m_IndexBufferAllocation.m_Offset = msg.m_ReturnOffset;
		m_IndexBufferAllocation.m_AllocationHandle = msg.m_ReturnHandle;
		m_IndexBufferAllocation.m_InterfaceHandle = msg.m_ReturnInterfaceHandle;
		m_IndexBufferAllocation.m_Size = iIndexCount;
		m_IndexBufferAllocation.m_Stride = sizeof(unsigned short);
		m_IndexBufferInterface = (IIndexBuffer*)msg.m_InterfacePointer;
		return true;
	}
	else
	{
		m_IndexBufferInterface = NULL;
		return false;
	}
}
*/
