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

#include "StdAfx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

REGISTER_COMPONENT(CAppertureGenRenderObject);
REGISTER_MESSAGE_HANDLER(GenerateAppertureTexture, OnGenerateAppertureTexture, CAppertureGenRenderObject);


IComponent *CAppertureGenRenderObject::Create(int nArgs, va_list argptr)
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
			self = new CAppertureGenRenderObject(parentName, name, true);
		}
		else if (nArgs == 3)
		{
			bAddToHier = va_arg(argptr, bool);
			self = new CAppertureGenRenderObject(parentName, name, bAddToHier) ;
		}
	}
	catch(...)
	{
		return NULL;
	}

	return self;
}

CAppertureGenRenderObject::CAppertureGenRenderObject(IHashString *parentName, IHashString *name, bool bAddToHier) : 
	CRenderObject<>( _T("CRenderManager"), _T("CAppertureGenRenderObject"), parentName, name),
	m_hszRenderTargetCameraName(_T("AppertureGen_RenderTargetCamera"))
{
	m_IRenderer = NULL;
	m_pEffect = NULL;
	m_pSourceTexture = NULL;
	m_pZeroTexture = NULL;
	m_pAccumRenderTargets[0] = m_pAccumRenderTargets[1] = NULL;
	m_TextureGenerationVBI = NULL;
	m_bRenderTargetActive = false;

	m_bAddToHierarchy = bAddToHier;
	if (bAddToHier)
	{
		AddToHierarchy();
	}

	m_bTransformIsDirty = true;

	m_pZeroTexture = LoadTexture( _T("Textures/black_zeroalpha.dds") );
}

CAppertureGenRenderObject::~CAppertureGenRenderObject()
{
	DeInit();
	if (m_bAddToHierarchy)
	{
		RemoveFromHierarchy();
	}
}

void CAppertureGenRenderObject::Init()
{
	GETRENDERERINTERFACEPARAMS renderInterfaceParam;
	static DWORD msgHash_GetRendererInterface = CHashString(_T("GetRendererInterface")).GetUniqueID();
	if (m_ToolBox->SendMessage(msgHash_GetRendererInterface, sizeof(GETRENDERERINTERFACEPARAMS), &renderInterfaceParam ) == MSG_HANDLED)
		m_IRenderer = renderInterfaceParam.m_RendererInterface;

	SCENEOBJECTPARAMS ubbp;
	ubbp.pObject = this;
	ubbp.sortID = RENDEROBJECT3D;
	static DWORD msgHash_UpdateBoundingBox = CHashString("UpdateBoundingBox").GetUniqueID();
	m_ToolBox->SendMessage(msgHash_UpdateBoundingBox, sizeof(SCENEOBJECTPARAMS), &ubbp );

	// create render target camera
	CREATEOBJECTPARAMS cop;
	cop.name = &m_hszRenderTargetCameraName;
	cop.parentName = NULL;
	static DWORD msgHash_CreateCamera = CHashString(_T("CreateCamera")).GetUniqueID();
	DWORD res = m_ToolBox->SendMessage(msgHash_CreateCamera, sizeof(cop), &cop );
	if( res == MSG_HANDLED )
	{
		//change active camera here to ortho view!
		DWORD projection = ORTHO_CAMERA;
		static DWORD msgHash_SetProjection = CHashString(_T("SetProjection")).GetUniqueID();
		m_ToolBox->SendMessage(msgHash_SetProjection, sizeof(DWORD), &projection, &m_hszRenderTargetCameraName, &CHashString(_T("CCamera")) );
	}

	CreateTextureGenerationGeometry();

	m_pEffect = LoadEffect( &CHashString(_T("shaders/AppertureTextureGen.efx")) );
}

bool CAppertureGenRenderObject::Update()
{
	/*
	static DWORD msgHash_RenderListToTexture = CHashString(_T("RenderListToTexture")).GetUniqueID();

	static DWORD msgHash_GetTime = CHashString(_T("GetTime")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_GetTime,sizeof(float), &m_fCurrentTime );
	float fTimeDelta = m_fCurrentTime - m_fPreviousAnimationTime;
	const float fMaxFramerate = 1.0f / 15.0f;

	if (fTimeDelta > fMaxFramerate)
	{
		// generate animated heightmap textures
		m_bRenderTargetActive = true;
		OBJECTLIST renderList;
		renderList.push_back( this );
		RENDERLISTTOTEXTUREPARAMS rlttp;
		rlttp.dwSortMethod = 0;
		rlttp.objList = &renderList;
		rlttp.Camera = &m_hszRenderTargetCameraName;

		m_fRenderingHeightmapAnimSpeed = 0.13f * 8.0f;
		rlttp.RenderTexture = m_pHeightmapRenderTarget0;
		rlttp.effectOverride = m_pHeightmapGenerationEffect;
		m_ToolBox->SendMessage(msgHash_RenderListToTexture, sizeof(rlttp), &rlttp );

		rlttp.RenderTexture = m_pdXYRenderTarget0;
		rlttp.effectOverride = m_pdXYGenerationEffect;
		m_ToolBox->SendMessage(msgHash_RenderListToTexture, sizeof(rlttp), &rlttp );

		m_fRenderingHeightmapAnimSpeed = 0.19f * 8.0f;
		rlttp.RenderTexture = m_pHeightmapRenderTarget1;
		rlttp.effectOverride = m_pHeightmapGenerationEffect;
		m_ToolBox->SendMessage(msgHash_RenderListToTexture, sizeof(rlttp), &rlttp );

		rlttp.RenderTexture = m_pdXYRenderTarget1;
		rlttp.effectOverride = m_pdXYGenerationEffect;
		m_ToolBox->SendMessage(msgHash_RenderListToTexture, sizeof(rlttp), &rlttp );

		m_bRenderTargetActive = false;
		m_fPreviousAnimationTime = m_fCurrentTime;
	}

	if( m_bTransformIsDirty == true )
	{
		static DWORD msgHash_GetGlobalTransform = CHashString(_T("GetGlobalTransform")).GetUniqueID();
		m_ToolBox->SendMessage(msgHash_GetGlobalTransform, sizeof(Matrix4x4), &m_bParentTransform, GetParentName() );
	}
	*/

	return true;
}

void CAppertureGenRenderObject::DeInit()
{
	DestroyTextures();
	DestroyTextureGenerationGeometry();

	SCENEOBJECTPARAMS ubbp;
	ubbp.pObject = this;
	ubbp.sortID = RENDEROBJECT3D;
	static DWORD msgHash_RemoveBoundingBoxedObject = CHashString("RemoveBoundingBoxedObject").GetUniqueID();
	m_ToolBox->SendMessage(msgHash_RemoveBoundingBoxedObject, sizeof( SCENEOBJECTPARAMS), &ubbp );
}

void CAppertureGenRenderObject::Serialize(IArchive &ar)
{
	if(ar.IsReading())
	{
		
	}
	else
	{
		
	}
}

DWORD CAppertureGenRenderObject::OnGenerateAppertureTexture( DWORD size, void * params )
{
	VERIFY_MESSAGE_SIZE( size, sizeof(GENERATEAPPERTURETEXTUREPARAMS) );
	GENERATEAPPERTURETEXTUREPARAMS *p = (GENERATEAPPERTURETEXTUREPARAMS*)params;

	m_pSourceTexture = p->pSourceTexture;
	m_iRenderTargetSize = p->iDestTextureSize;
	m_fOffset = p->fOffset;
	m_fFlatten = p->fFlatten;
	GenerateAppertureTexture( p->pDestTextureName );

	return MSG_HANDLED_STOP;
}

void CAppertureGenRenderObject::GenerateAppertureTexture( IHashString *pDestTextureName )
{
	static DWORD msgHash_RenderListToTexture = CHashString(_T("RenderListToTexture")).GetUniqueID();

	CreateTextures(m_iRenderTargetSize);

	m_bRenderTargetActive = true;

	const int iPassCount = 1024;
	m_iAccumRenderTarget = 0;

	m_IRenderer->SetBackgroundColor( 0, 0, 0, 0 );

	for (int i=0; i < iPassCount; i++)
	{
		m_iAccumPass = i;
		m_iAccumRenderTarget = !m_iAccumRenderTarget;

		OBJECTLIST renderList;
		renderList.push_back( this );
		RENDERLISTTOTEXTUREPARAMS rlttp;
		rlttp.dwSortMethod = 0;
		rlttp.objList = &renderList;
		rlttp.Camera = &m_hszRenderTargetCameraName;
		rlttp.RenderTexture = m_pAccumRenderTargets[m_iAccumRenderTarget];
		rlttp.effectOverride = m_pEffect;
		m_ToolBox->SendMessage(msgHash_RenderListToTexture, sizeof(rlttp), &rlttp );
	}

	m_bRenderTargetActive = false;


	// create output texture
	CHashString hszOutTextureName( pDestTextureName->GetString() );

	CREATETEXTUREPARAMS createtextureparams;
	createtextureparams.bitDepth = 32;
	createtextureparams.Name = &hszOutTextureName;
	createtextureparams.sizeX = m_iRenderTargetSize;
	createtextureparams.sizeY = m_iRenderTargetSize;
	static DWORD msgHash_CreateTexture = CHashString(_T("CreateTexture")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_CreateTexture, sizeof(createtextureparams), &createtextureparams );
	ITextureObject *pOutTexture = dynamic_cast<ITextureObject*>( createtextureparams.TextureObjectInterface );

	unsigned char *pOutTextureBuffer = new unsigned char[m_iRenderTargetSize*m_iRenderTargetSize*4];
	float *pAccumTextureData = (float*)m_pAccumRenderTargets[m_iAccumRenderTarget]->Read(0, true);

	// convert accumulation buffer to a format suitable for file output
	// unify the normals and normalize the occlusion term by the number of lights
	if (pAccumTextureData)
	{
		const float fPassCount = (float)iPassCount;
		unsigned char *pCurrOut = pOutTextureBuffer;
		float *pCurrAccum = pAccumTextureData;
		int iTexelCount = m_iRenderTargetSize * m_iRenderTargetSize;
		for (int i=0; i < iTexelCount; i++)
		{
			Vec3 p( pCurrAccum[0], pCurrAccum[1], pCurrAccum[2] );
			p.Normalize();
			float f0 = p.x * 0.5f + 0.5f;
			float f1 = p.y * 0.5f + 0.5f;
			float f2 = p.z * 0.5f + 0.5f;
			float f3 = 1.0f - pCurrAccum[3] / fPassCount;

			pCurrOut[0] = (unsigned char)(f0 * 255.0f);
			pCurrOut[1] = (unsigned char)(f1 * 255.0f);
			pCurrOut[2] = (unsigned char)(f2 * 255.0f);
			pCurrOut[3] = (unsigned char)(f3 * 255.0f);

			pCurrAccum += 4;
			pCurrOut += 4;
		}
	}

	pOutTexture->Write( pOutTextureBuffer );
	SAFE_DELETE_ARRAY( pOutTextureBuffer );

//	static CHashString dxtc(_T("DXT5"));
//	static DWORD msgHash_SetSaveAsFormat = CHashString(_T("SetSaveAsFormat")).GetUniqueID();
//	m_ToolBox->SendMessage( msgHash_SetSaveAsFormat, sizeof(IHashString), &dxtc );
	static DWORD msgHash_SaveFileByExtension = CHashString(_T("SaveFileByExtension")).GetUniqueID();
	m_ToolBox->SendMessage( msgHash_SaveFileByExtension, sizeof(LPTSTR), (void*)hszOutTextureName.GetString() );
}

bool CAppertureGenRenderObject::Render( UINT pass, IEffect * override )
{
	if (!m_IRenderer || pass != 0)
		return false;

	if (m_bRenderTargetActive)
	{
		float fRTSize = (float)m_iRenderTargetSize;

		Matrix4x4 worldTransform;
		worldTransform.SetIdentity();
		m_IRenderer->SetMatrix( WORLD_MATRIX, worldTransform.GetMatrix() );

		m_IRenderer->SetMaterial( 0, NULL );
		m_pEffect->Apply( 0, NULL, NULL );
		m_IRenderer->SetAlphaTest( false );
		m_IRenderer->SetDepthTest( false );
		m_IRenderer->SetDepthWrite( false );
		m_IRenderer->SetRenderState( RENDERSTATE_ALPHABLENDENABLE, RENDERSTATEPARAM_FALSE );
		m_IRenderer->SetRenderState( RENDERSTATE_CULLMODE, RENDERSTATEPARAM_CULLNONE );

		m_IRenderer->SetTexture( 0, m_pSourceTexture );
		if (m_iAccumPass == 0)
			m_IRenderer->SetTexture( 1, m_pZeroTexture );
		else
			m_IRenderer->SetTexture( 1, m_pAccumRenderTargets[!m_iAccumRenderTarget] );

		Vec3 dir;
		if (m_iAccumPass == 0){
			dir.Set(0, 0, 1);
		} else {
			// Generate points on surface of hemisphere
			float d;
			do {
				dir.x = float(rand()) / RAND_MAX * 2.0f - 1.0f;
				dir.y = float(rand()) / RAND_MAX * 2.0f - 1.0f;
				dir.z = float(rand()) / RAND_MAX;

				d = dir.Dot(dir);
			} while (d < 0.9f * 0.9f || d > 1.1f * 1.1f);
		}
		// Flatten the bumps a bit
		dir.z *= m_fFlatten;
		dir.Normalize();
		dir /= fRTSize;

		m_pEffect->SetPixelConstant( 20, dir.x, dir.y, dir.z, m_fOffset );

		m_IRenderer->RenderVertexBuffer( m_TextureGenerationVBI, m_TextureGenerationVB.m_Offset, m_TextureGenerationVB.m_Size/3 );

		m_IRenderer->SetDepthTest( true );
		m_IRenderer->SetDepthWrite( true );

		return true;
	}
	return false;
}

bool CAppertureGenRenderObject::IsLessThan( const IRenderObject * obj, DWORD sortMethod, int curRenderPass )
{
	return true;
}

DWORD CAppertureGenRenderObject::GetRenderPriority()
{
    return 0;
}

UINT CAppertureGenRenderObject::GetNumPass()
{
	return 1;
}

void CAppertureGenRenderObject::GetBoundingSphere( Vec3 &position, float &radius )
{
	position.Set( 0,0,0 );
	radius = 999999.0f;
}

IHashString * CAppertureGenRenderObject::GetBaseSortClass()
{
	return GetComponentType();
}

bool CAppertureGenRenderObject::IsAlphaObject()
{
	return false;
}

IMeshObject* CAppertureGenRenderObject::GetMeshObject()
{
	return NULL;
}

/*
void CAppertureGenRenderObject::RenderHeightmapAnimations( float fAnimSpeed, IEffect *pEffect )
{
	static CHashString alphablend(_T("alphablend")); 
	static CHashString hszFalse(_T("false")); 
	static CHashString cullmode(_T("cullmode"));
	static CHashString cullnone(_T("cullnone"));

	float fAnimTime = m_fCurrentTime*fAnimSpeed;
	float fTimeFloor = floor(fAnimTime);
	float fAnimLerp = fAnimTime - fTimeFloor;
	int iFrame0 = (int)fAnimTime % OCEAN_HEIGHTMAPTEXTURE_COUNT;
	int iFrame1 = (iFrame0+1) % OCEAN_HEIGHTMAPTEXTURE_COUNT;

	Matrix4x4 worldTransform;
	m_IRenderer->SetMatrix( WORLD_MATRIX, worldTransform.GetMatrix() );

	m_IRenderer->SetMaterial( 0, NULL );
	m_IRenderer->SetAlphaTest( false );
	m_IRenderer->SetDepthTest( false );
	m_IRenderer->SetDepthWrite( false );
	m_IRenderer->SetRenderState( alphablend.GetUniqueID(), hszFalse.GetUniqueID() );
	m_IRenderer->SetRenderState( cullmode.GetUniqueID(), cullnone.GetUniqueID() );

	pEffect->Apply( 0, NULL, NULL );
	pEffect->SetPixelConstant( 21, fAnimLerp, 0.0f, 0.0f, 0.0f );

	m_IRenderer->SetTexture( 0, m_pHeightmapTextures[iFrame0] );
	m_IRenderer->SetTexture( 1, m_pHeightmapTextures[iFrame1] );

	m_IRenderer->RenderVertexBuffer( m_TextureGenerationVBI, m_TextureGenerationVB.m_Offset, m_TextureGenerationVB.m_Size/3 );

	m_IRenderer->SetDepthTest( true );
	m_IRenderer->SetDepthWrite( true );
}

void CAppertureGenRenderObject::CreateRadialGrid()
{
	DestroyRadialGrid();

	//create new allocation
	CHANNELDESCRIPTORLIST VertexFormat;
	VertexFormat.push_back( MESH_STREAM_VERTEX_POSITION3 );

	const float fRadialResolution = (float)OCEAN_RADIALGRID_RES;
	const int iVertexCount = OCEAN_DISTANCEGRID_RES*OCEAN_RADIALGRID_RES;
	int iIndexCount = (OCEAN_DISTANCEGRID_RES-1)*(OCEAN_RADIALGRID_RES+1)*2;

	ALLOCATERENDERBUFFERMSG arbm;
	arbm.ElementsToAllocate = iVertexCount;
	arbm.SizeVertexFormat = sizeof(Vec3);
	arbm.VertexFormat = VertexFormat;
	arbm.Dynamic = false;
	arbm.m_AllowSharedBuffer = false;

	static DWORD msgHash_AllocateVertices = CHashString(_T("AllocateVertices")).GetUniqueID();
	DWORD res = EngineGetToolBox()->SendMessage(msgHash_AllocateVertices, sizeof(arbm), &arbm );
	if (res != MSG_HANDLED)
		return;

	m_VertexBufferAllocation.m_Offset = arbm.m_ReturnOffset;
	m_VertexBufferAllocation.m_AllocationHandle = arbm.m_ReturnHandle;
	m_VertexBufferAllocation.m_InterfaceHandle = arbm.m_ReturnInterfaceHandle;
	m_VertexBufferAllocation.m_Size = arbm.ElementsToAllocate;
	m_VertexBufferAllocation.m_Stride = arbm.SizeVertexFormat;
	m_VertexBuffer = (IVertexBufferObject*)arbm.m_InterfacePointer;

	Vec3 *pVertex = NULL;
	if (!m_VertexBuffer->Lock( m_VertexBufferAllocation.m_Offset, iVertexCount, (void**)&pVertex ))
		return ;

	int iVertex=0;
	for (int j=0; j < OCEAN_DISTANCEGRID_RES; j++)
	{
		for (int i=0; i < OCEAN_RADIALGRID_RES; i++)
		{
			float A = ((float)i*(float)M_PI*2.0f) / fRadialResolution;
			float dX = cos(A), dY = sin(A);
			pVertex[iVertex++].Set(dX, dY, (float)j);
		}
	}
	m_VertexBuffer->Unlock();

	arbm.m_AllowSharedBuffer = false;
	arbm.ElementsToAllocate = iIndexCount;
	arbm.SizeVertexFormat = sizeof(unsigned short);

	static DWORD msgHash_AllocateIndices = CHashString(_T("AllocateIndices")).GetUniqueID();
	res = m_ToolBox->SendMessage(msgHash_AllocateIndices, sizeof(arbm), &arbm );
	if (res != MSG_HANDLED)
		return;
	m_IndexBufferAllocation.m_Offset = arbm.m_ReturnOffset;
	m_IndexBufferAllocation.m_AllocationHandle = arbm.m_ReturnHandle;
	m_IndexBufferAllocation.m_InterfaceHandle = arbm.m_ReturnInterfaceHandle;
	m_IndexBufferAllocation.m_Size = arbm.ElementsToAllocate;
	m_IndexBufferAllocation.m_Stride = arbm.SizeVertexFormat;
	m_IndexBuffer = (IIndexBuffer*)arbm.m_InterfacePointer;

	int iIndex = 0;
	unsigned short *pIndices = NULL;
	m_IndexBuffer->Lock( m_IndexBufferAllocation.m_Offset, iIndexCount, (void**)&pIndices );
	if (pIndices)
	{
		for(j = 0; j < OCEAN_DISTANCEGRID_RES-1; j++)
		{
			for (int i = 0; i < OCEAN_RADIALGRID_RES+1; i++)
			{
				int index0 = (j)*OCEAN_RADIALGRID_RES+((i)%OCEAN_RADIALGRID_RES);
				int index1 = (j+1)*OCEAN_RADIALGRID_RES+((i)%OCEAN_RADIALGRID_RES);
				pIndices[iIndex++] = index0;  pIndices[iIndex++] = index1;
			}
		}
	}
	m_IndexBuffer->Unlock();
}

void CAppertureGenRenderObject::DestroyRadialGrid()
{
	static DWORD msgHash_RemoveVertexAllocation = CHashString(_T("RemoveVertexAllocation")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_RemoveVertexAllocation, sizeof(m_VertexBufferAllocation), &m_VertexBufferAllocation );
	m_VertexBufferAllocation.m_Size = 0;
	static DWORD msgHash_RemoveIndexAllocation = CHashString(_T("RemoveIndexAllocation")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_RemoveIndexAllocation, sizeof(m_IndexBufferAllocation), &m_IndexBufferAllocation );
	m_IndexBufferAllocation.m_Size = 0;
}
*/
void CAppertureGenRenderObject::CreateTextures( int iRenderTargetSize )
{
	static DWORD msgHash_GenerateUniqueObjectName = CHashString(_T("GenerateUniqueObjectName")).GetUniqueID();
	static DWORD msgHash_CreateTexture = CHashString(_T("CreateTexture")).GetUniqueID();

	DestroyTextures();

	// generate unique names
	CHashString hszRenderTargetName(_T("AppertureGen_RenderTargetTex"));
	GENERATEUNIQUEOBJECTNAMEPARAMS generateNameParams;
	CHashString tempName;
	generateNameParams.name = &hszRenderTargetName;

	CHashString hszRenderTargetName0;
	generateNameParams.newname = &hszRenderTargetName0;
	if (m_ToolBox->SendMessage(msgHash_GenerateUniqueObjectName, sizeof(generateNameParams), &generateNameParams ) != MSG_HANDLED)
		return;
	CHashString hszRenderTargetName1;
	generateNameParams.newname = &hszRenderTargetName1;
	if (m_ToolBox->SendMessage(msgHash_GenerateUniqueObjectName, sizeof(generateNameParams), &generateNameParams ) != MSG_HANDLED)
		return;

	// create render targets
	CREATETEXTUREPARAMS createtex;
	createtex.bitDepth = 128;
	createtex.sizeX = iRenderTargetSize;
	createtex.sizeY = iRenderTargetSize;
	// accum0
	createtex.Name = &hszRenderTargetName0;
	createtex.bRenderTargetTexture = RENDER_TARGET_FLOATINGPOINT;
	m_ToolBox->SendMessage(msgHash_CreateTexture, sizeof(createtex), &createtex );
	if( createtex.TextureObjectInterface )
		m_pAccumRenderTargets[0] = dynamic_cast< ITextureObject*>( createtex.TextureObjectInterface );
	// accum1
	createtex.Name = &hszRenderTargetName1;
	createtex.bRenderTargetTexture = RENDER_TARGET_FLOATINGPOINT;
	m_ToolBox->SendMessage(msgHash_CreateTexture, sizeof(createtex), &createtex );
	if( createtex.TextureObjectInterface )
		m_pAccumRenderTargets[1] = dynamic_cast< ITextureObject*>( createtex.TextureObjectInterface );
}

void CAppertureGenRenderObject::DestroyTextures()
{
	static DWORD msgHash_RemoveTexture = CHashString(_T("RemoveTexture")).GetUniqueID();

	// remove render target accum
	TEXTUREOBJECTPARAMS tobj;
	tobj.bLoad = false;
	tobj.Name = NULL;
	// accum0
	tobj.TextureObjectInterface = m_pAccumRenderTargets[0];
	m_ToolBox->SendMessage(msgHash_RemoveTexture, sizeof(tobj), &tobj );
	m_pAccumRenderTargets[0] = NULL;
	// accum1
	tobj.TextureObjectInterface = m_pAccumRenderTargets[1];
	m_ToolBox->SendMessage(msgHash_RemoveTexture, sizeof(tobj), &tobj );
	m_pAccumRenderTargets[1] = NULL;
}

void CAppertureGenRenderObject::CreateTextureGenerationGeometry()
{
	DestroyTextureGenerationGeometry();

	struct tempvert
	{
		float pos[3];
		float tex0[2];
	};
	tempvert verts[ 6 ] = 
	{
		{ 0,0,0, 0,1 },
		{ 1,0,0, 1,1 },
		{ 1,1,0, 1,0 },
		{ 0,0,0, 0,1 },
		{ 1,1,0, 1,0 },
		{ 0,1,0, 0,0 },
	};

	CHANNELDESCRIPTORLIST VertexFormat;
	VertexFormat.push_back( MESH_STREAM_VERTEX_POSITION3 );
	VertexFormat.push_back( MESH_STREAM_VERTEX_TEXTURECOORD1 );

	//create new allocation
	ALLOCATERENDERBUFFERMSG msg;
	msg.m_AllowSharedBuffer = false;

	int totalVertices = 6;
	msg.ElementsToAllocate = totalVertices;
	msg.SizeVertexFormat = sizeof( tempvert );
	msg.VertexFormat = VertexFormat;

	static DWORD msgHash_AllocateVertices = CHashString(_T("AllocateVertices")).GetUniqueID();
	DWORD res = m_ToolBox->SendMessage(msgHash_AllocateVertices, sizeof( msg ), &msg );
	if( res == MSG_HANDLED )
	{
		m_TextureGenerationVB.m_Offset = msg.m_ReturnOffset;
		m_TextureGenerationVB.m_AllocationHandle = msg.m_ReturnHandle;
		m_TextureGenerationVB.m_InterfaceHandle = msg.m_ReturnInterfaceHandle;
		m_TextureGenerationVB.m_Size =	totalVertices;
		m_TextureGenerationVB.m_Stride = msg.SizeVertexFormat;
		m_TextureGenerationVBI = (IVertexBufferObject*)msg.m_InterfacePointer;
	}

	void *pVertex;
	//Lock the vertex buffer and index buffer
	if( m_TextureGenerationVBI && 
		!m_TextureGenerationVBI->Lock( m_TextureGenerationVB.m_Offset,
		totalVertices, (void**)&pVertex ) )
	{
		return;
	}

	memcpy( pVertex, verts, sizeof( tempvert ) * totalVertices );
	m_TextureGenerationVBI->Unlock();
}

void CAppertureGenRenderObject::DestroyTextureGenerationGeometry()
{
	if (m_TextureGenerationVBI)
	{
		static DWORD msgHash_RemoveVertexAllocation = CHashString(_T("RemoveVertexAllocation")).GetUniqueID();
		m_ToolBox->SendMessage( msgHash_RemoveVertexAllocation, sizeof(m_TextureGenerationVB), &m_TextureGenerationVB );
		m_TextureGenerationVBI = NULL;
	}
}
