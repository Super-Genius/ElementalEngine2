///============================================================================
/// \file		DetailObject.cpp
/// \brief		Implmentation of CDetailObject Object Class
/// \date		05-27-2007
/// \author		Kenneth Hurley
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
#include "DetailObject.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

REGISTER_COMPONENT(CDetailObject);

REGISTER_MESSAGE_HANDLER(GetGlobalPosition, OnGetGlobalPosition, CDetailObject);
REGISTER_MESSAGE_HANDLER(SetGlobalPosition, OnGetGlobalPosition, CDetailObject);


/// Public Methods

CDetailObject::CDetailObject(IHashString *parentName, IHashString *name, bool bAddToHier) :
	m_hszNameType( _T("CDetailObject") ),
	SCENEOBJECTTEMPLATE(CRenderManager , CDetailObject, IRenderObject, parentName, name)
{
	m_bAddToHierarchy = bAddToHier;
	if (bAddToHier)
	{
		AddToHierarchy();
	}

	// create shader callback
	m_DetailObjectShaderCallback = new CDetailObjectShaderCallback();
	m_ToolBox = EngineGetToolBox();
	m_IRenderer = NULL;
}

CDetailObject::~CDetailObject( void )
{
	if (m_bAddToHierarchy)
	{
		RemoveFromHierarchy();
	}
	if (m_DetailObjectShaderCallback)
	{
		delete m_DetailObjectShaderCallback;
		m_DetailObjectShaderCallback = NULL;
	}
}

IComponent *CDetailObject::Create(int nArgs, va_list argptr)
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
			self = new CDetailObject(parentName, name, true);
		}
		else if (nArgs == 3)
		{
			bAddToHier = va_arg(argptr, bool);
			self = new CDetailObject(parentName, name, bAddToHier);
		}
	}
	catch(...)
	{
		return NULL;
	}

	return self;
}

void CDetailObject::Serialize( IArchive &ar )
{
	StdString temp;
	if(ar.IsReading())
	{
		ar.Read(temp, "LayerLink");
		m_LayerLink = temp.c_str();
		ar.Read(temp, "ModelName");
		m_ModelName = temp.c_str();
		ar.Read(m_XCoverage, "XCoverage");
		ar.Read(m_YCoverage, "YCoverage");
		ar.Read(m_XRandomness, "XRandomness");
		ar.Read(m_YRandomness, "YRandomness");
		ar.Read(m_MinScale, "MinScale");
		ar.Read(m_MinScale, "MaxScale");
		ar.Read(m_MinYaw, "MinYaw");
		ar.Read(m_MaxYaw, "MaxYaw");
	}
	else
	{
		ar.Write(m_LayerLink.GetString(), "LayerLink");
		ar.Write(m_ModelName.GetString(), "ModelName");
		ar.Write(m_XCoverage, "XCoverage");
		ar.Write(m_YCoverage, "YCoverage");
		ar.Write(m_XRandomness, "XRandomness");
		ar.Write(m_YRandomness, "YRandomness");
		ar.Write(m_MinScale, "MinScale");
		ar.Write(m_MinScale, "MaxScale");
		ar.Write(m_MinYaw, "MinYaw");
		ar.Write(m_MaxYaw, "MaxYaw");
	}
}

void CDetailObject::Init( void )
{
#if 0
	GETRENDERERINTERFACEPARAMS renderInterfaceParam;
	static DWORD msgHash_GetRendererInterface = CHashString(_T("GetRendererInterface")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_GetRendererInterface, sizeof(GETRENDERERINTERFACEPARAMS), &renderInterfaceParam, NULL, NULL );
	static DWORD msgHash_CreateController = CHashString(_T("CreateController")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_CreateController, sizeof( IController** ), &m_GeometryController );
	static DWORD msgHash_CreateGeometry = CHashString(_T("CreateGeometry")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_CreateGeometry, sizeof( IGeometry** ), &m_Geometry );

	m_IRenderer = renderInterfaceParam.m_RendererInterface;

	//load the shader
	IEffect * MyEffect = NULL;
	//load the shader
	LOADFILEEXTPARAMS lfep;
	if( m_ShaderFile == "" )
	{ 
		m_ShaderFile = "SHADERS/DEFAULTSHADER.efx"; 
	}
	m_ShaderFile.MakeSafeFileName();
	CHashString filename( m_ShaderFile );
	lfep.fileName = (TCHAR*)m_ShaderFile;
	lfep.bInternalLoad = true;
	static DWORD msgHash_LoadFileByExtension = CHashString(_T("LoadFileByExtension")).GetUniqueID();
	DWORD retval = EngineGetToolBox()->SendMessage(msgHash_LoadFileByExtension, sizeof(LOADFILEEXTPARAMS), &lfep);
	if( retval == MSG_HANDLED )
	{
		CREATEEFFECTINTERFACE cef;
		cef.m_Name = &filename;
		static DWORD msgHash_GetEffectInterface = CHashString(_T("GetEffectInterface")).GetUniqueID();
		EngineGetToolBox()->SendMessage(msgHash_GetEffectInterface, sizeof(CREATEEFFECTINTERFACE ), &cef );
		MyEffect = cef.m_EffectInterface;
		if( !MyEffect )
		{
			EngineGetToolBox()->Log( LOGERROR, _T("Could not load DetailObject's shader" ) );
		}
	}	

	GenerateVertices();

	CREATEMATERIALINTERFACE cmat;
	CREATEOBJECTPARAMS cop;
	TCHAR buf[ 512 ];
	CHashString hszType( _T("CMaterial" ) );
	IHashString * hname = GetName();
	if( m_GeometryController && m_Geometry )
	{
		m_Geometry->SetController( m_GeometryController );
		
		//set shader
		IMaterial * material = NULL;
		_stprintf( buf, "%s_mat", hname->GetString());
		CHashString hszName( buf );

		//Create the material
		cop.parentName = NULL;
		cop.typeName = &hszType;
		cop.name = &hszName;
		static DWORD msgHash_CreateObject = CHashString(_T("CreateObject")).GetUniqueID();
		m_ToolBox->SendMessage(msgHash_CreateObject, sizeof(CREATEOBJECTPARAMS), &cop, NULL, NULL);
		if( material )
		{
			cmat.m_Name = &hszName;
			static DWORD msgHash_GetMaterialInterface = CHashString(_T("GetMaterialInterface")).GetUniqueID();
			m_ToolBox->SendMessage(msgHash_GetMaterialInterface, sizeof(cmat), &cmat, NULL, NULL);
			if( cmat.m_MaterialInterface )
			{
				material = cmat.m_MaterialInterface;
				material->SetEffect( MyEffect );
			}
		}
		m_Geometry->SetMaterial( material );
		
		//do the proper allocation for the geometry
		BUFFERALLOCATIONSTRUCT tempalloc;
		tempalloc.m_Offset = m_VertexBufferAllocation.m_Offset;
		tempalloc.m_Size = m_VertexBufferAllocation.m_Size;
		tempalloc.m_Stride = m_VertexBufferAllocation.m_Stride;
		tempalloc.m_InterfaceHandle = m_VertexBufferAllocation.m_InterfaceHandle;
		tempalloc.m_AllocationHandle = m_VertexBufferAllocation.m_AllocationHandle;
		////vertex size and offset should be current

		m_Geometry->SetVertexBufferAllocation( tempalloc );
		tempalloc.m_Offset = m_IndexBufferAllocation.m_Offset;
		tempalloc.m_Size = m_IndexBufferAllocation.m_Size;
		tempalloc.m_Stride = m_IndexBufferAllocation.m_Stride;
		tempalloc.m_InterfaceHandle = m_IndexBufferAllocation.m_InterfaceHandle;
		tempalloc.m_AllocationHandle = m_IndexBufferAllocation.m_AllocationHandle;
		////vertex size and offset should be current
		m_Geometry->SetIndexBufferAllocation( tempalloc );
	}
	else
	{
	}

	InitializeTextures();

	UpdateBoundingObject();
#endif
}

bool CDetailObject::Update( void )
{

	return true;
}

void CDetailObject::DeInit( void )
{

}

IHashString* CDetailObject::GetComponentType( void )
{
	return &m_hszNameType;
}

bool CDetailObject::IsKindOf( IHashString* compType )
{
	return (compType->GetUniqueID() == m_hszNameType.GetUniqueID() );
}

bool CDetailObject::Render( UINT pass, IEffect * override )
{
#if 0
	m_IRenderer->SetDepthWrite( true );

	m_GeometryController->BuildRotation();

	Matrix4x4 * matworld =  m_GeometryController->GetMatrix();
	m_IRenderer->SetMatrix( WORLD_MATRIX, (CONST FLOAT*)matworld->GetMatrix() );

	m_Geometry->Render();

	// TODO:	We'll probably want to just single pixel width lines once the 
	//			functions to draw lines are working correctly. The following code is
	//			just a reference sample of how to call the function.
	//m_IRenderer->Draw3DLine(0, 0, 0,	m_AxisLength, 0, 0,		255, 0, 0, m_AxisWidthInPixels);
	//m_IRenderer->Draw3DLine(0, 0, 0,	0, m_AxisLength, 0,		0, 255, 0, m_AxisWidthInPixels);
	//m_IRenderer->Draw3DLine(0, 0, 0,	0, 0, m_AxisLength,		0, 0, 255, m_AxisWidthInPixels);
#endif
	return true;
}

bool CDetailObject::IsLessThan( const IRenderObject * obj, DWORD sortMethod, int curRenderPass )
{
	return true;
}

DWORD CDetailObject::GetRenderPriority( void )
{
    return 0;
}

UINT CDetailObject::GetNumPass( void )
{
	return 1;
}

void CDetailObject::SetPosition( const Vec3 &vPosition )
{
	m_vPosition.Set( vPosition );
}

Vec3 &CDetailObject::GetPosition( void )
{
	return m_vPosition;
}

void CDetailObject::GetBoundingSphere( Vec3 &position, float &radius )
{
	position.Set( 0,0,0 );
	radius = 5000;
}

bool CDetailObject::GetBoundingBox( Vec3 &pos, Vec3 &dim, EulerAngle &rotation )
{
	return false;
}

IHashString * CDetailObject::GetBaseSortClass( void )
{
	return GetComponentType();
}

bool CDetailObject::IsAlphaObject( void )
{
	return false;
}

IMeshObject* CDetailObject::GetMeshObject( void )
{
	return NULL;
}

void CDetailObject::GetMinMax( Vec3& vMin, Vec3& vMax )
{
	vMin.x = m_vPosition.x - 5;
	vMin.y = m_vPosition.y - 5;
	vMin.z = m_vPosition.z - 5;

	vMin.x = m_vPosition.x + 5;
	vMin.y = m_vPosition.y + 5;
	vMin.z = m_vPosition.z + 5;
}

DWORD CDetailObject::OnGetGlobalPosition(DWORD size, void *params)
{
	VERIFY_MESSAGE_SIZE( size, sizeof(Vec3) );

	Vec3 *pos = (Vec3*)params;
	pos->Set( GetPosition() );
	return MSG_HANDLED_PROCEED;
}

DWORD CDetailObject::OnSetGlobalPosition(DWORD size, void *params)
{
	VERIFY_MESSAGE_SIZE( size, sizeof(Vec3) );

	Vec3 *pos = (Vec3*)params;
	SetPosition( *pos );
	return MSG_HANDLED_PROCEED;
}



/// Private Methods

void CDetailObject::GenerateVertices( void )
{
#if 0
	const int atotalVertices = 84;
	const int atotalIndices = 126;

	static short avertexIndices[atotalIndices];

	int aindexOffset = 0;
	int aindexArrayOffset = 0;
	int aindexIterations = atotalIndices / 6;
	for( int i = 0; i < aindexIterations; i++)
	{
		/*0, 1, 2, 1, 3, 2,
		4, 5, 6, 5, 7, 6,
		8, 9, 10, 9, 11, 10,
		12, 13, 14, 13, 15, 14,

		16, 17, 18, 17, 19, 18,
		20, 21, 22, 21, 23, 22,
		24, 25, 26, 25, 27, 26,
		28, 29, 30, 29, 31, 30,

		32, 33, 34, 33, 35, 34,
		36, 37, 38, 37, 39, 38,
		40, 41, 42, 41, 43, 42,
		44, 45, 46, 45, 47, 46,

		48, 49, 50, 49, 51, 50*/

		aindexOffset = i * 4;
		aindexArrayOffset = i * 6;

		avertexIndices[aindexArrayOffset + 0] = aindexOffset + 0;
		avertexIndices[aindexArrayOffset + 1] = aindexOffset + 1;
		avertexIndices[aindexArrayOffset + 2] = aindexOffset + 2;
		avertexIndices[aindexArrayOffset + 3] = aindexOffset + 1;
		avertexIndices[aindexArrayOffset + 4] = aindexOffset + 3;
		avertexIndices[aindexArrayOffset + 5] = aindexOffset + 2;
	};

	//Allocate new vertex buffer space, remove any old ones				
	m_IndexBufferInterface = NULL;
	m_VertexBufferInterface = NULL;

	//resize the vertex allocation
	if( m_VertexBufferAllocation.m_Size != 0 )
	{
		//previously allocated, remove
		static DWORD msgHash_RemoveVertexAllocation = CHashString(_T("RemoveVertexAllocation")).GetUniqueID();
		m_ToolBox->SendMessage(msgHash_RemoveVertexAllocation, sizeof( m_VertexBufferAllocation ), &m_VertexBufferAllocation, NULL, NULL );
	}

	//resize index allocation
	if( m_IndexBufferAllocation.m_Size != 0 )
	{
		//previously allocated, remove
		static DWORD msgHash_RemoveIndexAllocation = CHashString(_T("RemoveIndexAllocation")).GetUniqueID();
		m_ToolBox->SendMessage(msgHash_RemoveIndexAllocation, sizeof( m_IndexBufferAllocation ), &m_IndexBufferAllocation, NULL, NULL );
	}

	CHANNELDESCRIPTORLIST avertexFormat;
	avertexFormat.push_back( MESH_STREAM_VERTEX_POSITION3 );
	avertexFormat.push_back( MESH_STREAM_VERTEX_TEXTURECOORD1 );

	//create new allocation
	ALLOCATERENDERBUFFERMSG arenderBufferMessage;

	UINT asizeVertex = sizeof( COORDINATETOOLVERTEX );

	arenderBufferMessage.ElementsToAllocate		= atotalVertices;
	arenderBufferMessage.SizeVertexFormat		= sizeof( COORDINATETOOLVERTEX );
	arenderBufferMessage.VertexFormat			= avertexFormat;

	static DWORD msgHash_AllocateVertices = CHashString(_T("AllocateVertices")).GetUniqueID();
	DWORD amessageResult = m_ToolBox->SendMessage(msgHash_AllocateVertices, sizeof( arenderBufferMessage ), &arenderBufferMessage, NULL, NULL );

	if( amessageResult == MSG_HANDLED )
	{
		m_VertexBufferAllocation.m_Offset				= arenderBufferMessage.m_ReturnOffset;
		m_VertexBufferAllocation.m_AllocationHandle		= arenderBufferMessage.m_ReturnHandle;
		m_VertexBufferAllocation.m_InterfaceHandle		= arenderBufferMessage.m_ReturnInterfaceHandle;
		m_VertexBufferAllocation.m_Size					= arenderBufferMessage.ElementsToAllocate;
		m_VertexBufferAllocation.m_Stride				= arenderBufferMessage.SizeVertexFormat;
		m_VertexBufferInterface							= (IVertexBufferObject*)arenderBufferMessage.m_InterfacePointer;
	}

	//create new allocation
	arenderBufferMessage.ElementsToAllocate = atotalIndices;
	arenderBufferMessage.SizeVertexFormat = sizeof( short );

	static DWORD msgHash_AllocateIndices = CHashString(_T("AllocateIndices")).GetUniqueID();
	amessageResult = m_ToolBox->SendMessage(msgHash_AllocateIndices, sizeof( arenderBufferMessage ), &arenderBufferMessage, NULL, NULL );
	if( amessageResult == MSG_HANDLED )
	{
		m_IndexBufferAllocation.m_Offset			= arenderBufferMessage.m_ReturnOffset;
		m_IndexBufferAllocation.m_AllocationHandle	= arenderBufferMessage.m_ReturnHandle;
		m_IndexBufferAllocation.m_InterfaceHandle	= arenderBufferMessage.m_ReturnInterfaceHandle;
		m_IndexBufferAllocation.m_Size				= arenderBufferMessage.ElementsToAllocate;
		m_IndexBufferAllocation.m_Stride			= sizeof( short );
		m_IndexBufferInterface						= (IIndexBuffer*)arenderBufferMessage.m_InterfacePointer;
	}

	//generate the vertices
	void	*pVertex;
	short	*pFace;

	//Lock the vertex buffer and index buffer
	if( m_VertexBufferInterface && 
		!m_VertexBufferInterface->Lock( m_VertexBufferAllocation.m_Offset,
		atotalVertices, (void**)&pVertex ) )
	{
		return;
	}
	if( m_VertexBufferInterface && 
		!m_IndexBufferInterface->Lock( m_IndexBufferAllocation.m_Offset,
		atotalIndices, (void**)&pFace ) )
	{
		m_VertexBufferInterface->Unlock();
		return;
	}

	memcpy( pVertex,	avertices,		sizeof( COORDINATETOOLVERTEX ) * atotalVertices );
	memcpy( pFace,		avertexIndices,	sizeof( short ) * atotalIndices );

	m_VertexBufferInterface->Unlock();
	m_IndexBufferInterface->Unlock();
#endif

}

void CDetailObject::InitializeTextures()
{
#if 0
	// get the filename of the texture
	StdString strFilename;
	strFilename = m_TexturePath;
	strFilename += "/sky_0";
	strFilename += ".tga";

	// load the texture from the file
	CHashString fullname( strFilename );
	
	TEXTUREOBJECTPARAMS tobj;
	tobj.bLoad = true;
	tobj.Name = &fullname;
	tobj.TextureObjectInterface = NULL;
	static DWORD msgHash_AddTexture = CHashString(_T("AddTexture")).GetUniqueID();
	DWORD res = m_ToolBox->SendMessage(msgHash_AddTexture, sizeof(tobj), &tobj );

	if( res == MSG_HANDLED )
	{
		// store the texture id in the user data of the map
		m_Texture = (IBaseTextureObject *)tobj.TextureObjectInterface;
	}
	else
	{
		m_Texture = NULL;
	}

	//Set the texture for geometries
	if( m_Geometry && 
		m_Geometry->GetMaterial())
	{
		IMaterial * mat = m_Geometry->GetMaterial();
		mat->SetTexture(0,0,m_Texture);
	}
#endif
}

