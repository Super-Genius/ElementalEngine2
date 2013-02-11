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
#include <d3dx9mesh.h>

#define EPSILON_ZERO .0001f
float MeshParameterization::TEXTURE_TO_WORLD_SPACE_RATIO=	7.f; //approx 10 worldspace units boils to 1 texture unit
int MeshParameterization::MIN_TEXTURE_SIZE	=256;
int MeshParameterization::MAX_TEXTURE_SIZE	=256;
#define ENABLE_DEBUG_DEGENERATES	0

REGISTER_COMPONENT(MeshParameterization);

REGISTER_MESSAGE_HANDLER(GenerateAtlas, OnGenerateAtlas, MeshParameterization)
REGISTER_MESSAGE_HANDLER(AddIndexSoup, OnAddIndexSoup, MeshParameterization);
REGISTER_MESSAGE_HANDLER(AddVertexIndexSoup, OnAddVertexIndexSoup, MeshParameterization);
REGISTER_MESSAGE_HANDLER(SetMeshTransform, SetMeshTransform, MeshParameterization);
REGISTER_MESSAGE_HANDLER(SetTextureSize, OnSetTextureSize, MeshParameterization)
REGISTER_MESSAGE_HANDLER(GetTextureSize, OnGetTextureSize, MeshParameterization)
REGISTER_MESSAGE_HANDLER(GetCache, GetCache, MeshParameterization);
REGISTER_MESSAGE_HANDLER(SetCache, SetCache, MeshParameterization);
REGISTER_MESSAGE_HANDLER(GetCollapsedMesh, OnGetCollapsedMesh, MeshParameterization);
REGISTER_MESSAGE_HANDLER(GetTriangleFaces, OnGetTriangleFaces, MeshParameterization);
REGISTER_MESSAGE_HANDLER(GetMeshTransform, OnGetMeshTransform, MeshParameterization);
REGISTER_MESSAGE_HANDLER(GetMeshInverseTransform, OnGetMeshInverseTransform, MeshParameterization);
REGISTER_MESSAGE_HANDLER(GetAABB, OnGetAABB, MeshParameterization);
REGISTER_MESSAGE_HANDLER(GenerateBounds, OnGenerateBounds, MeshParameterization);
REGISTER_MESSAGE_HANDLER(IntersectRayTriangle, OnIntersectRayTriangle, MeshParameterization);
REGISTER_MESSAGE_HANDLER(GenerateTriangleTexelData, OnGenerateTriangleTexelData, MeshParameterization);
REGISTER_MESSAGE_HANDLER(GetBaseColorAtTriangleIntersection, OnGetBaseColorAtTriangleIntersection, MeshParameterization);
REGISTER_MESSAGE_HANDLER(SetNoClone, OnSetNoClone, MeshParameterization);


//values for non-class specific callback
static Ray mp_Ray(Vec3(0,0,0), Vec3(0,0,0));
static double mp_T;
static double mp_U;
static double mp_V;
static bool mp_Intersected = false;
static MeshParameterization * mp_MeshP = NULL;
static int mp_TriIndex;

struct paramVertex
{
	float x;
	float y;
	float z;
	float nx;
	float ny;
	float nz;
	float u;
	float v;
};

MeshParameterization::MeshParameterization(IHashString *parentName, IHashString *name) : 
	OBJECTTEMPLATE(LightMapManager, MeshParameterization, IObject, parentName, name)
{
	m_Transform = NULL;
	m_InverseTransform = NULL;
	m_TextureBuffer = NULL;
	m_IndirectLightBuffer = NULL;
	tmax = Vec3( -99999, -99999, -99999 );
	tmin = Vec3( 99999, 99999, 99999 );
	m_TexSize = 256;
	m_TexSize = 256;
	m_KDTree = NULL;
	m_KDvertexData = NULL;
	m_CollapsedMesh = NULL;
	m_Faces = NULL;
	GETRENDERERINTERFACEPARAMS renderInterface;
	DWORD msgHash_GetRendererInterface = CHashString(_T("GetRendererInterface")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_GetRendererInterface, 
						sizeof(GETRENDERERINTERFACEPARAMS), 
						&renderInterface);
	IRenderer * Renderer = renderInterface.m_RendererInterface;
	SetDevice( (void*)Renderer->GetAPIDevice() );
}

MeshParameterization::~MeshParameterization()
{
	if( m_TextureBuffer ){
		delete [] m_TextureBuffer;
	}
	if( m_Transform )
	{
		delete m_Transform;
	}
	if( m_InverseTransform )
	{
		delete m_InverseTransform;
	}
	if( m_KDvertexData )
	{
		delete m_KDvertexData;
	}
	if( m_KDTree )
	{
		delete m_KDTree;
	}
}

/// \brief The Initialization for the Object
/// \return pointer to an IComponent
IComponent *MeshParameterization::Create(int nArgs, va_list argptr)
{
	IHashString *name;
	IHashString *parentName;
	IObject *self;

	name = va_arg(argptr, IHashString *);
	parentName = va_arg(argptr, IHashString *);
	// create the object however you must create it
	try
	{
		// try and create an instance
		if (nArgs == 2)
		{
			self = new MeshParameterization(parentName, name);
		}
	}
	catch(...)
	{
		return NULL;
	}

	return self;
}

void MeshParameterization::Serialize(IArchive &ar)
{
	
}

void MeshParameterization::Init()
{
}

/// \brief Update the object
/// \return true or false
bool MeshParameterization::Update()
{
	
	return true;
}

void MeshParameterization::DeInit()
{
}

bool MeshParameterization::IsKindOf( IHashString * compType )
{	
	static CHashString  CCal3dROCOMP(_T("MeshParameterization") );
	
	return (compType->GetUniqueID() == CCal3dROCOMP.GetUniqueID());
}

void MeshParameterization::SetDevice( void * unknownDevice )
{
	m_pDevice = static_cast < LPDIRECT3DDEVICE9 >( unknownDevice );
}

DWORD MeshParameterization::OnAddIndexSoup( DWORD size, void * params )
{
	VERIFY_MESSAGE_SIZE( size, sizeof( RENDERBUFFERALLOCATION ) );
	RENDERBUFFERALLOCATION * allocation = (RENDERBUFFERALLOCATION*)params;
	if( allocation )
	{
		AddVertexSoup( allocation, true );
		return MSG_HANDLED_STOP;
	}
	return MSG_ERROR;
}

DWORD MeshParameterization::OnAddVertexIndexSoup( DWORD size, void * params )
{
	VERIFY_MESSAGE_SIZE( size, sizeof( RENDERBUFFERALLOCATION ) );
	RENDERBUFFERALLOCATION * allocation = (RENDERBUFFERALLOCATION*)params;
	if( allocation )
	{
		AddVertexSoup( allocation, false );
		return MSG_HANDLED_STOP;
	}
	return MSG_ERROR;
}

void MeshParameterization::AddVertexSoup( RENDERBUFFERALLOCATION * renderbuffer, bool onlyIndices )
{
	if( renderbuffer)
	{
		// calculate the bbox
		HRESULT hr;
		Vec3 current;
		int numVerts = renderbuffer->m_VertexAllocation.m_Size;
		int stride = renderbuffer->m_VertexAllocation.m_Stride;
		int numIndices = renderbuffer->m_IndexAllocation.m_Size;
		m_CollapsedMesh->reserve( numVerts );
		m_Faces->reserve( numIndices/3 );

		BYTE* pVertex = NULL;
		static CHashString vp3(_T("MESH_STREAM_VERTEX_POSITION3"));
		int positionoffset = 0;
		int uvoffset = 0;
		int normalOffset = 0;
		int vertexFormatSize = renderbuffer->m_VertexAllocation.VertexFormat.size();
		for( int i = 0; i < vertexFormatSize; i++ )
		{
			ChannelDesc desc = renderbuffer->m_VertexAllocation.VertexFormat[i];
			if( desc.ChannelHash == vp3.GetUniqueID() )
			{
				break;
			}
			positionoffset += renderbuffer->m_VertexAllocation.VertexFormat[ i ].Stride;
		}
		static CHashString tc1(_T("MESH_STREAM_VERTEX_TEXTURECOORD1"));
		for( int i = 0; i < vertexFormatSize; i++ )
		{
			ChannelDesc desc = renderbuffer->m_VertexAllocation.VertexFormat[i];
			if( desc.ChannelHash == tc1.GetUniqueID() )
			{
				break;
			}
			uvoffset += renderbuffer->m_VertexAllocation.VertexFormat[ i ].Stride;
		}
		static CHashString nrm(_T("MESH_STREAM_VERTEX_NORMAL"));
		for( int i = 0; i < vertexFormatSize; i++ )
		{
			ChannelDesc desc = renderbuffer->m_VertexAllocation.VertexFormat[i];
			if( desc.ChannelHash == nrm.GetUniqueID() )
			{
				break;
			}
			normalOffset += renderbuffer->m_VertexAllocation.VertexFormat[ i ].Stride;
		}

		if( !onlyIndices )
		{
			IVertexBufferObject * vb = renderbuffer->m_VertexBufferInterface;
			if( !vb )return;
			ParameterizationVertex curVert;
			hr = vb->Lock( renderbuffer->m_VertexAllocation.m_Offset, renderbuffer->m_VertexAllocation.m_Size, (void**)&pVertex );
			if( pVertex != NULL )
			{
				for( int i=0; i < numVerts; i++ )
				{
					BYTE* posbyte = pVertex;
					BYTE* uvByte = pVertex;
					BYTE* normalByte = pVertex;
					normalByte += normalOffset;
					posbyte += positionoffset;
					uvByte += uvoffset;
					float* curpos = (float *)posbyte;
					float * curUv = (float*)uvByte;
					float * normal = (float*)normalByte;
					curVert.originalPosition.x = curpos[0];
					curVert.originalPosition.y = curpos[1];
					curVert.originalPosition.z = curpos[2];
					tmin.x = min( curVert.originalPosition.x, tmin.x );
					tmin.y = min( curVert.originalPosition.y, tmin.y );
					tmin.z = min( curVert.originalPosition.z, tmin.z );
					tmax.x = max( curVert.originalPosition.x, tmax.x );
					tmax.y = max( curVert.originalPosition.y, tmax.y );
					tmax.z = max( curVert.originalPosition.z, tmax.z );
					curVert.generatedU = curUv[ 0 ];
					curVert.generatedV = curUv[ 1 ];
					curVert.originalU = curUv[ 0 ];
					curVert.originalV = curUv[ 1 ];
					curVert.normal.x = normal[0];
					curVert.normal.y = normal[1];
					curVert.normal.z = normal[2];

					m_CollapsedMesh->push_back( curVert );
					pVertex += renderbuffer->m_VertexBufferInterface->GetStride();
				}
			}
			vb->Unlock();
		}
		//do index buffers
		IIndexBuffer * ib = renderbuffer->m_IndexBufferInterface;
		if( !ib )return;
		TriangleFace curTri;
		IMaterial * mat = renderbuffer->m_Material;
		DWORD textureID = 0;
		if( mat )
		{
			IBaseTextureObject * basetexture = mat->GetTexture(0,0);
			textureID = (DWORD)basetexture;
		}
		hr = ib->Lock( renderbuffer->m_IndexAllocation.m_Offset, renderbuffer->m_IndexAllocation.m_Size, (void**)&pVertex );
		if( pVertex != NULL )
		{
			for( int i=0; i < numIndices/3; i++ )
			{
				BYTE* posbyte = pVertex;
				short * pShort = (short*)pVertex;
				curTri.index[ 0 ] = pShort[0];
				curTri.index[ 1 ] = pShort[1];								
				curTri.index[ 2 ] = pShort[2];		
				curTri.m_Texture = textureID;
				pVertex += sizeof( short )*3;
				m_Faces->push_back( curTri );
			}
		}
		ib->Unlock();
		//Add texture if not already there
		ITextureObject * texture = NULL;
		IMaterial * curMat = renderbuffer->m_Material;
		if( curMat )
		{
			IBaseTextureObject * baseTexture = curMat->GetTexture(0,0);
			texture = dynamic_cast< ITextureObject* >( baseTexture );
			if( texture )
			{
				m_MaterialTextures.insert( texture );
			}
		}
	}
}
HRESULT WINAPI UVGenCallback( FLOAT fPercentDone,LPVOID lpUserContext)
{
	return  S_OK;
};

void NormalizeUV( float &x )
{
	if( x > 1.f)
	{
		x = x - floor( x );
	}
	else if( x < -1.f )
	{
		x = 1 + x - ceil( x );
	}
}

void MeshParameterization::GetBaseColorAtTriangleIntersection( int& face, float &u, float &v, floatColor &color )
{
	static BYTE bColor[4];		
	bColor[0] = 0;	
	bColor[1] = 0;	
	bColor[2] = 0;	
	bColor[3] = 0;		
	if( m_Faces )
	{
		TriangleFace &triFace = (*m_Faces)[ face ];
		DWORD texID = triFace.m_Texture;
		//ID is just a dword of the pointer
		set< ITextureObject* >::iterator iter = m_MaterialTextures.find( (ITextureObject*)texID );
		if( iter != m_MaterialTextures.end() )
		{
			ITextureObject * texture = (*iter);
			if( texture )
			{
				BYTE * pBytes = (BYTE*)texture->Read();
				if( pBytes )
				{
					//get u v
					int bytedepth = texture->GetColorDepth()/8;
					int texWidth = texture->GetWidth();
					int texHeight = texture->GetHeight();
					int iU = (int)( u *(float)texWidth);
					int iV = (int)(v *(float)texHeight);
					int index = (iV%texHeight)*texWidth + (iU%texWidth);
							
					if( index >= 0 && index < texWidth*texHeight )
					{
						for( int i = 0; i < bytedepth; i++ )
						{
							bColor[i] = pBytes[ index*bytedepth + i];
						}
					}
				}
			}
		}
	}
	color.r = (float)bColor[0]/255.f;
	color.g = (float)bColor[1]/255.f;
	color.b = (float)bColor[2]/255.f;
	color.a = (float)bColor[3]/255.f;
}

void MeshParameterization::GenerateBounds()
{
	//Generate bounds
	m_BoundingBox.Set( tmin, tmax );
	float length = ( tmin - tmax ).Length();
	int originalSize = (int)( length / TEXTURE_TO_WORLD_SPACE_RATIO );
	//find closest power of two
	int powertwo = 0;
	int size = originalSize;
	while( size > 1  )
	{
		size = size >> 1;
		powertwo++;
	}
	m_TexSize = 1 << powertwo;
	int d1 = m_TexSize*2 - originalSize; //next power of two
	int d2 = originalSize - m_TexSize;
	if( d2 > d1 )//closer to higher power
	{
		m_TexSize = m_TexSize*2;
	}
	m_TexSize = min( m_TexSize, MAX_TEXTURE_SIZE );
	m_TexSize = max( m_TexSize, MIN_TEXTURE_SIZE );
	GenerateKDTree();
}

DWORD MeshParameterization::OnGenerateAtlas( DWORD size, void * params )
{
	VERIFY_MESSAGE_SIZE( size, sizeof( GENERATEATLASMESSAGE ) );
	GENERATEATLASMESSAGE * msg = (GENERATEATLASMESSAGE*)params;
	if( !msg )
	{
		return MSG_ERROR;
	}
	bool bUseIncomingTexCoords = msg->useIncomingTexCoords;
	GenerateBounds();
	HRESULT hr;
	//This should be changed to use a new mesh parameterization technique
	DWORD numFaces = (DWORD)m_Faces->size();
	DWORD numVertices = (DWORD)m_CollapsedMesh->size();
	DWORD curError = 0;
	if( !bUseIncomingTexCoords &&
		numFaces > 0 &&
		numVertices > 0 )
	{
		DWORD fvf = D3DFVF_XYZ |  D3DFVF_NORMAL | D3DFVF_TEX1 ;
		DWORD flags =  D3DXMESH_SYSTEMMEM ;//  | D3DXMESH_MANAGED ;// | D3DXMESH_SOFTWAREPROCESSING | D3DXMESH_SYSTEMMEM |;
		
		LPD3DXMESH mesh = NULL;
		hr = D3DXCreateMeshFVF( numFaces, numVertices, flags, fvf, m_pDevice, &mesh );
		if( FAILED( hr ) )
		{
			curError = GetLastError();
			EngineGetToolBox()->Log(LOGERROR, _T("MeshParameterization: Error in create mesh fvf\n"));
			return MSG_ERROR;
		}
		//now fill with data
		BYTE * vertexData;
		BYTE * indexData;
		hr = mesh->LockVertexBuffer( 0, (LPVOID*)&vertexData );
		hr = mesh->LockIndexBuffer( 0, (LPVOID*)&indexData );
		D3DVERTEXELEMENT9 Decl[ MAX_FVF_DECL_SIZE ];
		mesh->GetDeclaration( Decl );
		
		paramVertex * pVertOriginal = (paramVertex*)vertexData;
		for( int i = 0; i < (int)numVertices; i++ )
		{
			paramVertex * pVert = (paramVertex*)vertexData;
			pVert->x = (*m_CollapsedMesh)[i].originalPosition.x;
			pVert->y = (*m_CollapsedMesh)[i].originalPosition.y;
			pVert->z = (*m_CollapsedMesh)[i].originalPosition.z;
			pVert->nx = -(*m_CollapsedMesh)[i].normal.x;
			pVert->ny = -(*m_CollapsedMesh)[i].normal.y;
			pVert->nz = -(*m_CollapsedMesh)[i].normal.z;
			pVert->u = (*m_CollapsedMesh)[i].generatedU;
			pVert->v = (*m_CollapsedMesh)[i].generatedV;
			NormalizeUV( pVert->u );
			NormalizeUV( pVert->v );

			vertexData += sizeof( paramVertex );
		}
		for( int i = 0; i < (int)numFaces; i++ )
		{
			unsigned short * index = (unsigned short*)indexData;
			index[0] = ( unsigned short )(*m_Faces)[ i ].index[ 0 ];
			index[1] = ( unsigned short )(*m_Faces)[ i ].index[ 1 ];
			index[2] = ( unsigned short )(*m_Faces)[ i ].index[ 2 ];
			indexData += sizeof( unsigned short )*3;//32 bit indices triangles
		}

		LPD3DXBUFFER imt;
		hr = D3DXComputeIMTFromPerVertexSignal( mesh, (const float*)pVertOriginal + 3*sizeof(float), 3, sizeof(paramVertex),
											0L, 0, 0, &imt );		
		mesh->UnlockIndexBuffer();
		mesh->UnlockVertexBuffer();
		//tensors
   		float * tensors = new float[ 3*numFaces ];
   		for( int i = 0; i < 3*(int)numFaces; i += 3 )
   		{
   			tensors[ i ] = 4.f;
   			tensors[ i + 1 ] = 0.f;
   			tensors[ i + 2 ] = 4.f;
   		}
		
		//some checks 
		numVertices = mesh->GetNumVertices();
		numFaces = mesh->GetNumFaces();

		//create adjacency
		DWORD * adjacency = new DWORD[	3*numFaces ];
		memset( adjacency, 0, sizeof(DWORD)*3*numFaces );
		hr = mesh->GenerateAdjacency( 0.001f, adjacency );
		//hr = mesh->ConvertPointRepsToAdjacency( NULL, adjacency );
		if( FAILED( hr ) )
		{
			curError = GetLastError();
			EngineGetToolBox()->Log(LOGERROR, _T("MeshParameterization: Error in generate adjacency\n"));
			return MSG_ERROR;
		}
		/* save to mesh to check model uvs
		D3DXMATERIAL mat;
		mat.MatD3D.Ambient.r = mat.MatD3D.Ambient.a =mat.MatD3D.Ambient.b =mat.MatD3D.Ambient.g = 0;
		mat.MatD3D.Diffuse.r = mat.MatD3D.Diffuse.a =mat.MatD3D.Diffuse.b =mat.MatD3D.Diffuse.g = 1;
		mat.pTextureFilename = "tex.dds";
		D3DXSaveMeshToX( "mesh.x", mesh, adjacency, &mat, NULL, 0, D3DXF_FILEFORMAT_TEXT );
		*/

		float * imtTensor = tensors;//(float*)imt->GetBufferPointer();

		float stretchout;
		unsigned int charts;
		LPD3DXMESH meshOut = NULL;
		LPD3DXBUFFER remappedData = NULL;
		LPD3DXBUFFER faceData = NULL;

		D3DXATTRIBUTERANGE Attrib;
		memset(&Attrib, 0, sizeof(D3DXATTRIBUTERANGE));
		Attrib.FaceCount = numFaces;
		Attrib.VertexCount = numVertices;
		mesh->SetAttributeTable(&Attrib, 1);
		int gutter = m_TexSize / 32;
		gutter = min( gutter, 6 );
		hr = D3DXUVAtlasCreate( mesh, 0, .5, m_TexSize, m_TexSize, 
								6, //gutter
								0, adjacency, 0,
								imtTensor, (LPD3DXUVATLASCB)UVGenCallback, .0001f, 0,
								//D3DXUVATLAS_GEODESIC_QUALITY ,
								D3DXUVATLAS_GEODESIC_FAST,
								&meshOut, &faceData, &remappedData, 
								&stretchout, &charts );
		if( FAILED( hr ) )
		{
			curError = GetLastError();
			EngineGetToolBox()->Log(LOGERROR, _T("MeshParameterization: Error in uv atlas create\n"));
			return MSG_ERROR;
		}
		/* save to mesh to check model uvs
		hr = meshOut->ConvertPointRepsToAdjacency( NULL, adjacency );
		D3DXSaveMeshToX( "mesh2.x", meshOut, adjacency, &mat, NULL, 0, D3DXF_FILEFORMAT_TEXT );
		*/
		delete [] adjacency;
		delete [] tensors;

		//Generate our lightmap cache data for passing on and saving
		GenerateCache( meshOut, remappedData );
		GenerateTriangleTexelData();
		
		meshOut->Release();
		faceData->Release();
		remappedData->Release();
		mesh->Release();
	}    
	else 
	if( bUseIncomingTexCoords )
	{
		GenerateCache();
		GenerateTriangleTexelData();
	}
	return MSG_HANDLED_STOP;
}

void SwapPointers( ParameterizationVertex ** v, ParameterizationVertex ** u )
{
	ParameterizationVertex *p = *v;
	*v = *u;
	*u = p;
}

float Vec2Length( float x, float y )
{
	return sqrtf( x*x + y*y );
}

void MeshParameterization::GenerateCache( LPD3DXMESH meshOut, LPD3DXBUFFER remapped )
{
	paramVertex * pNewVerts = NULL;
	IToolBox * tool = EngineGetToolBox();
			
	HRESULT hr;
	m_Cache.Clean();
	//have to now be able to map a face to a texture area, or, a face to a chart.
	int numVertices = m_CollapsedMesh->size();
	int numNewVertices = remapped->GetBufferSize() / sizeof( DWORD );

	if( numNewVertices != numVertices )
	{
		//Our cache is a remapping cache! New vertices were generated!!!!
		m_Cache.m_bRemapped = true;

		//First we remap our vertices
		int remapsize = remapped->GetBufferSize() / sizeof( DWORD );
		DWORD * remappedData = (DWORD*) remapped->GetBufferPointer();

		m_Cache.m_NumVertices = remapsize;
		m_Cache.ResizeRemappedData( remapsize );
		m_Cache.ResizeUVData(  remapsize*2 );
		memcpy( m_Cache.m_RemappedData, remappedData, sizeof( DWORD )*remapsize );

		vector< ParameterizationVertex > newVertBuffer;
		newVertBuffer.resize( remapsize );

		//loop through and assign new vertices
		hr = meshOut->LockVertexBuffer( 0, (LPVOID*)&pNewVerts );
		if( hr == D3D_OK &&
			pNewVerts)
		{
			for( int i = 0; i < remapsize; i++ )
			{
				int oldIndex = remappedData[ i ];
				newVertBuffer[ i ] = (*m_CollapsedMesh)[ oldIndex ];
				//UV for internal lightmap calcs
				NormalizeUV( pNewVerts[ i ].u );
				NormalizeUV( pNewVerts[ i ].v );
				newVertBuffer[ i ].generatedU = pNewVerts[ i ].u;
				newVertBuffer[ i ].generatedV = pNewVerts[ i ].v;
				//Duplicate uv to pass to render object as a stream
				m_Cache.m_UVData[ i*2 ]		= pNewVerts[ i ].u;
				m_Cache.m_UVData[ i*2 + 1 ] = pNewVerts[ i ].v;
			}
			meshOut->UnlockVertexBuffer();
			//assign new mesh
			m_CollapsedMesh->clear();
			(*m_CollapsedMesh) = newVertBuffer;
			newVertBuffer.clear();
		}else
		{
			tool->Log( LOGERROR, _T("Could not lock mesh param vertex buffer to remap vertices\n"));

		}
		//now do indices
		int numIndices = 3*meshOut->GetNumFaces();
		m_Cache.m_NumIndices = numIndices;
		m_Cache.ResizeRemappedIndices( numIndices );
		WORD * pIndices = NULL;
		hr = meshOut->LockIndexBuffer( 0, (LPVOID*)&pIndices );
		if( hr == D3D_OK &&
			pIndices)
		{
			for( int i = 0; i < numIndices; i++ )
			{
				m_Cache.m_RemappedIndices[ i ] = (DWORD)pIndices[ i ];
				//faces
				int faceIndex = i/3;
				int triIndex = i%3;
				(*m_Faces)[ faceIndex ].index[ triIndex ] = pIndices[ i ];
			}
		}else if( tool )
		{
			tool->Log( LOGERROR, _T("Could not lock mesh param index buffer to remap indices\n"));
		}
	}
	else //no remapping, just uvs needed
	{
		BYTE * pNewVerts = NULL;
		hr = meshOut->LockVertexBuffer( 0, (LPVOID*)&pNewVerts );
		
		m_Cache.m_NumVertices = m_CollapsedMesh->size();
		m_Cache.ResizeUVData( m_Cache.m_NumVertices*2 );
		if( hr == D3D_OK )
		{
			//copy new uvs
			paramVertex * pv = (paramVertex*)pNewVerts;
			for( int i = 0; i < (int)m_CollapsedMesh->size(); i++ )
			{
				NormalizeUV( pv[ i ].u );
				NormalizeUV( pv[ i ].v );
				(*m_CollapsedMesh)[i].generatedU = pv[ i ].u;
				(*m_CollapsedMesh)[i].generatedV = pv[ i ].v;
				//copy uvs for passing to render object as a stream
				m_Cache.m_UVData[ i*2 ] = pv[ i ].u;
				m_Cache.m_UVData[ i*2 + 1 ] = pv[ i ].v;
			}
			meshOut->UnlockVertexBuffer();
		}
	}
}

void MeshParameterization::GenerateCache( )
{
	IToolBox * tool = EngineGetToolBox();
	m_Cache.Clean();
	//no remapping, just uvs needed
	{
		m_Cache.m_NumVertices = m_CollapsedMesh->size();
		m_Cache.ResizeUVData( m_Cache.m_NumVertices*2 );
		//copy new uvs
		for( int i = 0; i < (int)m_CollapsedMesh->size(); i++ )
		{
			//copy uvs for passing to render object as a stream
			m_Cache.m_UVData[ i*2 ] = (*m_CollapsedMesh)[i].generatedU;
			m_Cache.m_UVData[ i*2 + 1 ] = (*m_CollapsedMesh)[i].generatedV;
		}
	}
}
extern void OutputVector( Vec3 &vec, char * name );
extern void OutputVector(float array[3], char * name );
struct blah
{
	int index;
	float area;
	int pixels;
};
void MeshParameterization::GenerateTriangleTexelData()
{
#if ENABLE_DEBUG_DEGENERATES
	
	map< float, blah > s_DegenerateList;
#endif
	//Generate texel data per triangle
	const float uIncrement = 1.f/(float)m_TexSize;
	const float vIncrement = 1.f/(float)m_TexSize;
	const float uHalfIncrement = uIncrement*.5f;
	const float vHalfIncrement =  vIncrement*.5f;
	ParameterizationVertex * p0;
	ParameterizationVertex * p1;
	ParameterizationVertex * p2;
	//0 is smallest y value
	//1 is smallest x value that's not 0
	TriangleTextureMapping toAdd;
	Vec3 barycentric;
	float u;
	float uMax;
	for( int i = 0; i < (int)m_Faces->size(); i++ )
	{
		TriangleFace &tri = (*m_Faces)[ i ];
		bool DangerTriangle = false;
		//get verts
		p0 = &(*m_CollapsedMesh)[ tri.index[ 0 ] ];
		p1 = &(*m_CollapsedMesh)[ tri.index[ 1 ] ];
		p2 = &(*m_CollapsedMesh)[ tri.index[ 2 ] ];
		
		NormalizeUV( p1->generatedU );
		NormalizeUV( p1->generatedV );
		NormalizeUV( p2->generatedU );
		NormalizeUV( p2->generatedV );
		NormalizeUV( p0->generatedU );
		NormalizeUV( p0->generatedV );
		if( p0->generatedV > p1->generatedV )
		{
			SwapPointers( &p0, &p1 ); 
		}
		if( p0->generatedV > p2->generatedV )
		{ 
			SwapPointers( &p0, &p2 );
		}
		if( p1->generatedU > p2->generatedU )
		{
			SwapPointers( &p1, &p2 ); 
		}			
		//find slopes
		float slope1, slope2, slope3, slope4;
		bool undefinedSlope[4];
		undefinedSlope[0] = false;
		undefinedSlope[1] = false;
		undefinedSlope[2] = false;
		undefinedSlope[3] = false;
		if( fabs( p0->generatedV - p1->generatedV ) <= EPSILON_ZERO )
		{ 
			undefinedSlope[0] = true; 
		}
		if( fabs( p0->generatedV - p2->generatedV ) <= EPSILON_ZERO )
		{ 
			undefinedSlope[1] = true;
		}
		if( fabs( p1->generatedV - p2->generatedV ) <= EPSILON_ZERO )
		{
			undefinedSlope[2] = true; 
		}
		if( fabs( p2->generatedV - p1->generatedV ) <= EPSILON_ZERO )
		{ 
			undefinedSlope[3] = true;
		}
		slope1 = ( p0->generatedU - p1->generatedU )/(p0->generatedV - p1->generatedV );
		slope2 = ( p0->generatedU - p2->generatedU )/(p0->generatedV - p2->generatedV );
		slope3 = ( p1->generatedU - p2->generatedU )/(p1->generatedV - p2->generatedV );
		slope4 = ( p2->generatedU - p1->generatedU )/(p2->generatedV - p1->generatedV );
		float maxV = ( p1->generatedV > p2->generatedV ) ? p1->generatedV : p2->generatedV;
		float maxU = ( p0->generatedU > p2->generatedU ) ? p0->generatedU : p2->generatedU;
		maxU += uIncrement;
		maxV += vHalfIncrement;
		for( float v = p0->generatedV - vHalfIncrement; v <= maxV ; v += vIncrement )
		{
			if( p1->generatedV >= v &&
				p2->generatedV >= v )
			{
				//find endpoints of scanline				
				u = undefinedSlope[0] ?  p1->generatedU : p1->generatedU + slope1*( v - p1->generatedV ) - uHalfIncrement;
				uMax = undefinedSlope[1] ? p2->generatedU : p2->generatedU + slope2*( v - p2->generatedV ) + uHalfIncrement;
			}
			else if( p2->generatedV <= v )
			{
				//find endpoints of scanline
				u = undefinedSlope[0] ? p1->generatedU : p1->generatedU + slope1*( v - p1->generatedV )- uHalfIncrement;
				uMax = undefinedSlope[3] ? p2->generatedU : p1->generatedU + slope4*( v - p1->generatedV ) + uHalfIncrement;
			}
			else // p1->v is smaller than v, we're need a new edge
			{
				//find endpoints of scanline
				u =  undefinedSlope[2] ? p1->generatedU : p2->generatedU + slope3*( v - p2->generatedV ) - uHalfIncrement;
				uMax =  undefinedSlope[1] ? p2->generatedU : p2->generatedU + slope2*( v - p2->generatedV ) + uHalfIncrement;
			}
			if( u > uMax ){ float temp = u; u = uMax; uMax = temp; }
			uMax = min( uMax, maxU );
			float umin = min( p1->generatedU, p0->generatedU - uIncrement);
			u = max( u, umin);
			for( ; u <= uMax; u+= uIncrement )
			{
				//we have u and v, get barycentric: 				
				//check if slope will be undefined
				float qX, qY;
				float slopeBC, b2, b1, slope;
				float slopeDenom = u - p0->generatedU;
				float slopeDenom2 = p1->generatedU - p2->generatedU;
				if( slopeDenom == 0 &&
					slopeDenom2 != 0)
				{
                    //vertical line, we know x coordinate
					slopeBC = ( p1->generatedV - p2->generatedV )/( p1->generatedU - p2->generatedU );
					b2 = p1->generatedV - slopeBC*p1->generatedU;
					//solve for y with known x coordinate
					qX = p0->generatedU;
					qY = slopeBC*qX + b2;
				}else if( slopeDenom2 != 0 &&
					slopeDenom != 0 )
				{
					slope = ( v - p0->generatedV )/slopeDenom;
					//find other slope, will never be undefined
					slopeBC = ( p1->generatedV - p2->generatedV )/( p1->generatedU - p2->generatedU );
					//now solve for intersect
					b1 = v - slope*u;
					b2 = p1->generatedV - slopeBC*p1->generatedU;
					qX = ( b1 - b2 )/(slopeBC - slope);
					qY = slope*qX + b1;
					//now find length for bary
				}else if( slopeDenom2 == 0 && slopeDenom != 0 )
				{
					slope = ( v - p0->generatedV )/slopeDenom;
					//now solve for intersect
					b1 = v - slope*u;
					qX = p2->generatedU;
					qY = slope*qX + b1;
					//now find length for bary
				}else //both equal 0
				{
					//this should never happen!
					OutputDebugString("Slopes of two triangle sides are both zero!!!\n");
					qX = p2->generatedU;
					qY = p2->generatedV;
				}
				barycentric.z = Vec2Length( p1->generatedU - qX, p1->generatedV - qY );
				barycentric.y = Vec2Length( p2->generatedU - qX, p2->generatedV - qY );
				//now find t to balance these out at q on p
				float ratio =  Vec2Length( qX - u, qY - v ) 
								/Vec2Length( u - p0->generatedU, v - p0->generatedV);
                barycentric.x = ( barycentric.y + barycentric.z )*ratio;

				float normalize = barycentric.x + barycentric.y + barycentric.z;
				if( normalize != 0 )
				{
					barycentric.x /= normalize;
					barycentric.y /= normalize;
					barycentric.z /= normalize;

					toAdd.localSpaceCoord = p0->originalPosition*barycentric.x + 
											p1->originalPosition*barycentric.y + 
											p2->originalPosition*barycentric.z;		
					
					if( toAdd.localSpaceCoord.x != toAdd.localSpaceCoord.x )
					{
						DangerTriangle = true;
						//OutputDebugString( "DANGER WILL ROBINSON\n");
					}
					toAdd.localNormal = p0->normal*barycentric.x + 
										p1->normal*barycentric.y + 
										p2->normal*barycentric.z;
					
					toAdd.u = (int)( u*(float)m_TexSize );
					toAdd.v = (int)( v*(float)m_TexSize );
					if( toAdd.localSpaceCoord.x == toAdd.localSpaceCoord.x &&
						toAdd.u >= 0 && toAdd.u < m_TexSize &&
						toAdd.v >= 0 && toAdd.v < m_TexSize )
					{
						tri.m_Pixels.push_back( toAdd );
					}
				}
									
			}
			static int numcont = 0;
			numcont++;
			if( DangerTriangle &&
				numcont < 200 )
			{
				ADDLINEPARAMS LineParam;
				static CHashString h(_T("none"));
				LineParam.name = &h;
				LineParam.blue = 0;
				LineParam.green = 0;
				LineParam.red = 255;
				LineParam.start = p0->originalPosition;
				LineParam.end = p1->originalPosition;
				//static DWORD msgHash_AddLine = CHashString(_T("AddLine")).GetUniqueID();
				//EngineGetToolBox()->SendMessage(msgHash_AddLine,sizeof(LineParam), &LineParam );
				LineParam.start = p0->originalPosition;
				LineParam.end = p2->originalPosition;
				//EngineGetToolBox()->SendMessage(msgHash_AddLine,sizeof(LineParam), &LineParam );
				LineParam.start = p2->originalPosition;
				LineParam.end = p1->originalPosition;
				//EngineGetToolBox()->SendMessage(msgHash_AddLine,sizeof(LineParam), &LineParam );
			}
		}
		//test code
#if ENABLE_DEBUG_DEGENERATES
		Vec3 edge1 = Vec3( p0->generatedU, p0->generatedV, 0 ) - Vec3( p1->generatedU, p1->generatedV, 0 );
		Vec3 edge2 = Vec3( p0->generatedU, p0->generatedV, 0 ) - Vec3( p2->generatedU, p2->generatedV, 0 );
		float lengthA = edge1.Length();
		float lengthB = (Vec3( p1->generatedU, p1->generatedV, 0 ) - Vec3( p2->generatedU, p2->generatedV, 0 )).Length();
		float lengthC = edge2.Length();
		float halfPerimeter = (lengthA + lengthB + lengthC)*.500f;
		float area = halfPerimeter*( halfPerimeter - lengthA )*(halfPerimeter - lengthB )*(halfPerimeter - lengthC);
		area = sqrt( area );
		float numPixels = (float)tri.m_Pixels.size();
		edge1.Normalize();
		edge2.Normalize();
		float flatTriangle = edge2.Dot( edge1 );
		//if( fabs( flatTriangle ) < .7f)
		{
			area *= (float)(m_TexSize*m_TexSize);
			float areaRatio = numPixels/area;
			if( areaRatio < .8f ) //half the uv pixels aren't generate?!#!
			{
				blah ref;
				ref.area = area;
				ref.pixels = (int)numPixels;
				ref.index = i;
				s_DegenerateList.insert( pair< float, blah >( numPixels, ref ) );
			}
		}
#endif
		if( tri.m_Pixels.size() < 2)
		{
			//something wrong here
			//OutputDebugString("Degenerate Triangle\n");
			//OutputVector( p0->originalPosition, "p0" );
			//OutputVector( p1->originalPosition, "p1" );
			//OutputVector( p2->originalPosition, "p2" );
			//int a = 0;
		}
	}	
#if ENABLE_DEBUG_DEGENERATES
	static char buf[1024];
	for( map< float, blah >::iterator iter = s_DegenerateList.begin();
		iter != s_DegenerateList.end();
		++iter )
	{
		blah &ref = iter->second;
		TriangleFace &tri = (*m_Faces)[ ref.index  ];
		//get verts
		p0 = &(*m_CollapsedMesh)[ tri.index[ 0 ] ];
		p1 = &(*m_CollapsedMesh)[ tri.index[ 1 ] ];
		p2 = &(*m_CollapsedMesh)[ tri.index[ 2 ] ];		
		//sprintf( buf, "Degenerate Triangle with area: %f, pixels: %d, index: %d \n", ref.area, ref.pixels, ref.index );
		//OutputDebugString( buf );
		//OutputVector( p0->originalPosition, "p0" );
		//OutputVector( p1->originalPosition, "p1" );
		//OutputVector( p2->originalPosition, "p2" );
		ADDLINEPARAMS LineParam;
		static CHashString h(_T("none"));
		LineParam.name = &h;
		LineParam.blue = 0;
		LineParam.green = 0;
		LineParam.red = 255;
		LineParam.start = p0->originalPosition;
		LineParam.end = p1->originalPosition;
		static DWORD msgHash_AddLine = CHashString(_T("AddLine")).GetUniqueID();
		EngineGetToolBox()->SendMessage(msgHash_AddLine,sizeof(LineParam), &LineParam );
		LineParam.start = p0->originalPosition;
		LineParam.end = p2->originalPosition;
		static DWORD msgHash_AddLine = CHashString(_T("AddLine")).GetUniqueID();
		EngineGetToolBox()->SendMessage(msgHash_AddLine,sizeof(LineParam), &LineParam );
		LineParam.start = p2->originalPosition;
		LineParam.end = p1->originalPosition;
		static DWORD msgHash_AddLine = CHashString(_T("AddLine")).GetUniqueID();
		EngineGetToolBox()->SendMessage(msgHash_AddLine,sizeof(LineParam), &LineParam );
	}
#endif
}

DWORD MeshParameterization::SetCache( DWORD size, void * params )
{
	VERIFY_MESSAGE_SIZE( size, sizeof( LightmapCache ) );
	LightmapCache * cache = (LightmapCache*)params;
	if( !cache )
	{
		return MSG_ERROR;
	}
	m_Cache = *cache;
	if( m_Cache.m_NumVertices != (int)m_CollapsedMesh->size() )
	{
		//Our cache is a remapping cache! New vertices were generated!!!!
		m_Cache.m_bRemapped = true;

		//First we remap our vertices
		vector< ParameterizationVertex > newVertBuffer;
		newVertBuffer.resize( m_Cache.m_NumVertices );
		//loop through and assign new vertices
		for( int i = 0; i < m_Cache.m_NumVertices; i++ )
		{
			int oldIndex = m_Cache.m_RemappedData[ i ];
			newVertBuffer[ i ] = (*m_CollapsedMesh)[ oldIndex ];
			//UV for internal lightmap calcs
			NormalizeUV( m_Cache.m_UVData[ i*2 ] );
			NormalizeUV( m_Cache.m_UVData[ i*2 + 1 ] );
			newVertBuffer[ i ].generatedU = m_Cache.m_UVData[ i*2 ];
			newVertBuffer[ i ].generatedV =  m_Cache.m_UVData[ i*2 + 1 ];
		}
		//assign new mesh
		m_CollapsedMesh->clear();
		(*m_CollapsedMesh) = newVertBuffer;
		newVertBuffer.clear();
		//now do indices
		int numIndices = m_Cache.m_NumIndices;
		for( int i = 0; i < numIndices; i++ )
		{
			//faces
			int faceIndex = i/3;
			int triIndex = i%3;
			(*m_Faces)[ faceIndex ].index[ triIndex ] = (unsigned short )m_Cache.m_RemappedIndices[ i ];
		}
		
	}
	return MSG_HANDLED_STOP;
}

DWORD MeshParameterization::SetMeshTransform( DWORD size, void * params )
{
	VERIFY_MESSAGE_SIZE( size, sizeof( Matrix4x4 ) );
	Matrix4x4 * transform = (Matrix4x4*)params;
	if( transform )
	{
		if( m_Transform ){ delete m_Transform; }
		if( m_InverseTransform ){ delete m_InverseTransform; }
		m_Transform = new Matrix4x4();
		m_InverseTransform = new Matrix4x4();
		transform->GetInverse( *m_InverseTransform );
		*m_Transform = (*transform);
	}
	return MSG_HANDLED_STOP;
}

DWORD MeshParameterization::OnSetTextureSize( DWORD size, void * params )
{
	VERIFY_MESSAGE_SIZE( size, sizeof( DWORD ) );
	DWORD * texsize = (DWORD*)params;
	if( texsize )
	{
		m_TexSize = *texsize;
	}
	return MSG_HANDLED_STOP;
}

DWORD MeshParameterization::GetCache( DWORD size, void * params )
{
	VERIFY_MESSAGE_SIZE( size, sizeof( SETLIGHTMAPDATAMSG ) );
	SETLIGHTMAPDATAMSG * cache = (SETLIGHTMAPDATAMSG*)params;
	if( cache )
	{
		cache->cache = &m_Cache;
		return MSG_HANDLED_STOP;
	}
	return MSG_ERROR;
}

bool NodeCreationCallback( KDTreeNode * node )
{
	if( node &&
		//node->pntidx >= 0 &&
		mp_MeshP)
	{
		static int numtimes = 0;
		static CHashString name( _T("none"));
		ADDAXISALIGNEDBOXPARAMS boxrender;
		boxrender.name = &name;
		boxrender.min = (*mp_MeshP->m_Transform)*node->bounds.m_Bounds[0];
		boxrender.max = (*mp_MeshP->m_Transform)*node->bounds.m_Bounds[1];
		float zdiff = boxrender.max.z - boxrender.min.z;
		
		boxrender.alpha = 50;
		boxrender.green = 0;
		boxrender.blue = 0;
		boxrender.red = 255;
		//static DWORD msgHash_AddAxisAlignedBox = CHashString(_T("AddAxisAlignedBox")).GetUniqueID();
		//EngineGetToolBox()->SendMessage(msgHash_AddAxisAlignedBox,sizeof(boxrender), &boxrender );
		//go through tris and insert any that fit match
		int numtris = (int)mp_MeshP->m_Faces->size();
		static Vec3 *v0, *v1, *v2;
		for( int i = 0; i < numtris; i++ )
		{
			TriangleFace &f = (*mp_MeshP->m_Faces)[ i ];
			v0 = &((*mp_MeshP->m_CollapsedMesh)[ f.index[0] ].originalPosition);
			v1 = &((*mp_MeshP->m_CollapsedMesh)[ f.index[1] ].originalPosition);
			v2 = &((*mp_MeshP->m_CollapsedMesh)[ f.index[2] ].originalPosition);
			float maxX = max( max( v0->x, v1->x ), v2->x );
			float maxY = max( max( v0->y, v1->y ), v2->y );
			float maxZ = max( max( v0->z, v1->z ), v2->z );
			float minX = min( min( v0->x, v1->x ), v2->x );
			float minY = min( min( v0->y, v1->y ), v2->y );
			float minZ = min( min( v0->z, v1->z ), v2->z );
			/*if( node->bounds.bounds[0].x > maxX ||
				node->bounds.bounds[1].x < minX ||
				node->bounds.bounds[0].y > maxY ||
				node->bounds.bounds[1].y < minY ||
				node->bounds.bounds[0].z > maxZ ||
				node->bounds.bounds[1].z < minZ )
			{
			}
			else//*/
			if( node->bounds.IntersectTri( *v0, *v1, *v2 ) ) //test tri intersection
			{
				node->objectList.push_back( i );
				if( zdiff > 20 &&
				numtimes == 0 )
				{
					
					//draw the damn triangles
					//means no triangles added, draw this damn box
					ADDLINEPARAMS LineParam;
					static CHashString h(_T("none"));
					LineParam.name = &h;
					LineParam.blue = 0;
					LineParam.green = 0;
					LineParam.red = 255;
					LineParam.start = *v0;
					LineParam.end = *v1;
					static DWORD msgHash_AddLine = CHashString(_T("AddLine")).GetUniqueID();
					EngineGetToolBox()->SendMessage(msgHash_AddLine,sizeof(LineParam), &LineParam );
					LineParam.start = *v0;
					LineParam.end = *v2;
					EngineGetToolBox()->SendMessage(msgHash_AddLine,sizeof(LineParam), &LineParam );
					LineParam.start = *v2;
					LineParam.end = *v1;
					EngineGetToolBox()->SendMessage(msgHash_AddLine,sizeof(LineParam), &LineParam );
				}
			}
			if( zdiff > 20 )
			{
				numtimes++;
			}
		}
		static int a = 0;
		//if( a < 2 )
		/*/if( node->objectList.size() <= 0 )
		{
			 a++;
			//means no triangles added, draw this damn box
			static CHashString name( _T("none"));
			ADDAXISALIGNEDBOXPARAMS boxrender;
			boxrender.name = &name;
			boxrender.min = (*mp_MeshP->m_Transform)*node->bounds.m_Bounds[0];
			boxrender.max = (*mp_MeshP->m_Transform)*node->bounds.m_Bounds[1];
			
			boxrender.alpha = 255;
			boxrender.green = 0;
			boxrender.blue = 0;
			boxrender.red = 255;
			static DWORD msgHash_AddAxisAlignedBox = CHashString(_T("AddAxisAlignedBox")).GetUniqueID();
			EngineGetToolBox()->SendMessage(msgHash_AddAxisAlignedBox,sizeof(boxrender), &boxrender );
		}
		assert( node->objectList.size() > 0 );*/
	}
	return true;
}

void MeshParameterization::GenerateKDTree()
{
	mp_MeshP = this;
	if( !m_KDTree )
	{
		int ndim = 3;
		m_KDTree = new KDTree();
		int sizeverts = (int)(m_CollapsedMesh->size());
		m_KDvertexData = new Vec3[ sizeverts ];
		for( int i = 0; i < sizeverts; i++ )
		{
			m_KDvertexData[ i ] = (*m_CollapsedMesh)[ i ].originalPosition;
		}
		m_KDTree->SetNodeCreationCallback( NodeCreationCallback );
		m_KDTree->create( (float*)m_KDvertexData, sizeverts, ndim );
	}else
	{
		//TODO: make sure it was filled earlier if we really want to regenerate? This path is not currently used
		//kd trees are generated once
	}
}

bool	RayCallback( vector< KDTreeNode * > &nodes)
{
	mp_Intersected = false;
	Vec3 v0, v1, v2;
	if( nodes.size() > 0 &&
		//node->pntidx >= 0 &&
		mp_MeshP ) //leaf node
	{
		//test against triangle index
		static double su, st, sv;
		double temp = 999999;
		int numNodes = nodes.size();
		for( int i = 0; i < numNodes; i++ )
		{
			KDTreeNode * node = nodes[ i ];
			int size = node->objectList.size();
			INDEXLIST::iterator iter = node->objectList.begin();
			for( ; iter != node->objectList.end(); ++iter )
			{
				int TriIndex =(*iter); //index refers to vertices, 3 verts per tri
				TriangleFace &tri = (*mp_MeshP->m_Faces)[ TriIndex ];
				v0 = (*mp_MeshP->m_CollapsedMesh)[ tri.index[0] ].originalPosition;
				v1 = (*mp_MeshP->m_CollapsedMesh)[ tri.index[1] ].originalPosition;
				v2 = (*mp_MeshP->m_CollapsedMesh)[ tri.index[2] ].originalPosition;
				
				bool value = mp_Ray.IntersectTriangle( v0, v1, v2, &st, &su, &sv );
				if( value && 
					st >= 0 &&
					temp > st )//distance of this intersection is minimum, make min intersection this one
				{
					temp = st;
					mp_T = st;
					mp_U = su;
					mp_V = sv;
					mp_Intersected = true;
					mp_TriIndex = TriIndex;
				}
			}
		}
		return mp_Intersected;
	}
	return false;
}

//look for ray trace
bool MeshParameterization::IntersectRayTriangle( Ray &ray, int &triIndex, double * t, double * u, double * v )
{
	mp_MeshP = this;
	if( !m_KDTree )
	{
		OutputDebugString( "No KD Tree created yet!\n");
		return false;
	}	
	mp_Ray = ray;
	Vec3 rMin, rMax;
	rMin.x = min( ray.m_Origin.x, tmin.x );
	rMin.y = min( ray.m_Origin.y, tmin.y );
	rMin.z = min( ray.m_Origin.z, tmin.z );
	rMax.x = max( ray.m_Origin.x, tmax.x );
	rMax.y = max( ray.m_Origin.y, tmax.y );
	rMax.z = max( ray.m_Origin.z, tmax.z );

	m_KDTree->FindNearestToRay2( ray, tmin, tmax, RayCallback, m_Transform );
	if( mp_Intersected )
	{
		triIndex = mp_TriIndex;
		*t = mp_T;
		*u = mp_U;
		*v = mp_V;
		mp_Intersected = false;
		return true;
	}
	return false;
}

DWORD MeshParameterization::OnGetCollapsedMesh( DWORD size, void * params )
{
	VERIFY_MESSAGE_SIZE( size, sizeof( GETPARAMETERIZEDVERTICESMSG ) );
	GETPARAMETERIZEDVERTICESMSG * msg = (GETPARAMETERIZEDVERTICESMSG*)params;
	if( msg != NULL )
	{
		msg->outList = m_CollapsedMesh;
		return MSG_HANDLED_STOP;
	}
	return MSG_ERROR;
}

DWORD MeshParameterization::OnGetTriangleFaces( DWORD size, void * params )
{
	VERIFY_MESSAGE_SIZE( size, sizeof( GETPARAMETERIZEDTRIANGLESMSG ) );
	GETPARAMETERIZEDTRIANGLESMSG * msg = (GETPARAMETERIZEDTRIANGLESMSG*)params;
	if( msg != NULL )
	{
		msg->outList = m_Faces;
		return MSG_HANDLED_STOP;
	}
	return MSG_ERROR;
}

DWORD MeshParameterization::OnGetMeshTransform( DWORD size, void * params )
{
	VERIFY_MESSAGE_SIZE( size, sizeof( Matrix4x4 ) );
	Matrix4x4 * mat = (Matrix4x4*)params;
	if( mat != NULL )
	{
		mat = m_Transform;
		return MSG_HANDLED_STOP;
	}
	return MSG_ERROR;
}

DWORD MeshParameterization::OnGetMeshInverseTransform( DWORD size, void * params )
{
	VERIFY_MESSAGE_SIZE( size, sizeof( Matrix4x4 ) );
	Matrix4x4 * mat = (Matrix4x4*)params;
	if( mat != NULL )
	{
		mat = m_InverseTransform;
		return MSG_HANDLED_STOP;
	}
	return MSG_ERROR;
}

DWORD MeshParameterization::OnGetAABB( DWORD size, void * params )
{
	VERIFY_MESSAGE_SIZE( size, sizeof( AABB ) );
	AABB * msg = (AABB*)params;
	if( msg != NULL )
	{
		*msg = m_BoundingBox;
		return MSG_HANDLED_STOP;
	}
	return MSG_ERROR;
}

DWORD MeshParameterization::OnGenerateBounds( DWORD size, void * params )
{
	GenerateBounds();
	return MSG_HANDLED_STOP;
}
DWORD MeshParameterization::OnIntersectRayTriangle( DWORD size, void * params )
{
	VERIFY_MESSAGE_SIZE( size, sizeof( MESHPARAMINTERSECTRAYTRIANGLEMSG ) );
	MESHPARAMINTERSECTRAYTRIANGLEMSG * msg = (MESHPARAMINTERSECTRAYTRIANGLEMSG*)params;
	if( msg != NULL )
	{
		if( msg->inRay != NULL )
		{			
			Ray inRay = *msg->inRay;
			msg->outCollided = 
				IntersectRayTriangle( inRay, msg->outFaceIndex, &msg->outIntersectionDistance, &msg->outULength, &msg->outVLength );
			return MSG_HANDLED_STOP;
		}
	}
	return MSG_ERROR;
}
DWORD MeshParameterization::OnGenerateTriangleTexelData( DWORD size, void * params )
{
	GenerateTriangleTexelData();
	return MSG_HANDLED_STOP;
}

DWORD MeshParameterization::OnGetBaseColorAtTriangleIntersection(  DWORD size, void * params )
{
	VERIFY_MESSAGE_SIZE( size, sizeof( MESHPARAMGETBASECOLORATTRIANGLEINTERSECTIONMSG ) );
	MESHPARAMGETBASECOLORATTRIANGLEINTERSECTIONMSG * msg = (MESHPARAMGETBASECOLORATTRIANGLEINTERSECTIONMSG*) params;
	if( msg != NULL )
	{
		GetBaseColorAtTriangleIntersection( msg->inFace, msg->inUBaryCentric, msg->inVBaryCentric, msg->outColor );
		return MSG_HANDLED_STOP;
	}
	return MSG_ERROR;
}