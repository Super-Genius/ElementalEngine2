///============================================================================
/// \file		CoordinateRenderObject.cpp
/// \brief		Implmentation of CCoordinateRenderObject Object Class
/// \date		09-18-2006
/// \author		Richard Nguyen
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
#include "CoordinateRenderObject.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

REGISTER_COMPONENT(CCoordinateRenderObject);

/// Public Methods

CCoordinateRenderObject::CCoordinateRenderObject( IHashString *parentName, IHashString *name ) :
	m_hszNameType( _T("CCoordinateRenderObject") ),
	SCENEOBJECTTEMPLATE( CCoordinateToolManager, CCoordinateRenderObject, IRenderObject, parentName, name )
{
	// create shader callback
	m_CoordinateToolShaderCallback = new CCoordinateToolShaderCallback();
	m_ToolBox = EngineGetToolBox();
	m_IRenderer = NULL;
}

CCoordinateRenderObject::~CCoordinateRenderObject( void )
{
	if (m_CoordinateToolShaderCallback)
	{
		delete m_CoordinateToolShaderCallback;
		m_CoordinateToolShaderCallback = NULL;
	}
}

IComponent *CCoordinateRenderObject::Create(int nArgs, va_list argptr)
{
	IHashString *name;
	IHashString *parentName;
	IObject *self;

	name = va_arg(argptr, IHashString *);
	parentName = va_arg(argptr, IHashString *);

	// create the object however you must create it
	try
	{
		self = new CCoordinateRenderObject( parentName, name );
	}
	catch(...)
	{
		return NULL;
	}

	return self;
}

void CCoordinateRenderObject::Serialize( IArchive &ar )
{
	// TODO:	This serialization reads in the texture path for the sky textures.
	//			Not going to need it in the future.
	if(ar.IsReading())
	{
		ar.Read( m_TexturePath, "TexturePath" );
	}
	else
	{
		ar.Write( m_TexturePath, "TexturePath" );
	}
}

void CCoordinateRenderObject::Init( void )
{
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
	CHashString filename( m_ShaderFile  );
	lfep.fileName = (TCHAR*)m_ShaderFile.c_str();
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
			EngineGetToolBox()->Log( LOGERROR, _T("Could not load CoordinateRenderObject's shader" ) );
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
}

bool CCoordinateRenderObject::Update( void )
{

	return true;
}

void CCoordinateRenderObject::DeInit( void )
{

}

IHashString* CCoordinateRenderObject::GetComponentType( void )
{
	return &m_hszNameType;
}

bool CCoordinateRenderObject::IsKindOf( IHashString* compType )
{
	return (compType->GetUniqueID() == m_hszNameType.GetUniqueID() );
}

bool CCoordinateRenderObject::Render( UINT pass, IEffect * override )
{
	m_IRenderer->SetDepthWrite( true );

	m_GeometryController->BuildRotation();

	Matrix4x4 * matworld =  m_GeometryController->GetMatrix();
	m_IRenderer->SetMatrix( WORLD_MATRIX, (const float*)matworld->GetMatrix() );

	m_Geometry->Render();

	// TODO:	We'll probably want to just single pixel width lines once the 
	//			functions to draw lines are working correctly. The following code is
	//			just a reference sample of how to call the function.
	//m_IRenderer->Draw3DLine(0, 0, 0,	m_AxisLength, 0, 0,		255, 0, 0, m_AxisWidthInPixels);
	//m_IRenderer->Draw3DLine(0, 0, 0,	0, m_AxisLength, 0,		0, 255, 0, m_AxisWidthInPixels);
	//m_IRenderer->Draw3DLine(0, 0, 0,	0, 0, m_AxisLength,		0, 0, 255, m_AxisWidthInPixels);

	return true;
}

bool CCoordinateRenderObject::IsLessThan( const IRenderObject * obj, DWORD sortMethod, int curRenderPass )
{
	return true;
}

DWORD CCoordinateRenderObject::GetRenderPriority( void )
{
    return 0;
}

UINT CCoordinateRenderObject::GetNumPass( void )
{
	return 1;
}

void CCoordinateRenderObject::SetPosition( const Vec3 &vPosition )
{
	m_vPosition.Set( vPosition );
}

Vec3 &CCoordinateRenderObject::GetPosition( void )
{
	return m_vPosition;
}

void CCoordinateRenderObject::GetBoundingSphere( Vec3 &position, float &radius )
{
	position.Set( 0,0,0 );
	radius = 5000;
}

bool CCoordinateRenderObject::GetBoundingBox( Vec3 &pos, Vec3 &dim, EulerAngle &rotation )
{
	return false;
}

IHashString * CCoordinateRenderObject::GetBaseSortClass( void )
{
	return GetComponentType();
}

bool CCoordinateRenderObject::IsAlphaObject( void )
{
	return false;
}

IMeshObject* CCoordinateRenderObject::GetMeshObject( void )
{
	return NULL;
}

void CCoordinateRenderObject::GetMinMax( Vec3& vMin, Vec3& vMax )
{
	vMin.x = m_vPosition.x - 5;
	vMin.y = m_vPosition.y - 5;
	vMin.z = m_vPosition.z - 5;

	vMin.x = m_vPosition.x + 5;
	vMin.y = m_vPosition.y + 5;
	vMin.z = m_vPosition.z + 5;
}

Vec3& CCoordinateRenderObject::GetEnds( void )
{
	return m_vEnds;
}

void CCoordinateRenderObject::SetEnds( Vec3& vEnds )
{
	
}

EulerAngle& CCoordinateRenderObject::GetRotation( void )
{
	return m_vRotation;
}
void CCoordinateRenderObject::SetRotation( EulerAngle& vRotation )
{
	m_vRotation.Set( vRotation );
}

/// Private Methods

void CCoordinateRenderObject::GenerateVertices( void )
{
	const float aarrowWidth = m_AxisWidthInSpace * 3.0f;
	const float aarrowLength = m_AxisWidthInSpace * 3.0f;

	static COORDINATETOOLVERTEX avertices[] =
	{
		{ 0,					m_AxisLength,		m_AxisWidthInSpace,	1, 0 },
		{ m_AxisWidthInSpace,	m_AxisLength,		0,					1, 1 },
		{ 0,					0,					m_AxisWidthInSpace,	0, 0 },
		{ m_AxisWidthInSpace,	0,					0,					0, 1 },

		{ m_AxisWidthInSpace,	m_AxisLength,		0,					1,	0 },
		{ 0,					m_AxisLength,		-m_AxisWidthInSpace,1,	1 },
		{ m_AxisWidthInSpace,	0,					0,					0,	0 },
		{ 0,				0	,					-m_AxisWidthInSpace,0,	1 },

		{ -m_AxisWidthInSpace,	m_AxisLength,		0,					1,	0 },
		{ 0,					m_AxisLength,		m_AxisWidthInSpace,	1,	1 },
		{ -m_AxisWidthInSpace,	0,					0,					0,	0 },
		{ 0,					0,					m_AxisWidthInSpace,	0,	1 },

		{ 0,					m_AxisLength,		-m_AxisWidthInSpace,1,	0 },
		{ -m_AxisWidthInSpace,	m_AxisLength,		0,					1,	1 },
		{ 0,					0,					-m_AxisWidthInSpace,0,	0 },
		{ -m_AxisWidthInSpace,	0,					0,					0,	1 },


		{ 0,					m_AxisWidthInSpace,	m_AxisLength,		1, 0 },
		{ -m_AxisWidthInSpace,	0,					m_AxisLength,		1, 1 },
		{ 0,					m_AxisWidthInSpace,	0,					0, 0 },
		{ -m_AxisWidthInSpace,	0,					0,					0, 1 },

		{ -m_AxisWidthInSpace,	0,					m_AxisLength,		1,	0 },
		{ 0,					-m_AxisWidthInSpace,m_AxisLength,		1,	1 },
		{ -m_AxisWidthInSpace,	0,					0,					0,	0 },
		{ 0,					-m_AxisWidthInSpace,0,					0,	1 },

		{ m_AxisWidthInSpace,	0,					m_AxisLength,		1,	0 },
		{ 0,					m_AxisWidthInSpace,	m_AxisLength,		1,	1 },
		{ m_AxisWidthInSpace,	0,					0,					0,	0 },
		{ 0,					m_AxisWidthInSpace,	0,					0,	1 },

		{ 0,					-m_AxisWidthInSpace,m_AxisLength,		1,	0 },
		{ m_AxisWidthInSpace,	0,					m_AxisLength,		1,	1 },
		{ 0,					-m_AxisWidthInSpace,0,					0,	0 },
		{ m_AxisWidthInSpace,	0,					0,					0,	1 },


		{ m_AxisLength,			0,					m_AxisWidthInSpace,	1, 0 },
		{ m_AxisLength,			-m_AxisWidthInSpace,0,					1, 1 },
		{ 0,					0,					m_AxisWidthInSpace,	0, 0 },
		{ 0,					-m_AxisWidthInSpace,0,					0, 1 },

		{ m_AxisLength,			-m_AxisWidthInSpace,0,					1,	0 },
		{ m_AxisLength,			0,					-m_AxisWidthInSpace,1,	1 },
		{ 0,					-m_AxisWidthInSpace,0,					0,	0 },
		{ 0,					0,					-m_AxisWidthInSpace,0,	1 },

		{ m_AxisLength,			m_AxisWidthInSpace,	0,					1,	0 },
		{ m_AxisLength,			0,					m_AxisWidthInSpace,	1,	1 },
		{ 0,					m_AxisWidthInSpace,	0,					0,	0 },
		{ 0,					0,					m_AxisWidthInSpace,	0,	1 },

		{ m_AxisLength,			0,					-m_AxisWidthInSpace,1,	0 },
		{ m_AxisLength,			m_AxisWidthInSpace,	0,					1,	1 },
		{ 0,					0,					-m_AxisWidthInSpace,0,	0 },
		{ 0,					m_AxisWidthInSpace,	0,					0,	1 },


		{ 0,					m_AxisLength,		aarrowWidth,	1, 0 },
		{ aarrowWidth,			m_AxisLength,		0,				1, 1 },
		{ -aarrowWidth,			m_AxisLength,		0,				0, 0 },
		{ 0,					m_AxisLength,		-aarrowWidth,	0, 1 },

		{ aarrowWidth,			0,					m_AxisLength,	1, 0 },
		{ 0,					aarrowWidth,		m_AxisLength,	1, 1 },
		{ 0,					-aarrowWidth,		m_AxisLength,	0, 0 },
		{ -aarrowWidth,			0,					m_AxisLength,	0, 1 },

		{ m_AxisLength,			0,					aarrowWidth,	1, 0 },
		{ m_AxisLength,			-aarrowWidth,		0,				1, 1 },
		{ m_AxisLength,			aarrowWidth,		0,				0, 0 },
		{ m_AxisLength,			0,					-aarrowWidth,	0, 1 },

		{ 0,			m_AxisLength,				aarrowWidth,	1, 0 },
		{ 0,			m_AxisLength+aarrowWidth*2,	0,				1, 1 },
		{ aarrowWidth,	m_AxisLength,				0,				0, 0 },
		{ 0,			m_AxisLength,				-aarrowWidth,	0, 1 },

		{ 0,			m_AxisLength,				aarrowWidth,	1, 0 },
		{ -aarrowWidth,	m_AxisLength,				0,				1, 1 },
		{ 0,			m_AxisLength+aarrowWidth*2,	0,				0, 0 },
		{ 0,			m_AxisLength,				-aarrowWidth,	0, 1 },

		{ 0,			-aarrowWidth,	m_AxisLength,				1, 0 },
		{ 0,			0,				m_AxisLength+aarrowWidth*2,	1, 1 },
		{ aarrowWidth,	0,				m_AxisLength,				0, 0 },
		{ 0,			aarrowWidth,	m_AxisLength,				0, 1 },

		{ 0,			aarrowWidth,	m_AxisLength,				1, 0 },
		{ 0,			0,				m_AxisLength+aarrowWidth*2,	1, 1 },
		{ -aarrowWidth,	0,				m_AxisLength,				0, 0 },
		{ 0,			-aarrowWidth,	m_AxisLength,				0, 1 },

		{ m_AxisLength,					0,				aarrowWidth,	1, 0 },
		{ m_AxisLength+aarrowWidth*2,	0,				0,				1, 1 },
		{ m_AxisLength,					-aarrowWidth,	0,				0, 0 },
		{ m_AxisLength,					0,				-aarrowWidth,	0, 1 },

		{ m_AxisLength,					0,				-aarrowWidth,	1, 0 },
		{ m_AxisLength+aarrowWidth*2,	0,				0,				1, 1 },
		{ m_AxisLength,					aarrowWidth,	0,				0, 0 },
		{ m_AxisLength,					0,				aarrowWidth,	0, 1 },

	};

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
}

void CCoordinateRenderObject::InitializeTextures()
{
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
}

const float	CCoordinateRenderObject::m_AxisLength = 50.f;
const float	CCoordinateRenderObject::m_AxisWidthInSpace = 2.f;
const UINT CCoordinateRenderObject::m_AxisWidthInPixels = 1;