///============================================================================
/// \file		Skybox.cpp
/// \brief		Implementation of CSkyboxObject Class
/// \date		08-16-2005
/// \author		Brian Bazyk
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

#include "StdAfx.h"
#include "Matrix4x4.h"
#include "IEffect.h"

#include "PerformanceProfiler.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

REGISTER_COMPONENT(CSkyboxObject);
REGISTER_MESSAGE_HANDLER(SetZOffset, OnSetZOffset, CSkyboxObject);
REGISTER_MESSAGE_HANDLER(GetSkyboxTexture, OnGetSkyboxTexture, CSkyboxObject);
REGISTER_MESSAGE_HANDLER(SetSkyboxTexture, OnSetSkyboxTexture, CSkyboxObject);

IComponent *CSkyboxObject::Create(int nArgs, va_list argptr)
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
			self = new CSkyboxObject(parentName, name, true);
		}
		else if (nArgs == 3)
		{
			bAddToHier = va_arg(argptr, bool);
			self = new CSkyboxObject(parentName, name, bAddToHier);
		}
	}
	catch(...)
	{
		return NULL;
	}

	return self;
}

CSkyboxObject::CSkyboxObject(IHashString *parentName, IHashString *name, bool bAddToHier) : 
	m_hszNameType( _T("CSkyboxObject") ),
	SCENEOBJECTTEMPLATE(CRenderManager, CSkyboxObject, IRenderObject, parentName, name)
{
	m_bAddToHierarchy = bAddToHier;
	if (bAddToHier)
	{
		AddToHierarchy();
	}

	//grab the renderer
	GETRENDERERINTERFACEPARAMS renderInterfaceParam;
	renderInterfaceParam.m_RendererInterface = NULL;
	static DWORD msgHash_GetRendererInterface = CHashString(_T("GetRendererInterface")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_GetRendererInterface, sizeof(GETRENDERERINTERFACEPARAMS), &renderInterfaceParam, NULL, NULL );
	m_IRenderer = renderInterfaceParam.m_RendererInterface;

	// create shader callback
	m_SkyboxShaderCallback = new CSkyboxShaderCallback();
	m_IndexBufferInterface = NULL;
	m_VertexBufferInterface = NULL;

	m_pSkyboxTexture = NULL;
	m_pSkyboxEffect = NULL;
	m_fYOffset = 0.0f;

	m_iMeshSize = 8;

	GenerateSkyBox();
}

CSkyboxObject::~CSkyboxObject()
{
	DeInit();
	if (m_bAddToHierarchy)
	{
		RemoveFromHierarchy();
	}

	if (m_VertexBufferInterface)
	{
		static DWORD msgHash_RemoveVertexAllocation = CHashString(_T("RemoveVertexAllocation")).GetUniqueID();
		m_ToolBox->SendMessage( msgHash_RemoveVertexAllocation, sizeof(m_VertexBufferAllocation), &m_VertexBufferAllocation );
		m_VertexBufferInterface = NULL;
	}
	if (m_IndexBufferInterface)
	{
		static DWORD msgHash_RemoveIndexAllocation = CHashString(_T("RemoveIndexAllocation")).GetUniqueID();
		m_ToolBox->SendMessage( msgHash_RemoveIndexAllocation, sizeof(m_IndexBufferAllocation), &m_IndexBufferAllocation );
		m_IndexBufferInterface = NULL;
	}

	SAFE_DELETE( m_SkyboxShaderCallback );
}

void CSkyboxObject::Init()
{
	DeInit();

	//load the effect
	LOADFILEEXTPARAMS lfep;
	m_EffectPath.MakeSafeFileName();
	CHashString filename( m_EffectPath );

	lfep.fileName = (TCHAR*)m_EffectPath.c_str();
	lfep.bInternalLoad = true;
	static DWORD msgHash_LoadFileByExtension = CHashString(_T("LoadFileByExtension")).GetUniqueID();
	DWORD retval = EngineGetToolBox()->SendMessage(msgHash_LoadFileByExtension, sizeof(LOADFILEEXTPARAMS), &lfep);
	if( retval == MSG_HANDLED )
	{
		CREATEEFFECTINTERFACE cef;
		cef.m_Name = &filename;
		static DWORD msgHash_GetEffectInterface = CHashString(_T("GetEffectInterface")).GetUniqueID();
		EngineGetToolBox()->SendMessage(msgHash_GetEffectInterface, sizeof(CREATEEFFECTINTERFACE ), &cef );
		m_pSkyboxEffect = cef.m_EffectInterface;
		if( !m_pSkyboxEffect )
		{
			EngineGetToolBox()->Log( LOGERROR, _T("Could not load Skybox's Effect\n" ) );
		}
	}	

	LoadTextures();

	UpdateGlobalObject();
}

void CSkyboxObject::GenerateSkyBox()
{
	const float SkyboxDimensions = 1.0f;

	SKYCUBEVERTEX SkyVerts[] = {

		{ -SkyboxDimensions, +SkyboxDimensions, +SkyboxDimensions,-1,+1,+1 },
		{ -SkyboxDimensions, -SkyboxDimensions, +SkyboxDimensions,-1,-1,+1 },
		{ -SkyboxDimensions, +SkyboxDimensions, -SkyboxDimensions,-1,+1,-1 },
		{ -SkyboxDimensions, -SkyboxDimensions, -SkyboxDimensions,-1,-1,-1 },
			
		{ -SkyboxDimensions, +SkyboxDimensions, -SkyboxDimensions,-1,+1,-1 },
		{ -SkyboxDimensions, -SkyboxDimensions, -SkyboxDimensions,-1,-1,-1 },
		{ +SkyboxDimensions, +SkyboxDimensions, -SkyboxDimensions,+1,+1,-1 },
		{ +SkyboxDimensions, -SkyboxDimensions, -SkyboxDimensions,+1,-1,-1 },

		{ +SkyboxDimensions, +SkyboxDimensions, -SkyboxDimensions,+1,+1,-1 },
		{ +SkyboxDimensions, -SkyboxDimensions, -SkyboxDimensions,+1,-1,-1 },
		{ +SkyboxDimensions, +SkyboxDimensions, +SkyboxDimensions,+1,+1,+1 },
		{ +SkyboxDimensions, -SkyboxDimensions, +SkyboxDimensions,+1,-1,+1 },
		
		{ +SkyboxDimensions, +SkyboxDimensions, +SkyboxDimensions,+1,+1,+1 },
		{ +SkyboxDimensions, -SkyboxDimensions, +SkyboxDimensions,+1,-1,+1 },
		{ -SkyboxDimensions, +SkyboxDimensions, +SkyboxDimensions,-1,+1,+1 },
		{ -SkyboxDimensions, -SkyboxDimensions, +SkyboxDimensions,-1,-1,+1 },

		{ -SkyboxDimensions, +SkyboxDimensions, -SkyboxDimensions,-1,+1,-1 },
		{ -SkyboxDimensions, +SkyboxDimensions, +SkyboxDimensions,-1,+1,+1 },
		{ +SkyboxDimensions, +SkyboxDimensions, -SkyboxDimensions,+1,+1,-1 },
		{ +SkyboxDimensions, +SkyboxDimensions, +SkyboxDimensions,+1,+1,+1 },

		{ -SkyboxDimensions, -SkyboxDimensions, -SkyboxDimensions,-1,-1,-1 },
		{ -SkyboxDimensions, -SkyboxDimensions, +SkyboxDimensions,-1,-1,+1 },
		{ +SkyboxDimensions, -SkyboxDimensions, -SkyboxDimensions,+1,-1,-1 },
		{ +SkyboxDimensions, -SkyboxDimensions, +SkyboxDimensions,+1,-1,+1 }
	};

	short SkyIndices[] = {
		0,2,1, 1,2,3,
		4,6,5, 5,6,7,
		8,10,9, 9,10,11,
		12,14,13, 13,14,15,
		16,17,18, 17,19,18,
		20,22,21, 21,22,23
	};

	CHANNELDESCRIPTORLIST VertexFormat;
	VertexFormat.push_back( MESH_STREAM_VERTEX_POSITION3 );
	VertexFormat.push_back( MESH_STREAM_VERTEX_CUBECOORD1 );

	//create new allocation
	ALLOCATERENDERBUFFERMSG msg;
	msg.m_AllowSharedBuffer = false;

	int totalVertices = 24;
	msg.ElementsToAllocate = totalVertices;
	msg.SizeVertexFormat = sizeof( SKYCUBEVERTEX );
	msg.VertexFormat = VertexFormat;

	static DWORD msgHash_AllocateVertices = CHashString(_T("AllocateVertices")).GetUniqueID();
	DWORD res = m_ToolBox->SendMessage(msgHash_AllocateVertices, sizeof( msg ), &msg );
	if( res == MSG_HANDLED )
	{
		m_VertexBufferAllocation.m_Offset = msg.m_ReturnOffset;
		m_VertexBufferAllocation.m_AllocationHandle = msg.m_ReturnHandle;
		m_VertexBufferAllocation.m_InterfaceHandle = msg.m_ReturnInterfaceHandle;
		m_VertexBufferAllocation.m_Size =	totalVertices;
		m_VertexBufferAllocation.m_Stride = msg.SizeVertexFormat;
		m_VertexBufferInterface = (IVertexBufferObject*)msg.m_InterfacePointer;
	}

	//create new allocation
	int totalIndices = 36;
	msg.ElementsToAllocate = totalIndices;
	msg.SizeVertexFormat = sizeof(short);
	static DWORD msgHash_AllocateIndices = CHashString(_T("AllocateIndices")).GetUniqueID();
	res = m_ToolBox->SendMessage(msgHash_AllocateIndices, sizeof( msg ), &msg, NULL, NULL );
	if( res == MSG_HANDLED )
	{
		m_IndexBufferAllocation.m_Offset = msg.m_ReturnOffset;
		m_IndexBufferAllocation.m_AllocationHandle = msg.m_ReturnHandle;
		m_IndexBufferAllocation.m_InterfaceHandle = msg.m_ReturnInterfaceHandle;
		m_IndexBufferAllocation.m_Size = totalIndices;
		m_IndexBufferAllocation.m_Stride = sizeof(		short		 );
		m_IndexBufferInterface = (IIndexBuffer*)msg.m_InterfacePointer;
	}

	void *pVertex;
	short *pFace;
	//Lock the vertex buffer and index buffer
	if( m_VertexBufferInterface && 
		!m_VertexBufferInterface->Lock( m_VertexBufferAllocation.m_Offset,
		totalVertices, (void**)&pVertex ) )
	{
		return;
	}
	if( m_IndexBufferInterface && 
		!m_IndexBufferInterface->Lock( m_IndexBufferAllocation.m_Offset,
		totalIndices, (void**)&pFace ) )
	{
		m_VertexBufferInterface->Unlock();
		return;
	}

	memcpy( pVertex,	SkyVerts, sizeof( SKYCUBEVERTEX ) * totalVertices );
	memcpy( pFace,		SkyIndices, sizeof( short ) * totalIndices );				
	m_VertexBufferInterface->Unlock();
	m_IndexBufferInterface->Unlock();
}

void CSkyboxObject::DeInit()
{
	UnloadTextures();

	// remove bounding boxed object
	RemoveBoundingObject();

	static DWORD msgHash_ParentRemoveBoundingBox = CHashString(_T("ParentRemoveBoundingBox")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_ParentRemoveBoundingBox, 0, NULL, &m_ParentName);
}

void CSkyboxObject::Serialize(IArchive &ar)
{
	if(ar.IsReading())
	{
		ar.Read( m_EffectPath, "EffectPath" );
		ar.Read( m_TexturePath, "TexturePath" );
		ar.Read( m_fYOffset, "ZOffset" );
	}
	else
	{
		ar.Write( m_EffectPath, "EffectPath" );
		ar.Write( m_TexturePath, "TexturePath" );
		ar.Write( m_fYOffset, "ZOffset" );
	}
}

bool CSkyboxObject::Render( UINT pass, IEffect * override )
{
	if (!m_IRenderer || pass != 0)
		return false;

	if( !m_VertexBufferInterface || !m_IndexBufferInterface )
		return false;
	if (m_pSkyboxEffect == NULL)
		return false;

	Matrix4x4 worldTransform;
	const float fScale = 10.0f; // arbitrary value just to keep it from hitting the near clip plane
	m_IRenderer->SetMatrix( WORLD_MATRIX, worldTransform.GetMatrix() );
	m_IRenderer->SetMaterial( 0, NULL );
	m_IRenderer->SetAlphaTest( false );
	m_IRenderer->SetDepthTest( false );
	m_IRenderer->SetDepthWrite( false );
	m_IRenderer->SetRenderState( RENDERSTATE_ALPHABLENDENABLE, RENDERSTATEPARAM_FALSE );
	m_IRenderer->SetRenderState( RENDERSTATE_CULLMODE, RENDERSTATEPARAM_CULLNONE );
	m_pSkyboxEffect->Apply( pass, NULL, NULL );
	m_pSkyboxEffect->SetVertexConstant( 23, fScale, m_fYOffset, 0, 0 );
	m_IRenderer->SetTexture( 0, m_pSkyboxTexture );
	m_IRenderer->RenderIndexBuffer( m_IndexBufferInterface, m_VertexBufferInterface, 0, m_IndexBufferAllocation.m_Size/3 );

	m_IRenderer->SetDepthWrite( true );
	m_IRenderer->SetDepthTest( true );
	return true;
}

bool CSkyboxObject::IsLessThan( const IRenderObject * obj, DWORD sortMethod, int curRenderPass )
{
	return ( obj > this );
}

DWORD CSkyboxObject::GetRenderPriority()
{
	return 999999999;
}

void CSkyboxObject::SetPosition( const Vec3 &vec )
{
}

Vec3 &CSkyboxObject::GetPosition()
{
	static Vec3 zero(0,0,0);
	return zero;
}

IMeshObject* CSkyboxObject::GetMeshObject()
{
	return NULL;
}


bool CSkyboxObject::IsKindOf( IHashString * compType )
{	
	return (compType->GetUniqueID() == m_hszNameType.GetUniqueID() );
}

void CSkyboxObject::GenerateSphere()
{
	int numVertices = (2+m_iMeshSize*m_iMeshSize*2);
    int numIndices = ((m_iMeshSize*4 + m_iMeshSize*4*(m_iMeshSize-1))*3);
	int i, j;
 
	m_Sphere = new VERTEX[numVertices];				// array of vertices
	m_SphereIndices = new int[numIndices];			// order of the vertices
	 
	float  dj = 3.14159265f/(m_iMeshSize+1.0f);
	float  di = 3.14159265f/m_iMeshSize;
	 
	// generate the sphere data, note the random texture coords
	// vertices 0 and 1 are the north and south poles, and are hardcoded as such in the code
	m_Sphere[0] = VERTEX(Vec3(0.0f, 1.0f * m_fSize, 0.0f), 
					   Vec3(0.0f, 1.0f, 0.0f), 0.0f, 0.0f);
	m_Sphere[1] = VERTEX(Vec3(0.0f, -1.0f * m_fSize, 0.0f), 
					   Vec3(0.0f, -1.0f, 0.0f), 1.0f, 1.0f);
	 
	for (j = 0; j < m_iMeshSize; ++j) 
	{
		for (i = 0; i < m_iMeshSize*2; ++i) 
		{
			Vec3 p;
			float u, v;
			// Generate the x,y,&z coordinates from the equation
			// mentioned in the text
			p.y = (float) cos((j+1) * dj);
			p.x = (float) sin(i * di) * (float) sin((j+1) * dj);
			p.z = (float) cos(i * di) * (float) sin((j+1) * dj);
	 
			// now assign the texture coordinates
			u = (float)i/m_iMeshSize;
			if (u > 1.0f)
			{
				u -= 1.0f;
			}
			u = 1.0f - u;    // flip so texture is not mirrored
			v = (float)j/m_iMeshSize;
			m_Sphere[2+i+j*m_iMeshSize*2] = VERTEX(Vec3(p.x * m_fSize,
														p.y * m_fSize,
														p.z * m_fSize), p, u, v);
		}
	}
	 
	// now generate the triangle indices strip around north pole
	for (i = 0; i < m_iMeshSize*2; ++i) 
	{
		m_SphereIndices[3*i] = 0;	 // always start tgl with the north pole
		m_SphereIndices[3*i+1] = i+2;
		m_SphereIndices[3*i+2] = i+3;
		if (i == (m_iMeshSize*2-1))
		{
			m_SphereIndices[3*i+2] = 2;
		}
	}
	 
	// now all the middle strips
	int v;     // vertex offset
	int ind;  // indices offset

	for (j = 0; j < m_iMeshSize-1; ++j) 
	{
		v = 2+j*m_iMeshSize*2;
		ind = 3*m_iMeshSize*2 + j*6*m_iMeshSize*2;
		for (i = 0; i < m_iMeshSize*2; ++i) 
		{
			m_SphereIndices[6*i+ind] = v+i;
			m_SphereIndices[6*i+2+ind] = v+i+1;
			m_SphereIndices[6*i+1+ind] = v+i+m_iMeshSize*2;
			m_SphereIndices[6*i+ind+3] = v+i+m_iMeshSize*2;
			m_SphereIndices[6*i+2+ind+3] = v+i+1;
			m_SphereIndices[6*i+1+ind+3] = v+i+m_iMeshSize*2+1;
			if (i == (m_iMeshSize*2-1)) 
			{
				m_SphereIndices[6*i+2+ind] = v+i+1-2*m_iMeshSize;
				m_SphereIndices[6*i+2+ind+3] = v+i+1-2*m_iMeshSize;
				m_SphereIndices[6*i+1+ind+3] = v+i+m_iMeshSize*2+1-2*m_iMeshSize;
			}
		}
	}
}

void CSkyboxObject::LoadTextures()
{
	UnloadTextures();

	CHashString hszTexturePath(m_TexturePath);
	TEXTUREOBJECTPARAMS top;
	top.Name = &hszTexturePath;
	top.TextureObjectInterface = NULL;
	top.bLoad = true;

	static DWORD msgAddTexture = CHashString(_T("AddTexture")).GetUniqueID();
	EngineGetToolBox()->SendMessage(msgAddTexture, sizeof(TEXTUREOBJECTPARAMS), &top);
	if (top.TextureObjectInterface != NULL)
	{
		m_pSkyboxTexture = top.TextureObjectInterface;
		static CSkyboxTextureCallback g_SkyboxTextureCallback;
		g_SkyboxTextureCallback.textureName = m_TexturePath;
		static DWORD msgHash_RegisterTextureCallback = CHashString(_T("RegisterTextureCallback")).GetUniqueID();
		m_ToolBox->SendMessage(msgHash_RegisterTextureCallback, sizeof(ITextureCallback), &g_SkyboxTextureCallback);
	}
}

void CSkyboxObject::UnloadTextures()
{
	TEXTUREOBJECTPARAMS tobj;
	tobj.bLoad = false;
	tobj.Name = NULL;
	tobj.TextureObjectInterface = m_pSkyboxTexture;
	static DWORD msgHash_RemoveTexture = CHashString(_T("RemoveTexture")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_RemoveTexture, sizeof(tobj), &tobj );
	m_pSkyboxTexture = NULL;
}

DWORD CSkyboxObject::OnSetZOffset(DWORD size, void *param)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(float));
	float offset = *(float*)param;
	m_fYOffset = offset;
	return MSG_HANDLED_STOP;
}

DWORD CSkyboxObject::OnGetSkyboxTexture(DWORD size, void *param)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(IHashString*));
	IHashString *pTextureNameParam = (IHashString*)param;
	*pTextureNameParam = m_TexturePath;
	return MSG_HANDLED_STOP;
}

DWORD CSkyboxObject::OnSetSkyboxTexture(DWORD size, void *param)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(float));
	IHashString *pTextureNameParam = (IHashString*)param;
	m_TexturePath = pTextureNameParam->GetString();
	LoadTextures();
	return MSG_HANDLED_STOP;
}
