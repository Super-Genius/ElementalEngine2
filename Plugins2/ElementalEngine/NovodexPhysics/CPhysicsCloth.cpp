///==========================================================================
/// \file	CPhysicsCloth.cpp
/// \brief	Physics object for cloth
/// \date	7/16/2007
/// \author	Kyle Swaim
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

CPhysicsCloth::CPhysicsCloth()
{
	m_ToolBox = EngineGetToolBox();
	// Get The manager
	IComponent* managerComponent = m_ToolBox->GetComponent( &CHashString( _T("CPhysicsManager")) );
	m_Manager = dynamic_cast<CPhysicsManager*>( managerComponent );

	m_Cloth = NULL;
	m_ClothMesh = NULL;
	m_Name = NULL;
}

CPhysicsCloth::CPhysicsCloth(NxClothDesc& clothDesc)
{
	m_ToolBox = EngineGetToolBox();
	// Get The manager
	IComponent* managerComponent = m_ToolBox->GetComponent( &CHashString( _T("CPhysicsManager")) );
	m_Manager = dynamic_cast<CPhysicsManager*>( managerComponent );

	m_Cloth = NULL;
	m_ClothMesh = NULL;
	m_Name = NULL;
	m_ClothDesc = &clothDesc;
}

CPhysicsCloth::CPhysicsCloth( IHashString* parentEntityName, IHashString* name, Matrix4x4 pose, NxClothDesc& clothDesc )
{
	m_Cloth = NULL;
	m_ClothMesh = NULL;
	m_Name = NULL;

	m_ToolBox = EngineGetToolBox();
	// Get the manager
	IComponent* managerComponent = m_ToolBox->GetComponent( &CHashString( _T("CPhysicsManager")) );
	m_Manager = dynamic_cast<CPhysicsManager*>( managerComponent );

	// Get the geometry for the object and convert it into a mesh descriptor
	GETRENDERBUFFERALLOCATIONBYNAMEMSG grbabn;
	grbabn.m_hszName = name;
	static DWORD msgHash_GetGeometryAllocationByName = CHashString(_T("GetGeometryAllocationByName")).GetUniqueID();
	m_ToolBox->SendMessage( msgHash_GetGeometryAllocationByName, sizeof(grbabn), &grbabn, parentEntityName );

    // make sure the geometry is valid...
	if( grbabn.m_GeometryAllocationsOut.size() == 0 )
	{
		EngineGetToolBox()->Log( LOGERROR, _T("No Geometry Allocations in CPhysicsCloth::CPhysicsCloth()!\n") );
		return;
	}
	RENDERBUFFERALLOCATION &renderAlloc = grbabn.m_GeometryAllocationsOut[0];

	// Build the cloth mesh desc from the render buffer allocations
	NxClothMeshDesc meshDesc;
	meshDesc.flags					= NX_MF_16_BIT_INDICES;
	meshDesc.numVertices			= renderAlloc.m_VertexAllocation.m_Size;
	meshDesc.numTriangles			= renderAlloc.m_IndexAllocation.m_Size / 3;
	meshDesc.pointStrideBytes		= renderAlloc.m_VertexAllocation.m_Stride;
	meshDesc.triangleStrideBytes	= renderAlloc.m_IndexAllocation.m_Stride * 3;
	
	//int size = meshDesc.numVertices * sizeof(Vec3);
	//geom->LockVertices( 0, size,  ); // does this work???
	void *pPointData = NULL;
	renderAlloc.m_VertexBufferInterface->Lock( renderAlloc.m_VertexAllocation.m_Offset,
									renderAlloc.m_VertexAllocation.m_Size,
									&pPointData );
	meshDesc.points = pPointData;

	//size = meshDesc.numTriangles * sizeof(unsigned int) * 3;
	//geom->LockIndices( 0, size, (void**)meshDesc.triangles );
	void *pTriangleData = NULL;
	renderAlloc.m_IndexBufferInterface->Lock(	renderAlloc.m_IndexAllocation.m_Offset,
												renderAlloc.m_IndexAllocation.m_Size,
												&pTriangleData );
	meshDesc.triangles = pTriangleData;

	// Cook the meshDesc
	CookMesh( meshDesc );

	renderAlloc.m_VertexBufferInterface->Unlock();
	renderAlloc.m_IndexBufferInterface->Unlock();

	// build receive buffers
	NxU32 maxVertices = meshDesc.numVertices;
	m_MeshData.verticesPosBegin = (NxVec3*)malloc(sizeof(NxVec3)*maxVertices);
	m_MeshData.verticesNormalBegin = (NxVec3*)malloc(sizeof(NxVec3)*maxVertices);
	m_MeshData.verticesPosByteStride = sizeof(NxVec3);
	m_MeshData.verticesNormalByteStride = sizeof(NxVec3);
	m_MeshData.maxVertices = maxVertices;
	m_MeshData.numVerticesPtr = (NxU32*)malloc(sizeof(NxU32));
	*m_MeshData.numVerticesPtr = 0;

	// convert Matrix4x4 to NxMat34
	NxMat34 clothPose;
	clothPose.setColumnMajor44( pose.GetMatrix() );
	//clothPose.setRowMajor44( pose.GetMatrix() );

	// Build the cloth actor.
	clothDesc.clothMesh = m_ClothMesh;
	clothDesc.meshData = m_MeshData;
	clothDesc.globalPose = clothPose;
	m_Cloth = m_Manager->GetScene()->createCloth( clothDesc );
	m_Cloth->setGroup( 1 );

	m_Name = new CHashString( name );
}

CPhysicsCloth::CPhysicsCloth( NxCloth* cloth )
{
	m_ToolBox = EngineGetToolBox();
	// Get The manager
	IComponent* managerComponent = m_ToolBox->GetComponent( &CHashString( _T("CPhysicsManager")) );
	m_Manager = dynamic_cast<CPhysicsManager*>( managerComponent );

	m_Cloth = cloth;
	m_ClothMesh = cloth->getClothMesh();

	// This is really roundabout but the only way I can see to get the data needed...
	NxClothMeshDesc meshDesc;
	m_ClothMesh->saveToDesc( meshDesc );

	// Set the user buffer cloth wrapper
	NxU32 maxVertices = meshDesc.numVertices;
	m_MeshData.verticesPosBegin = (NxVec3*)malloc(sizeof(NxVec3)*maxVertices);
	m_MeshData.verticesNormalBegin = (NxVec3*)malloc(sizeof(NxVec3)*maxVertices);
	m_MeshData.verticesPosByteStride = sizeof(NxVec3);
	m_MeshData.verticesNormalByteStride = sizeof(NxVec3);
	m_MeshData.maxVertices = maxVertices;
	m_MeshData.numVerticesPtr = (NxU32*)malloc(sizeof(NxU32));
	*m_MeshData.numVerticesPtr = 0;
	m_Cloth->setMeshData( m_MeshData );

	m_Name = new CHashString( cloth->getName() );
}

CPhysicsCloth::~CPhysicsCloth()
{
	if( m_Name != NULL )
	{
		delete m_Name;
		m_Name = NULL;
	}

	free( m_MeshData.verticesPosBegin );
	free( m_MeshData.verticesNormalBegin );
	free( m_MeshData.numVerticesPtr );
	m_MeshData.verticesPosBegin = NULL;
	m_MeshData.verticesNormalBegin = NULL;
	m_MeshData.numVerticesPtr = NULL;

	NxScene* scene = m_Manager->GetScene();
	NxPhysicsSDK* sdk = m_Manager->GetPhysicsSDK();
	if( m_Cloth != NULL )
	{
		scene->releaseCloth( *m_Cloth );
	}
	if( m_ClothMesh != NULL )
	{
		sdk->releaseClothMesh( *m_ClothMesh );
	}
}

IHashString* CPhysicsCloth::GetName()
{
	return m_Name;
}

void CPhysicsCloth::SetGeometry( IGeometry* geom )
{
	if( geom == NULL )
	{
		EngineGetToolBox()->Log( LOGERROR, _T("NULL IGeometry* in CPhysicsCloth::SetGeometry()!\n") );
		return;
	}

	// Grabs the user buffer wrapper for the cloth mesh data.
	NxMeshData meshData = m_Cloth->getMeshData();

	if( meshData.numVerticesPtr == NULL )
	{
		EngineGetToolBox()->Log( LOGERROR, _T("NULL NxMeshData in CPhysicsCloth::SetGeometry()!\n") );
		return;
	}

	BUFFERALLOCATIONSTRUCT vertexAllocation;
	geom->GetVertexBufferAllocation( vertexAllocation );

	int size = *meshData.numVerticesPtr;
	NxVec3 *pSrcData = (NxVec3*)meshData.verticesPosBegin;
	void *pDestData = NULL;
	if (geom->LockVertices( 0, size, &pDestData ))
	{
		for (int i=0; i < size; i++)
		{
			((Vec3*)pDestData)->Set( pSrcData[i].x, pSrcData[i].y, pSrcData[i].z );
			pDestData = (unsigned char*)pDestData + vertexAllocation.m_Stride;
		}
	}
	geom->UnlockVertices();
}

bool CPhysicsCloth::CookMesh( NxClothMeshDesc& desc )
{
	// Cook the mesh data
	bool cookingInitStatus = NxInitCooking();
	MemoryWriteStream buf;
	bool status = NxCookClothMesh(desc, buf);
	// make sure cooking worked
	if( status == false )
	{
		EngineGetToolBox()->Log( LOGERROR, _T("Failed to cook mesh data in CPhysicsCloth::CookMesh()!\n") );
		return false;
	}
	NxCloseCooking();

	// Read the cooked mesh data into m_ClothMesh
	NxPhysicsSDK* sdk = m_Manager->GetPhysicsSDK();
	MemoryReadStream rb( buf.data );
    m_ClothMesh = sdk->createClothMesh( rb );

	return true;
}

void CPhysicsCloth::ReleaseMeshDescBuffers( const NxClothMeshDesc& desc )
{
	Vec3* p = (Vec3*)desc.points;
	int* t = (int*)desc.triangles;
	free(p);
	free(t);
}

NxClothDesc* CPhysicsCloth::GetNxClothDesc()
{
	return this->m_ClothDesc;
}