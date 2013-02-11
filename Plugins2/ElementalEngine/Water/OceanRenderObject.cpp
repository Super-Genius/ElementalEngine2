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

REGISTER_COMPONENT(COceanRenderObject);

REGISTER_MESSAGE_HANDLER( SetEnvMapTexture, OnSetEnvMapTexture, COceanRenderObject );

IComponent *COceanRenderObject::Create(int nArgs, va_list argptr)
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
			self = new COceanRenderObject(parentName, name, true);
		}
		else if (nArgs == 3)
		{
			bAddToHier = va_arg(argptr, bool);
			self = new COceanRenderObject(parentName, name, bAddToHier);
		}
	}
	catch(...)
	{
		return NULL;
	}

	return self;
}

COceanRenderObject::COceanRenderObject(IHashString *parentName, IHashString *name, bool bAddToHier) : 
	CRenderObject<>( _T("CWaterManager"), _T("COceanRenderObject"), parentName, name), 
	m_hszRenderTargetCameraName(_T("Ocean_RenderTargetCamera"))
{
	m_IRenderer = NULL;
	m_fCurrentTime = 0;
	m_fPreviousAnimationTime = 0;
	m_VertexBuffer = NULL;
	m_IndexBuffer = NULL;
	m_pEffect = NULL;
	memset( m_pHeightmapTextures, 0, sizeof(m_pHeightmapTextures) );
	m_pHeightmapRenderTarget0 = NULL;
	m_pHeightmapRenderTarget1 = NULL;
	m_pHeightmapGenerationEffect = NULL;
	m_bRenderTargetActive = false;
	m_pFoamTexture = NULL;
	m_pEnvMap = NULL;
	m_pFresnelLookup = NULL;

	m_TextureGenerationVBI = NULL;

	m_bAddToHierarchy = bAddToHier;
	if (bAddToHier)
	{
		AddToHierarchy();
	}

	m_bTransformIsDirty = true;
}

COceanRenderObject::~COceanRenderObject()
{
	DeInit();
	if (m_bAddToHierarchy)
	{
		RemoveFromHierarchy();
	}
}

void COceanRenderObject::Init()
{
	GETRENDERERINTERFACEPARAMS renderInterfaceParam;
	static DWORD msgHash_GetRendererInterface = CHashString(_T("GetRendererInterface")).GetUniqueID();
	if (m_ToolBox->SendMessage(msgHash_GetRendererInterface, sizeof(GETRENDERERINTERFACEPARAMS), &renderInterfaceParam ) == MSG_HANDLED)
		m_IRenderer = renderInterfaceParam.m_RendererInterface;

	UpdateGlobalObject();

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
		static CHashString CamTypeName(_T("CCamera"));
		m_ToolBox->SendMessage(msgHash_SetProjection, sizeof(DWORD), &projection, &m_hszRenderTargetCameraName, &CamTypeName );
	}

	CreateRadialGrid();
	CreateTextures();
	CreateTextureGenerationGeometry();

	static CHashString OWFileName(_T("shaders/OceanWater.efx"));
	static CHashString OHGFileName(_T("shaders/OceanHeightmapGeneration.efx"));
	m_pEffect = LoadEffect( &OWFileName );
	m_pHeightmapGenerationEffect = LoadEffect( &OHGFileName );
}

bool COceanRenderObject::Update()
{
	static DWORD msgHash_RenderListToTexture = CHashString(_T("RenderListToTexture")).GetUniqueID();

	m_fCurrentTime = m_Timer->GetTime();
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

		m_fRenderingHeightmapAnimSpeed = 0.19f * 8.0f;
		rlttp.RenderTexture = m_pHeightmapRenderTarget1;
		rlttp.effectOverride = m_pHeightmapGenerationEffect;
		m_ToolBox->SendMessage(msgHash_RenderListToTexture, sizeof(rlttp), &rlttp );

		m_bRenderTargetActive = false;
		m_fPreviousAnimationTime = m_fCurrentTime;
	}

	if( m_bTransformIsDirty == true )
	{
		static DWORD msgHash_GetGlobalTransform = CHashString(_T("GetGlobalTransform")).GetUniqueID();
		m_ToolBox->SendMessage(msgHash_GetGlobalTransform, sizeof(Matrix4x4), &m_bParentTransform, GetParentName() );
	}

	return true;
}

void COceanRenderObject::DeInit()
{
	DestroyTextures();
	DestroyRadialGrid();
	DestroyTextureGenerationGeometry();

	RemoveBoundingObject();
}

void COceanRenderObject::Serialize(IArchive &ar)
{
	if(ar.IsReading())
	{
		CHashString hszVersion = _T("");
		float version;
		static DWORD msgHash_GetFileVersion = CHashString(_T("GetFileVersion")).GetUniqueID();
		DWORD retval = m_ToolBox->SendMessage(msgHash_GetFileVersion, sizeof(IHashString), &hszVersion);
		if (retval != MSG_HANDLED)
		{
			m_ToolBox->Log(LOGERROR, _T("CEntity Serialize: Could not get file version!"));
			assert(0);
			return;
		}
		version = (float)_tstof(hszVersion.GetString());
		

		if( version >= 3.0 )
		{
			ar.Read( m_szFoamTexture, "FoamTexture" );
			ar.Read( m_szEnvMapTexture, "EnvMapTexture" );
			ar.Read( m_DiffuseColor, "DiffuseColor" );
			ar.Read( m_AmbientValue, "AmbientValue" );
			ar.Read( m_SunDir, "SunDir" );
			ar.Read( m_fClipScale, "ClipScale" );
			ar.Read( m_fClipOffset, "ClipOffset" );
			ar.Read( m_fTextureScale, "TextureScale" );
			ar.Read( m_fWindy, "Windy" );
		}
		else
		{
			Vec3 temp;
			ar.Read( m_szFoamTexture, "FoamTexture" );
			ar.Read( m_szEnvMapTexture, "EnvMapTexture" );
			ar.Read( m_DiffuseColor, "DiffuseColor" );
			ar.Read( m_AmbientValue, "AmbientValue" );
			ar.Read( temp, "SunDir" );
			m_SunDir.Set( temp.x, temp.z, -temp.y );
			ar.Read( m_fClipScale, "ClipScale" );
			ar.Read( m_fClipOffset, "ClipOffset" );
			ar.Read( m_fTextureScale, "TextureScale" );
			ar.Read( m_fWindy, "Windy" );
		}
	}
	else
	{
		ar.Write( m_szFoamTexture, "FoamTexture" );
		ar.Write( m_szEnvMapTexture, "EnvMapTexture" );
		ar.Write( m_DiffuseColor, "DiffuseColor" );
		ar.Write( m_AmbientValue, "AmbientValue" );
		ar.Write( m_SunDir, "SunDir" );
		ar.Write( m_fClipScale, "ClipScale" );
		ar.Write( m_fClipOffset, "ClipOffset" );
		ar.Write( m_fTextureScale, "TextureScale" );
		ar.Write( m_fWindy, "Windy" );
	}
}

void COceanRenderObject::RenderHeightmapAnimations( float fAnimSpeed, IEffect *pEffect )
{
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
	m_IRenderer->SetRenderState( RENDERSTATE_ALPHABLENDENABLE, RENDERSTATEPARAM_FALSE );
	m_IRenderer->SetRenderState( RENDERSTATE_CULLMODE, RENDERSTATEPARAM_CULLNONE );

	pEffect->Apply( 0, NULL, NULL );
	pEffect->SetPixelConstant( 21, fAnimLerp, 0.0f, 0.0f, 0.0f );

	m_IRenderer->SetTexture( 0, m_pHeightmapTextures[iFrame0] );
	m_IRenderer->SetTexture( 1, m_pHeightmapTextures[iFrame1] );
	m_IRenderer->SetColorMask( true, true, true, true );

	m_IRenderer->RenderVertexBuffer( m_TextureGenerationVBI, m_TextureGenerationVB.m_Offset, m_TextureGenerationVB.m_Size/3 );

	m_IRenderer->SetDepthTest( true );
	m_IRenderer->SetDepthWrite( true );
}

bool COceanRenderObject::Render( UINT pass, IEffect * override )
{
	if (!m_IRenderer || pass != 0)
		return false;

	if (m_bRenderTargetActive)
	{
		RenderHeightmapAnimations( m_fRenderingHeightmapAnimSpeed, override );
		return true;
	}

	if( !m_VertexBuffer || !m_IndexBuffer )
		return false;
	if (m_pEffect == NULL)
		return false;

	// setup constants
	Vec3 sunDir(m_SunDir);
	sunDir.Normalize();

	const float fLength = 128.0f;
	const float fScale = 1.0f / fLength;
	const float fMax = 36000.0f / 8.0f;
	const float fDetailHeightCutoff = 0.4f * fLength;
	const float fDetailHeightCutoffMin = fDetailHeightCutoff * 0.25f;

	Vec4 PosKs;
	PosKs.x = pow(fScale, 4) * fMax;
	PosKs.y = fDetailHeightCutoff;
	PosKs.z = fDetailHeightCutoffMin;
	PosKs.w = 0;

	Vec4 ClipKs;
	ClipKs.x = m_fClipScale;
	ClipKs.y = m_fClipOffset;
	ClipKs.z = 0;
	ClipKs.w = 0;

	float _S = (1.0f-m_fWindy)*0.5f + 0.25f;
	float H = 1.5f * 2.5f * _S;
	float fTexScale = 0.0025f*(1.0f+(0.5f-abs(m_fWindy-0.5f))*4.0f);

	Vec4 DMParameters;
	DMParameters.x = fTexScale * m_fTextureScale;
	DMParameters.y = OCEAN_RENDERTARGET_SIZE / 2.0f;
	DMParameters.z = H / m_fTextureScale;
	DMParameters.w = 1.0f / (fTexScale * m_fTextureScale);

	Vec4 MipKs;
	MipKs.x = 50.0f * m_fTextureScale;
	MipKs.y = 300.0f * m_fTextureScale;
	float zMax = ((m_pHeightmapRenderTarget0->GetLevelCount() - 1) > 0) ? 
		(float)(m_pHeightmapRenderTarget0->GetLevelCount() - 1) : 0.0f; 
	if (m_pHeightmapRenderTarget0)
		MipKs.z = zMax;
	else
		MipKs.z = 0.0f;
	MipKs.x = MipKs.x * MipKs.x; // square these outside the shader
	MipKs.y = MipKs.y * MipKs.y; // square these outside the shader
	MipKs.w = MipKs.z/(MipKs.y-MipKs.x); // precalc

//	Matrix4x4 worldTransform;
//	worldTransform.SetTranslation( m_Position );
///	m_IRenderer->SetMatrix( WORLD_MATRIX, worldTransform.GetMatrix() );
	m_IRenderer->SetMatrix( WORLD_MATRIX, m_bParentTransform.GetMatrix() );
	m_IRenderer->SetMaterial( 0, NULL );
	m_IRenderer->SetAlphaTest( true );
	m_IRenderer->SetRenderState( RENDERSTATE_ALPHAREF, 1 ); 
	m_IRenderer->SetDepthTest( true );
	m_IRenderer->SetDepthWrite( true );
	m_IRenderer->SetRenderState( RENDERSTATE_ALPHABLENDENABLE, RENDERSTATEPARAM_TRUE );
	m_IRenderer->SetRenderState( RENDERSTATE_CULLMODE, RENDERSTATEPARAM_CULLNONE );
	m_pEffect->Apply( pass, NULL, NULL );
	m_pEffect->SetVertexConstant( 23, PosKs.x, PosKs.y, PosKs.z, PosKs.w );
	m_pEffect->SetVertexConstant( 24, ClipKs.x, ClipKs.y, ClipKs.z, ClipKs.w );
	m_pEffect->SetVertexConstant( 25, DMParameters.x, DMParameters.y, DMParameters.z, DMParameters.w );
	m_pEffect->SetVertexConstant( 26, MipKs.x, MipKs.y, MipKs.z, MipKs.w );
	m_pEffect->SetPixelConstant( 23, m_DiffuseColor.x, m_DiffuseColor.y, m_DiffuseColor.z, m_AmbientValue );
	m_pEffect->SetPixelConstant( 24, sunDir.x, sunDir.y, sunDir.z, 0.0f );
	m_IRenderer->SetVertexTexture( 0, m_pHeightmapRenderTarget0 );
	m_IRenderer->SetVertexTexture( 1, m_pHeightmapRenderTarget1 );
	m_IRenderer->SetTexture( 0, m_pHeightmapRenderTarget0 );
	m_IRenderer->SetTexture( 1, m_pHeightmapRenderTarget1 );
	m_IRenderer->SetTexture( 2, m_pFoamTexture );
	m_IRenderer->SetTexture( 3, m_pEnvMap );
//	m_IRenderer->SetTexture( 4, m_pFresnelLookup );
	m_IRenderer->RenderIndexBuffer( m_IndexBuffer, m_VertexBuffer, 0, m_IndexBufferAllocation.m_Size-2, DP_TRIANGLESTRIP );

	m_IRenderer->SetVertexTexture( 0, NULL );
	m_IRenderer->SetVertexTexture( 1, NULL );
	m_IRenderer->SetTexture( 0, NULL );
	m_IRenderer->SetTexture( 1, NULL );

	return true;
}

bool COceanRenderObject::IsLessThan( const IRenderObject * obj, DWORD sortMethod, int curRenderPass )
{
	return true;
}

DWORD COceanRenderObject::GetRenderPriority()
{
    return 0;
}

UINT COceanRenderObject::GetNumPass()
{
	return 1;
}

IHashString * COceanRenderObject::GetBaseSortClass()
{
	return GetComponentType();
}

bool COceanRenderObject::IsAlphaObject()
{
	return false;
}

IMeshObject* COceanRenderObject::GetMeshObject()
{
	return NULL;
}

void COceanRenderObject::CreateRadialGrid()
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
		for(int j = 0; j < OCEAN_DISTANCEGRID_RES-1; j++)
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

void COceanRenderObject::DestroyRadialGrid()
{
	static DWORD msgHash_RemoveVertexAllocation = CHashString(_T("RemoveVertexAllocation")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_RemoveVertexAllocation, sizeof(m_VertexBufferAllocation), &m_VertexBufferAllocation );
	m_VertexBufferAllocation.m_Size = 0;
	static DWORD msgHash_RemoveIndexAllocation = CHashString(_T("RemoveIndexAllocation")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_RemoveIndexAllocation, sizeof(m_IndexBufferAllocation), &m_IndexBufferAllocation );
	m_IndexBufferAllocation.m_Size = 0;
}

void COceanRenderObject::CreateTextures()
{
	static DWORD msgHash_GenerateUniqueObjectName = CHashString(_T("GenerateUniqueObjectName")).GetUniqueID();
	static DWORD msgHash_CreateTexture = CHashString(_T("CreateTexture")).GetUniqueID();

	DestroyTextures();

	// load heightmaps
	for (int i=0; i < OCEAN_HEIGHTMAPTEXTURE_COUNT; i++)
	{
		TCHAR sz[MAX_PATH];
		_stprintf( sz, _T("Textures/Water/WaterH%02i.dds"), i );
		m_pHeightmapTextures[i] = LoadTexture( sz );
	}

	// generate unique names
	CHashString hszRenderTargetName0(_T("Ocean_RenderTargetTex"));
	CHashString hszRenderTargetName1(_T("Ocean_RenderTargetTex"));
	CHashString hszRenderTargetName2(_T("Ocean_RenderTargetTex"));
	CHashString hszRenderTargetName3(_T("Ocean_RenderTargetTex"));
	CHashString hszRenderLookup1(_T("Ocean_RenderLookupTex"));

	GENERATEUNIQUEOBJECTNAMEPARAMS generateNameParams;
	CHashString tempName;
	generateNameParams.newname = &tempName;

	generateNameParams.name = &hszRenderTargetName0;
	if (m_ToolBox->SendMessage(msgHash_GenerateUniqueObjectName, sizeof(generateNameParams), &generateNameParams ) != MSG_HANDLED)
		return;
	hszRenderTargetName0 = tempName;

	generateNameParams.name = &hszRenderTargetName1;
	if (m_ToolBox->SendMessage(msgHash_GenerateUniqueObjectName, sizeof(generateNameParams), &generateNameParams ) != MSG_HANDLED)
		return;
	hszRenderTargetName1 = tempName;

	generateNameParams.name = &hszRenderTargetName2;
	if (m_ToolBox->SendMessage(msgHash_GenerateUniqueObjectName, sizeof(generateNameParams), &generateNameParams ) != MSG_HANDLED)
		return;
	hszRenderTargetName2 = tempName;

	generateNameParams.name = &hszRenderTargetName3;
	if (m_ToolBox->SendMessage(msgHash_GenerateUniqueObjectName, sizeof(generateNameParams), &generateNameParams ) != MSG_HANDLED)
		return;
	hszRenderTargetName3 = tempName;

	generateNameParams.name = &hszRenderLookup1;
	if (m_ToolBox->SendMessage(msgHash_GenerateUniqueObjectName, sizeof(generateNameParams), &generateNameParams ) != MSG_HANDLED)
		return;
	hszRenderLookup1 = tempName;

	// create render targets
	CREATETEXTUREPARAMS createtex;
	createtex.bitDepth = 128;
	createtex.sizeX = OCEAN_RENDERTARGET_SIZE;
	createtex.sizeY = OCEAN_RENDERTARGET_SIZE;
	// hmap0
	createtex.Name = &hszRenderTargetName0;
	createtex.bRenderTargetTexture = RENDER_TARGET_COLOR_BUFFER;
	createtex.bAutoGenMipMaps = true;
	m_ToolBox->SendMessage(msgHash_CreateTexture, sizeof(createtex), &createtex );
	if( createtex.TextureObjectInterface )
		m_pHeightmapRenderTarget0 = dynamic_cast< ITextureObject*>( createtex.TextureObjectInterface );
	// hmap1
	createtex.Name = &hszRenderTargetName1;
	createtex.bRenderTargetTexture = RENDER_TARGET_COLOR_BUFFER;
	createtex.bAutoGenMipMaps = true;
	m_ToolBox->SendMessage(msgHash_CreateTexture, sizeof(createtex), &createtex );
	if( createtex.TextureObjectInterface )
		m_pHeightmapRenderTarget1 = dynamic_cast< ITextureObject*>( createtex.TextureObjectInterface );

	m_pFoamTexture = LoadTexture( m_szFoamTexture );
	m_pEnvMap = LoadTexture( m_szEnvMapTexture );

/*	// fresnel lookup texture
	const int iFresnelLookupSize = 128;
	createtex.Name = &hszRenderLookup1;
	createtex.bRenderTargetTexture = RENDER_TARGET_NONE;
	createtex.bitDepth = 8;
	createtex.sizeX = iFresnelLookupSize;
	createtex.sizeY = 1;
	m_ToolBox->SendMessage(msgHash_CreateTexture, sizeof(createtex), &createtex );
	if( createtex.TextureObjectInterface )
	{
		m_pFresnelLookup = dynamic_cast< ITextureObject*>( createtex.TextureObjectInterface );

		const float fresnelPower = 5.0f;
		const float fresnelBias = 0.0237f; // air to water boundary: 0.0237

		unsigned char data[iFresnelLookupSize];
		for (int i=0; i < iFresnelLookupSize; i++)
		{
			float facing = (float)i / (float)(iFresnelLookupSize-1);
			float value = fresnelBias + (1.0f-fresnelBias)*pow(facing, fresnelPower);
			value = min( value, 1.0f );
			value = max( value, 0.0f );
			data[i] = (unsigned char)(value * 255.0f);
		}
		m_pFresnelLookup->Write( data );
	}
*/
}

void COceanRenderObject::DestroyTextures()
{
	static DWORD msgHash_RemoveTexture = CHashString(_T("RemoveTexture")).GetUniqueID();

	// remove heightmaps
	TEXTUREOBJECTPARAMS tobj;
	tobj.bLoad = false;
	tobj.Name = NULL;
	for (int i=0; i < OCEAN_HEIGHTMAPTEXTURE_COUNT; i++)
	{
		tobj.TextureObjectInterface = m_pHeightmapTextures[i];
		m_ToolBox->SendMessage(msgHash_RemoveTexture, sizeof(tobj), &tobj );
		m_pHeightmapTextures[i] = NULL;
	}

	// remove render target hmap0
	tobj.TextureObjectInterface = m_pHeightmapRenderTarget0;
	m_ToolBox->SendMessage(msgHash_RemoveTexture, sizeof(tobj), &tobj );
	m_pHeightmapRenderTarget0 = NULL;
	// remove render target hmap1
	tobj.TextureObjectInterface = m_pHeightmapRenderTarget1;
	m_ToolBox->SendMessage(msgHash_RemoveTexture, sizeof(tobj), &tobj );
	m_pHeightmapRenderTarget1 = NULL;

	// remove foam texture
	tobj.TextureObjectInterface = m_pFoamTexture;
	m_ToolBox->SendMessage(msgHash_RemoveTexture, sizeof(tobj), &tobj );
	m_pFoamTexture = NULL;

	// remove env map
	tobj.TextureObjectInterface = m_pEnvMap;
	m_ToolBox->SendMessage(msgHash_RemoveTexture, sizeof(tobj), &tobj );
	m_pEnvMap = NULL;

	// remove fresnel lookup
//	tobj.TextureObjectInterface = m_pFresnelLookup;
//	m_ToolBox->SendMessage(msgHash_RemoveTexture, sizeof(tobj), &tobj );
//	m_pFresnelLookup = NULL;
}

void COceanRenderObject::CreateTextureGenerationGeometry()
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

void COceanRenderObject::DestroyTextureGenerationGeometry()
{
	if (m_TextureGenerationVBI)
	{
		static DWORD msgHash_RemoveVertexAllocation = CHashString(_T("RemoveVertexAllocation")).GetUniqueID();
		m_ToolBox->SendMessage( msgHash_RemoveVertexAllocation, sizeof(m_TextureGenerationVB), &m_TextureGenerationVB );
		m_TextureGenerationVBI = NULL;
	}
}

DWORD COceanRenderObject::OnSetEnvMapTexture( DWORD size, void *params )
{
	VERIFY_MESSAGE_SIZE( size, sizeof( IHashString ) );

	IHashString *hszTextPath = (IHashString*)params;
	m_szEnvMapTexture = hszTextPath->GetString();

	m_pEnvMap = LoadTexture( m_szEnvMapTexture );

	return MSG_HANDLED_STOP;
}