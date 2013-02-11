///============================================================================
/// \file		NavMesh.cpp
/// \brief		Implmentation of CNavMeshObject Class
/// \date		09-07-2005
/// \author		Brian Bazyk
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
#define _USE_MATH_DEFINES
#include <math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/* NOTES:
Navigation Graph Generation:
1) Generate root nodes (root nodes are the topmost node on each grid position)
2) Generate subnodes and z values for all nodes (subnodes are nodes under root nodes)
3) Rebuild Node Grid in order to index root nodes during edge creation
5) Cat subnodes onto the node array
6) Generate all possible edges between all pairs of adjacent grid positions
7) Run edge and node trimming functions
8) Reindex the edge and node arrays
*/

/// Increment this whenever the file format changes for a .nav file.
static const int kNavFileVersion = 1;

//static const float kHeadRoom = 2.0f * 0.898f; // TODO: find a decent head room distance.  Is this 2 meters?
static const float kHeadRoom = 4.0f;
static const float kHeadRoomStep = 0.1f; // step size used to find z with enough headroom

REGISTER_COMPONENT(CNavMeshObject);
REGISTER_MESSAGE_HANDLER(GenerateNavMesh, OnGenerateNavMesh, CNavMeshObject);
REGISTER_MESSAGE_HANDLER(VisualizeNavMesh, OnVisualizeNavMesh, CNavMeshObject);

IComponent *CNavMeshObject::Create(int nArgs, va_list argptr)
{
	IHashString *name = va_arg(argptr, IHashString *);
	IHashString *parentName = va_arg(argptr, IHashString *);
	IObject *self = NULL;
	bool bAddToHier;
	try 
	{
		if (nArgs == 2)
		{
			self = new CNavMeshObject(parentName, name, true);
		}
		else if (nArgs == 3)
		{
			bAddToHier = va_arg(argptr, bool);
			self = new CNavMeshObject(parentName, name, bAddToHier);
		}
	}
	catch(...) 
	{
		return NULL;
	}
	return self;
}
/*
CNavMeshObject::CNavMeshObject(IHashString *parentName, IHashString *name) :
	m_hszNameType( _T("CNavMeshObject") ),
	m_hszNavMeshRenderObject( _T("NavMeshRenderObject") ),
	OBJECTTEMPLATE(, CNavMeshObject, IObject, parentName, name)
{
	m_ToolBox = EngineGetToolBox();

	AddToHierarchy();

	m_pNodes = NULL;
	m_nNodes = 0;
	m_pEdges = NULL;
	m_nEdges = 0;

	m_fSampleDistance = 0;

	m_iGridSizeX = m_iGridSizeY = 0;
	m_pNodeGrid = NULL;

	m_bNavMeshVis = false;
}

CNavMeshObject::~CNavMeshObject()
{
	RemoveFromHierarchy();
	DeInit();
}

void CNavMeshObject::Init()
{
	ReadNavMeshFile();

	RebuildNodeGrid();

//	bool bShow = false;
	static DWORD msgHash_GetVisualizeNavMeshMode = CHashString("GetVisualizeNavMeshMode").GetUniqueID();
//	m_ToolBox->SendMessage(msgHash_GetVisualizeNavMeshMode, sizeof(bool), &bShow );
//	OnVisualizeNavMesh( sizeof(bool), &bShow );
}

void CNavMeshObject::DeInit()
{
	if (m_pNodes)
	{
		delete[] m_pNodes;
		m_pNodes = NULL;
		m_nNodes = 0;
	}
	if (m_pEdges)
	{
		delete[] m_pEdges;
		m_pEdges = NULL;
		m_nEdges = 0;
	}

	m_SubNodes.clear();

	m_iGridSizeX = m_iGridSizeY = 0;
	if (m_pNodeGrid)
	{
		delete[] m_pNodeGrid;
		m_pNodeGrid = NULL;
	}

	bool bShow = false;
	OnVisualizeNavMesh( sizeof(bool), &bShow );
}

void CNavMeshObject::Serialize( IArchive &ar )
{
	if(ar.IsReading())
	{
		ar.Read( m_szNavFile, "File" );
	}
	else
	{
		ar.Write( m_szNavFile, "File" );
	}
}

IHashString* CNavMeshObject::GetComponentType( void )
{
	return &m_hszNameType;
}

bool CNavMeshObject::IsKindOf( IHashString* compType )
{
	return (compType->GetUniqueID() == m_hszNameType.GetUniqueID() );
}

DWORD CNavMeshObject::OnGenerateNavMesh( DWORD size, void *params )
{
	// if the current nav file doesn't exist, generate a unique name
	if (!NavFileExists(m_szNavFile))
		m_szNavFile = NewNavFile();

	GenerateNavMesh();
	RemoveEdgesBySlope( 30.0f );
	RemoveEdgesByMonofilament(); // should be done after slope
	Reindex();
	OutputDebugString( _T("NavMesh: Fin!\n") );

	if (!WriteNavMeshFile())
		return MSG_ERROR;

	Init();

	return MSG_HANDLED_STOP;
}

DWORD CNavMeshObject::OnGetNavMeshData( DWORD size, void *params )
{
	VERIFY_MESSAGE_SIZE(sizeof(NAVDATAPARAMS), size);
	NAVDATAPARAMS *pData = (NAVDATAPARAMS*)params;

	pData->pNodes = m_pNodes;
	pData->nNodes = m_nNodes;
	pData->pEdges = m_pEdges;
	pData->nEdges = m_nEdges;
	return MSG_HANDLED_STOP;
}

void CNavMeshObject::GenerateNavMesh()
{
	DeInit(); // destroy prior navmesh

	// get terrain dimensions
	TERRAINDESCRIPTOR terrainDesc;
	static DWORD msgHash_GetTerrainDescriptor = CHashString(_T("GetTerrainDescriptor")).GetUniqueID();
	if (m_ToolBox->SendMessage(msgHash_GetTerrainDescriptor, sizeof(terrainDesc), &terrainDesc ) != MSG_HANDLED)
		return;

	// find a decent sampling rate
	m_fSampleDistance = terrainDesc.SectorSize / (float)terrainDesc.SectorResolution;
	// find node grid size
	m_iGridSizeX = (int)((float)terrainDesc.TerrainWidth * terrainDesc.SectorSize / m_fSampleDistance) + 1;
	m_iGridSizeY = (int)((float)terrainDesc.TerrainHeight * terrainDesc.SectorSize / m_fSampleDistance) + 1;
	// allocate nodes
	m_nNodes = m_iGridSizeX * m_iGridSizeY;
	if (m_nNodes == 0)
		return;
	m_pNodes = new NAVNODE[m_nNodes];
	if (!m_pNodes)
		return;
	// Initialize nodes and calculate X,Y positions
	OutputDebugString( _T("NavMesh: Init Node + X,Y Positions\n") );
	int iNode = 0;
	for (int j=0; j < m_iGridSizeY; j++)
	{
		for (int i=0; i < m_iGridSizeX; i++)
		{
			m_pNodes[iNode].m_Position.x = (float)i * m_fSampleDistance;
			m_pNodes[iNode].m_Position.y = (float)j * m_fSampleDistance;
			m_pNodes[iNode].m_Position.z = 0.0f;
			m_pNodes[iNode].m_vEdges[0] = -1;
			m_pNodes[iNode].m_vEdges[1] = -1;
			m_pNodes[iNode].m_vEdges[2] = -1;
			m_pNodes[iNode].m_vEdges[3] = -1;
			m_pNodes[iNode].m_iSubNode = -1;
			m_pNodes[iNode].m_iGridX = i;
			m_pNodes[iNode].m_iGridY = j;

			// Adjust edge node positions so raycasts dont graze the edge of the map
			if (i == 0)
				m_pNodes[0].m_Position.x += 0.001f;
			else if (i == m_iGridSizeX-1)
				m_pNodes[0].m_Position.x -= 0.001f;
			if (j == 0)
				m_pNodes[0].m_Position.y += 0.001f;
			else if (j == m_iGridSizeY-1)
				m_pNodes[0].m_Position.y -= 0.001f;

			iNode++;
		}
	}

	// Build Node Grid for use in edge creation
	RebuildNodeGrid();

	// Calculate Z node positions and generate subnodes
//	GenerateNavMeshZ_Raindrop();
//	GenerateNavMeshZ_Groundup();
	GenerateNavMeshZ_Bouncy();

	// Cat subnodes onto node array
	if (m_SubNodes.size() > 0)
	{
		NAVNODE *pNewNodes = new NAVNODE[m_nNodes + m_SubNodes.size()];
		memcpy( pNewNodes, m_pNodes, sizeof(NAVNODE)*m_nNodes );
		for (unsigned int i=0; i < m_SubNodes.size(); i++)
			pNewNodes[m_nNodes+i] = m_SubNodes[i];

		delete[] m_pNodes;
		m_pNodes = pNewNodes;
		m_nNodes = m_nNodes + m_SubNodes.size();
	}

	// SubNodes no longer in use
	m_SubNodes.clear();

	// Generate Edges
	GenerateEdges();

	// calculate distances along edges
	OutputDebugString( _T("NavMesh: Edge Distances\n") );
	for (int e=0; e < m_nEdges; e++)
	{
		NAVNODE *pNodeA = &m_pNodes[m_pEdges[e].m_vNodes[0]];
		NAVNODE *pNodeB = &m_pNodes[m_pEdges[e].m_vNodes[1]];
		m_pEdges[e].m_fDistance = (pNodeA->m_Position - pNodeB->m_Position).Length();
	}
}

void CNavMeshObject::GenerateNavMeshZ_Raindrop()
{
	OutputDebugString( _T("NavMesh: Z Positions : Raindrop\n") );

	Vec3 rayPos( 0, 0, 9999 );
	Vec3 rayDir( 0, 0, -1 );
	//std::list<RAYCOLLISION> rayHitList;
	//std::list<RAYCOLLISION>::iterator itrHitList;
	RAYCOLLISION rayHitList;
	RAYCASTPARAMS ray;
	ray.rayPos = &rayPos;
	ray.rayDir = &rayDir;
	ray.listRayCollisions = &rayHitList;
	ray.group = OBJECTS;
	for (int iNode=0; iNode < m_nNodes; iNode++)
	{
		ray.rayPos->x = m_pNodes[iNode].m_Position.x;
		ray.rayPos->y = m_pNodes[iNode].m_Position.y;
		ray.rayPos->z = 9999;                       // start in sky
		ray.group = OBJECTS;   

		//rayHitList.clear();
		static DWORD msgHash_Raycast = CHashString(_T("Raycast")).GetUniqueID();
		if (m_ToolBox->SendMessage(msgHash_Raycast, sizeof(ray), &ray ) == MSG_HANDLED)
		{
			// if no objects hit, try terrain
			//if (rayHitList.size() == 0)
			if (rayHitList.object == NULL)
			{
				ray.group = TERRAIN;
				static DWORD msgHash_Raycast = CHashString(_T("Raycast")).GetUniqueID();
				m_ToolBox->SendMessage(msgHash_Raycast, sizeof(ray), &ray );
			}
		}
		//if (rayHitList.size() > 0)
		if (rayHitList.object != NULL )
		{
			//RAYCOLLISION *pCollision = &*(rayHitList.begin());
			RAYCOLLISION* pCollision = &rayHitList;
			m_pNodes[iNode].m_Position.z = pCollision->hitPoint[2]; // set root node's z
		}

		// find subnodes
		ray.rayPos->SetZ( m_pNodes[iNode].m_Position.z ); // start at root node
		do
		{
			// if we already hit the terrain then we are done with this node
			if (ray.group == TERRAIN)
				break;

			//rayHitList.clear();
			static DWORD msgHash_Raycast = CHashString(_T("Raycast")).GetUniqueID();
			if (m_ToolBox->SendMessage(msgHash_Raycast, sizeof(ray), &ray ) == MSG_HANDLED)
			{
				// if no objects hit, try terrain
				//if (rayHitList.size() == 0)
				if (rayHitList.object == NULL )
				{
					ray.group = TERRAIN;
					static DWORD msgHash_Raycast = CHashString(_T("Raycast")).GetUniqueID();
					m_ToolBox->SendMessage(msgHash_Raycast, sizeof(ray), &ray );
				}
			}
			//if (rayHitList.size() > 0)
			if (rayHitList.object != NULL)
			{
				//RAYCOLLISION *pCollision = &*(rayHitList.begin());
				RAYCOLLISION* pCollision = &rayHitList;
				if (pCollision->distance <= kHeadRoom)
				{
					ray.rayPos->SetZ( ray.rayPos->z - kHeadRoomStep );
				}
				else
				{
					// add this point as a sub node
					AddSubNode( iNode, ray.rayPos->z );

					// set z at next collide point and continue from there to find 
					// another subnode
					ray.rayPos->SetZ( ray.rayPos->z - pCollision->distance - 0.01f );
				}
				continue;
			}
			else
			{
				// nothing below us
				break;
			}
		} while( 1 );
	}
}

void CNavMeshObject::GenerateNavMeshZ_Groundup()
{
	OutputDebugString( _T("NavMesh: Z Positions : Groundup\n") );

	//unsigned int terrainMask = 0;
	//ADDCOLLISIONGROUP( terrainMask, TERRAINCOLLISIONGROUP );
	//unsigned int objectMask = 0;
	//ADDCOLLISIONGROUP( objectMask, OBJECTCOLLISIONGROUP );

	Vec3 rayPos( 0, 0, 9999 );
	Vec3 rayDir( 0, 0, -1 );
	//std::list<RAYCOLLISION> rayHitList;
	//std::list<RAYCOLLISION>::iterator itrHitList;
	RAYCOLLISION rayHitList;
	RAYCASTPARAMS ray;
	ray.rayPos = &rayPos;
	ray.rayDir = &rayDir;
	ray.listRayCollisions = &rayHitList;
	ray.group = TERRAIN;
	for (int iNode=0; iNode < m_nNodes; iNode++)
	{
		ray.rayPos->x = m_pNodes[iNode].m_Position.x;
		ray.rayPos->y = m_pNodes[iNode].m_Position.y;

		// find hit on ground
		ray.rayPos->z = 9999;         // start in the sky
		ray.rayDir->z = -1;           // aim down
		ray.group = TERRAIN;			  // hit only terrain
		//rayHitList.clear();
		static DWORD msgHash_Raycast = CHashString(_T("Raycast")).GetUniqueID();
		if (m_ToolBox->SendMessage(msgHash_Raycast, sizeof(ray), &ray ) == MSG_HANDLED)
		{
			//if (rayHitList.size() > 0)
			if (rayHitList.object != NULL)
			{
				//itrHitList = rayHitList.begin();
				//m_pNodes[iNode].m_Position.z = itrHitList->hitPoint[2];
				m_pNodes[iNode].m_Position.z = rayHitList.hitPoint[2];
			}
		}
		//rayHitList.clear(); // make sure hit list is clear

		// now trace upwards with an object hit mask on and increment z position 
		// until a trace with a certain head space clearance is returned.
		ray.rayPos->SetZ( m_pNodes[iNode].m_Position.z ); // start at terrain height
		ray.rayDir->SetZ( 1 );                            // aim up
		ray.group = OBJECTS;							  // hit only objects
		do
		{
			//rayHitList.clear();
			static DWORD msgHash_Raycast = CHashString(_T("Raycast")).GetUniqueID();
			if (m_ToolBox->SendMessage(msgHash_Raycast, sizeof(ray), &ray ) == MSG_HANDLED)
			{
				//if (rayHitList.size() > 0)
				if (rayHitList.object != NULL)
				{
					//RAYCOLLISION *pCollision = &*(rayHitList.begin());
					RAYCOLLISION *pCollision = &rayHitList;
					if (pCollision->distance <= kHeadRoom)
					{
						ray.rayPos->SetZ( ray.rayPos->z + kHeadRoomStep );
						continue;
					}
					else
					{
						// add this as a sub node because there is still stuff above us
						AddSubNode( iNode, ray.rayPos->z );

						// set z at next collide point and continue from there to find 
						// either another fudge node or the end position for this node
						ray.rayPos->SetZ( ray.rayPos->z + pCollision->distance + 0.01f );
						continue;
					}
				}
			}
			// nothing above us
			m_pNodes[iNode].m_Position.z = ray.rayPos->z; // update end node
			break;
		} while( 1 );
	}
}

void CNavMeshObject::GenerateNavMeshZ_Bouncy()
{
	OutputDebugString( _T("NavMesh: Z Positions : Bouncy\n") );

	//unsigned int terrainMask = 0;
	//ADDCOLLISIONGROUP( terrainMask, TERRAINCOLLISIONGROUP );
	//unsigned int objectMask = 0;
	//ADDCOLLISIONGROUP( objectMask, OBJECTCOLLISIONGROUP );
	//unsigned int allMask = 0;
	//ADDCOLLISIONGROUP( allMask, TERRAINCOLLISIONGROUP );
	//ADDCOLLISIONGROUP( allMask, OBJECTCOLLISIONGROUP );

	Vec3 rayPos( 0, 0, 9999 );
	Vec3 rayDir( 0, 0, -1 );
	//std::list<RAYCOLLISION> rayHitList;
	//std::list<RAYCOLLISION>::iterator itrHitList;
	RAYCOLLISION rayHitList;
	RAYCASTPARAMS ray;
	ray.rayPos = &rayPos;
	ray.rayDir = &rayDir;
	ray.listRayCollisions = &rayHitList;
	ray.group = TERRAIN;
	for (int iNode=0; iNode < m_nNodes; iNode++)
	{
		ray.rayPos->x = m_pNodes[iNode].m_Position.x );
		ray.rayPos->y = m_pNodes[iNode].m_Position.y );

		// find hit on ground
		ray.rayPos->z = 9999 );         // start in the sky
		ray.rayDir->z = -1 );           // aim down
		ray.group = TERRAIN;			  // hit only terrain
		//rayHitList.clear();
		static DWORD msgHash_Raycast = CHashString(_T("Raycast")).GetUniqueID();
		if (m_ToolBox->SendMessage(msgHash_Raycast, sizeof(ray), &ray ) == MSG_HANDLED)
		{
		}
		//if (rayHitList.size() > 0)
		if (rayHitList.object != NULL)
		{
			//itrHitList = rayHitList.begin();
			//m_pNodes[iNode].m_Position.z = itrHitList->hitPoint[2];
			m_pNodes[iNode].m_Position.z = rayHitList.hitPoint[2];
		}

		// now trace upwards with an object hit mask on
		ray.rayPos->SetZ( m_pNodes[iNode].m_Position.z ); // start at terrain height
		ray.rayDir->SetZ( 1 );                            // aim up       
		ray.group = OBJECTS;							  // hit only objects
		do
		{
			// trace up to find objects to hit
			ray.rayDir->SetZ( 1 );						  // aim up
			ray.group = OBJECTS;						  // hit only objects
			//rayHitList.clear();
			static DWORD msgHash_Raycast = CHashString(_T("Raycast")).GetUniqueID();
			m_ToolBox->SendMessage(msgHash_Raycast, sizeof(ray), &ray );
			//if (rayHitList.size() > 0)
			if (rayHitList.object != NULL)
			{
				// hit an object so now trace down from the bottom of the object to hit the top of 
				// the object we just came from
				//RAYCOLLISION *pCollision = &*(rayHitList.begin());
				RAYCOLLISION *pCollision = &rayHitList;
				float fBottomNextObject = pCollision->hitPoint[2];
				float fTopCurrentObject = fBottomNextObject;

				ray.rayPos->SetZ( fBottomNextObject - 0.01f ); // start at bottom of next object
				ray.rayDir->SetZ( -1 );                        // aim down
				ray.group = ALL;							  // hit everything
				//rayHitList.clear();
				static DWORD msgHash_Raycast = CHashString(_T("Raycast")).GetUniqueID();
				m_ToolBox->SendMessage(msgHash_Raycast, sizeof(ray), &ray );
				//if (rayHitList.size() > 0)
				if (rayHitList.object != NULL)
				{
					//pCollision = &*(rayHitList.begin());
					pCollision = &rayHitList;
					fTopCurrentObject = pCollision->hitPoint[2];
				}

				if (fBottomNextObject-fTopCurrentObject > kHeadRoom)
				{
					// add this as a sub node because there is still stuff above us
					AddSubNode( iNode, fTopCurrentObject );
				}
				// start inside next object
				ray.rayPos->SetZ( fBottomNextObject + 0.01f );
				continue;
			}
			else
			{
				// nothing left to hit
				// aim down from the sky and get our root node position
				ray.rayPos->SetZ( 9999 );     // start in sky
				ray.rayDir->SetZ( -1 );       // aim down
				ray.group = ALL;			  // hit everything
				//rayHitList.clear();
				static DWORD msgHash_Raycast = CHashString(_T("Raycast")).GetUniqueID();
				m_ToolBox->SendMessage(msgHash_Raycast, sizeof(ray), &ray );
				//if (rayHitList.size() > 0)
				if (rayHitList.object != NULL)
				{
					// update root node
					//RAYCOLLISION *pCollision = &*(rayHitList.begin());
					RAYCOLLISION *pCollision = &rayHitList;
					m_pNodes[iNode].m_Position.z = pCollision->hitPoint[2];
				}
				break;
			}
		} while( 1 );
	}
}

bool CNavMeshObject::WriteNavMeshFile()
{
	OutputDebugString( _T("NavMesh: Writing file\n") );

	// Open file archive to write nav file
	CHashString streamType(_T("File"));
	CREATEARCHIVE ca;
	ca.mode = STREAM_MODE_WRITE | STREAM_MODE_BINARY;
	ca.streamData = const_cast<TCHAR*>((const TCHAR*)m_szNavFile);
	ca.streamType = &streamType;
	static DWORD msgHash_CreateArchive = CHashString(_T("CreateArchive")).GetUniqueID();
	if (m_ToolBox->SendMessage(msgHash_CreateArchive, sizeof(CREATEARCHIVE), &ca) != MSG_HANDLED)
	{
		return false;
	}

	IArchive *FileArchive = dynamic_cast<IArchive *>(ca.archive);

	// Write Version
	FileArchive->Write( kNavFileVersion );

	// Write Grid Size
	FileArchive->Write( m_iGridSizeX );
	FileArchive->Write( m_iGridSizeY );

	// Write Nodes
	FileArchive->Write( m_nNodes );
	for (int i=0; i < m_nNodes; i++)
	{
		FileArchive->Write( m_pNodes[i].m_Position.x );
		FileArchive->Write( m_pNodes[i].m_Position.y );
		FileArchive->Write( m_pNodes[i].m_Position.z );
		FileArchive->Write( m_pNodes[i].m_vEdges[0] );
		FileArchive->Write( m_pNodes[i].m_vEdges[1] );
		FileArchive->Write( m_pNodes[i].m_vEdges[2] );
		FileArchive->Write( m_pNodes[i].m_vEdges[3] );
		FileArchive->Write( m_pNodes[i].m_iSubNode );
		FileArchive->Write( m_pNodes[i].m_iGridX );
		FileArchive->Write( m_pNodes[i].m_iGridY );
	}

	// Write Edges
	FileArchive->Write( m_nEdges );
	for (int i=0; i < m_nEdges; i++)
	{
		FileArchive->Write( m_pEdges[i].m_vNodes[0] );
		FileArchive->Write( m_pEdges[i].m_vNodes[1] );
		FileArchive->Write( m_pEdges[i].m_fDistance );
	}

	FileArchive->Close();
	return true;
}

bool CNavMeshObject::ReadNavMeshFile()
{
	DeInit(); // destroy prior navmesh

	// Open file archive to read nav file
	CHashString streamType(_T("File"));
	CREATEARCHIVE ca;
	ca.mode = STREAM_MODE_READ | STREAM_MODE_BINARY;
	ca.streamData = const_cast<TCHAR*>((const TCHAR*)m_szNavFile);
	ca.streamType = &streamType;
	static DWORD msgHash_CreateArchive = CHashString(_T("CreateArchive")).GetUniqueID();
	if (m_ToolBox->SendMessage(msgHash_CreateArchive, sizeof(CREATEARCHIVE), &ca) != MSG_HANDLED)
	{
		return false;
	}

	IArchive *FileArchive = dynamic_cast<IArchive *>(ca.archive);

	// Read Version
	int iVersion;
	FileArchive->Read( iVersion );

	// Check Version
	if (iVersion != kNavFileVersion)
	{
		OutputDebugString( _T("NavMesh: Invalid NavFile version.\n") );
		// TODO: Ask Ken about popping up some sort of dialog or notification to the
		// user that this map has an old nav file associated with it and it needs to be
		// recreated.
		return false;
	}

	// Read Grid Size
	FileArchive->Read( m_iGridSizeX );
	FileArchive->Read( m_iGridSizeY );

	// Read Nodes
	FileArchive->Read( m_nNodes );
	if (m_nNodes > 0)
		m_pNodes = new NAVNODE[m_nNodes];
	for (int i=0; i < m_nNodes; i++)
	{
		FileArchive->Read( m_pNodes[i].m_Position.x );
		FileArchive->Read( m_pNodes[i].m_Position.y );
		FileArchive->Read( m_pNodes[i].m_Position.z );
		FileArchive->Read( m_pNodes[i].m_vEdges[0] );
		FileArchive->Read( m_pNodes[i].m_vEdges[1] );
		FileArchive->Read( m_pNodes[i].m_vEdges[2] );
		FileArchive->Read( m_pNodes[i].m_vEdges[3] );
		FileArchive->Read( m_pNodes[i].m_iSubNode );
		FileArchive->Read( m_pNodes[i].m_iGridX );
		FileArchive->Read( m_pNodes[i].m_iGridY );
	}

	// Read Edges
	FileArchive->Read( m_nEdges );
	if (m_nEdges > 0)
		m_pEdges = new NAVEDGE[m_nEdges];
	for (int i=0; i < m_nEdges; i++)
	{
		FileArchive->Read( m_pEdges[i].m_vNodes[0] );
		FileArchive->Read( m_pEdges[i].m_vNodes[1] );
		FileArchive->Read( m_pEdges[i].m_fDistance );
	}

	FileArchive->Close();
	return true;
}

bool CNavMeshObject::NavFileExists( const TCHAR *szFile )
{
	struct _stat buf;
	return (_tstat( szFile, &buf ) == 0);
}

const TCHAR* CNavMeshObject::NewNavFile()
{
	int i = 0;
	static TCHAR szFile[MAX_PATH];

//	m_ToolBox->GetDirectories( szBaseDir, szResourceDir );

	do
	{
		_stprintf( szFile, _T("NavMeshes/%i.nav"), i++ );
	} while (NavFileExists(szFile));

	return szFile;
}

void CNavMeshObject::RemoveEdge( int iEdge )
{
	if (iEdge == -1)
		return;

	assert( iEdge < m_nEdges );

	// remove edge index from edge's nodes
	for (int n=0; n < 2; n++)
	{
		NAVNODE *pNode = &m_pNodes[m_pEdges[iEdge].m_vNodes[n]];

		for (int e=0; e < 4; e++)
		{
			if (pNode->m_vEdges[e] == iEdge)
				pNode->m_vEdges[e] = -1;
		}
	}

	// clear edge
	m_pEdges[iEdge].m_vNodes[0] = -1;
	m_pEdges[iEdge].m_vNodes[1] = -1;
}

void CNavMeshObject::Reindex()
{
	OutputDebugString( _T("NavMesh: Reindexing\n") );

	if (m_nNodes == 0)
		return;

	// create old to new node index map
	int *pNodeIndexMap = new int[m_nNodes];
	int *pSubNodeIndexMap = new int[m_nNodes];
	int iNewNodeCount = 0;
	for (int i=0; i < m_nNodes; i++)
	{
		// only keep nodes with at least one active edge
		if (m_pNodes[i].m_vEdges[0] != -1 || 
			m_pNodes[i].m_vEdges[1] != -1 || 
			m_pNodes[i].m_vEdges[2] != -1 || 
			m_pNodes[i].m_vEdges[3] != -1)
		{
			pNodeIndexMap[i] = iNewNodeCount++;
			// if a node's subnode index refers to i, keep it mapped to i
			pSubNodeIndexMap[i] = i; // ok to use old index, will be remapped later
		}
		else
		{
			pNodeIndexMap[i] = -1;
			// if a node's subnode index refers to i, map it to the node beneath i (if any, can be -1)
			pSubNodeIndexMap[i] = m_pNodes[i].m_iSubNode; // ok to use old index, will be remapped later
		}
	}
	// remap subnode index map to map to new node indices
	for (int i=0; i < m_nNodes; i++)
	{
		if (pSubNodeIndexMap[i] != -1)
			pSubNodeIndexMap[i] = pNodeIndexMap[pSubNodeIndexMap[i]];
	}
	// copy to a new shorter node list
	NAVNODE *pNewNodes = new NAVNODE[iNewNodeCount];
	for (int i=0; i < m_nNodes; i++)
	{
		if (pNodeIndexMap[i] != -1)
			pNewNodes[pNodeIndexMap[i]] = m_pNodes[i];
	}
	// delete old nodes
	delete[] m_pNodes;
	m_pNodes = pNewNodes;
	m_nNodes = iNewNodeCount;
	// remap node indices stored in the edges
	for (int i=0; i < m_nEdges; i++)
	{
		m_pEdges[i].m_vNodes[0] = (m_pEdges[i].m_vNodes[0] == -1) ? -1 : pNodeIndexMap[m_pEdges[i].m_vNodes[0]];
		m_pEdges[i].m_vNodes[1] = (m_pEdges[i].m_vNodes[1] == -1) ? -1 : pNodeIndexMap[m_pEdges[i].m_vNodes[1]];
	}

	// create old to new edge index map
	int *pEdgeIndexMap = new int[m_nEdges];
	int iNewEdgeCount = 0;
	for (int i=0; i < m_nEdges; i++)
	{
		// only keep edges connected to active nodes
		if (m_pEdges[i].m_vNodes[0] != -1 && 
			m_pEdges[i].m_vNodes[1] != -1)
		{
			pEdgeIndexMap[i] = iNewEdgeCount++;
		}
		else
			pEdgeIndexMap[i] = -1;
	}
	// copy to a new shorter edge list
	NAVEDGE *pNewEdges = new NAVEDGE[iNewEdgeCount];
	for (int i=0; i < m_nEdges; i++)
	{
		if (pEdgeIndexMap[i] != -1)
			pNewEdges[pEdgeIndexMap[i]] = m_pEdges[i];
	}
	// delete old edges
	delete[] m_pEdges;
	m_pEdges = pNewEdges;
	m_nEdges = iNewEdgeCount;
	// remap indices stored in the nodes
	for (int i=0; i < m_nNodes; i++)
	{
		m_pNodes[i].m_vEdges[0] = (m_pNodes[i].m_vEdges[0] == -1) ? -1 : pEdgeIndexMap[m_pNodes[i].m_vEdges[0]];
		m_pNodes[i].m_vEdges[1] = (m_pNodes[i].m_vEdges[1] == -1) ? -1 : pEdgeIndexMap[m_pNodes[i].m_vEdges[1]];
		m_pNodes[i].m_vEdges[2] = (m_pNodes[i].m_vEdges[2] == -1) ? -1 : pEdgeIndexMap[m_pNodes[i].m_vEdges[2]];
		m_pNodes[i].m_vEdges[3] = (m_pNodes[i].m_vEdges[3] == -1) ? -1 : pEdgeIndexMap[m_pNodes[i].m_vEdges[3]];

		m_pNodes[i].m_iSubNode = (m_pNodes[i].m_iSubNode == -1) ? -1 : pSubNodeIndexMap[m_pNodes[i].m_iSubNode];
	}

	delete[] pNodeIndexMap;
	delete[] pSubNodeIndexMap;
	delete[] pEdgeIndexMap;

#ifdef _DEBUG
	Validate();
#endif

	// keep node grid updated
	RebuildNodeGrid();
}

void CNavMeshObject::RemoveEdgesBySlope( float fSlope )
{
	OutputDebugString( _T("NavMesh: Remove Edges by Slope\n") );

	assert( fSlope >= 0.0f && fSlope <= 90.0f );

	// find max distance between two nodes using a slope angle
	float fMaxDistance = m_fSampleDistance / cosf( (fSlope / 180.0f) * (float)M_PI );

	for (int e=0; e < m_nEdges; e++)
	{
		if (m_pEdges[e].m_fDistance >= fMaxDistance)
			RemoveEdge( e );
	}
}

void CNavMeshObject::RemoveEdgesByMonofilament()
{
	OutputDebugString( _T("NavMesh: Remove Edges by Monofilament\n") );

	for (int n=0; n < m_nNodes; n++)
	{
		if (m_pNodes[n].m_vEdges[NAV_EDGE_DOWN] == -1 && 
			m_pNodes[n].m_vEdges[NAV_EDGE_UP] == -1)
		{
			RemoveEdge( m_pNodes[n].m_vEdges[NAV_EDGE_LEFT] );
			RemoveEdge( m_pNodes[n].m_vEdges[NAV_EDGE_RIGHT] );
		}

		if (m_pNodes[n].m_vEdges[NAV_EDGE_LEFT] == -1 && 
			m_pNodes[n].m_vEdges[NAV_EDGE_RIGHT] == -1)
		{
			RemoveEdge( m_pNodes[n].m_vEdges[NAV_EDGE_DOWN] );
			RemoveEdge( m_pNodes[n].m_vEdges[NAV_EDGE_UP] );
		}
	}
}

void CNavMeshObject::AddSubNode( int iNode, float z )
{
	NAVNODE NewNode;
	NewNode.m_Position = m_pNodes[iNode].m_Position;
	NewNode.m_Position.z = z;
	for (int i=0; i < 4; i++)
		NewNode.m_vEdges[i] = -1;
	NewNode.m_iSubNode = -1;
	NewNode.m_iGridX = m_pNodes[iNode].m_iGridX;
	NewNode.m_iGridY = m_pNodes[iNode].m_iGridY;

	int iNewNode = m_SubNodes.size() + m_iGridSizeX*m_iGridSizeY;
	m_SubNodes.push_back( NewNode );

	// add new node to list of subnodes under iNode
	NAVNODE *pParentNode = &m_pNodes[iNode];
	while (pParentNode->m_iSubNode != -1)
	{
		pParentNode = &m_SubNodes[pParentNode->m_iSubNode - m_iGridSizeX*m_iGridSizeY];
	}
	pParentNode->m_iSubNode = iNewNode;
}

void CNavMeshObject::GenerateEdges()
{
	OutputDebugString( _T("NavMesh: Generate Edges\n") );

	std::vector<NAVEDGE> vecEdges;

	for (int j=0; j < m_iGridSizeY; j++)
	{
		for (int i=0; i < m_iGridSizeX-1; i++)
		{
			int iGridNodeA = j * m_iGridSizeX + i;
			int iGridNodeB = iGridNodeA + 1;
			GenerateEdges( iGridNodeA, iGridNodeB, vecEdges, NAV_EDGE_RIGHT );
		}
	}
	for (int j=0; j < m_iGridSizeY-1; j++)
	{
		for (int i=0; i < m_iGridSizeX; i++)
		{
			int iGridNodeA = j * m_iGridSizeX + i;
			int iGridNodeB = iGridNodeA + m_iGridSizeX;
			GenerateEdges( iGridNodeA, iGridNodeB, vecEdges, NAV_EDGE_DOWN );
		}
	}

	m_pEdges = new NAVEDGE[vecEdges.size()];
	if (!m_pEdges)
		return;
	m_nEdges = vecEdges.size();
	for (int e=0; e < m_nEdges; e++)
		m_pEdges[e] = vecEdges[e];

	vecEdges.clear();
}

void CNavMeshObject::GenerateEdges( int iGridNodeA, int iGridNodeB, std::vector<NAVEDGE> &vecEdges, int iEdgeAB )
{
	// generate edges until no more can be generated between these two grid node locations
	bool bDone = false;
	while (!bDone)
	{
		// generate an edge for the best fit pair of nodes on the given grid node locations
		int iBestA = -1, iBestB = -1;
		float fBestPairDistance = 999999.0f;

		// for each node on grid location A
		for (int iSubNodeA = iGridNodeA; iSubNodeA != -1; iSubNodeA = m_pNodes[iSubNodeA].m_iSubNode)
		{
			NAVNODE *pNodeA = &m_pNodes[iSubNodeA];
			// ignore subnode if it's edge is filled
			if (pNodeA->m_vEdges[iEdgeAB] != -1)
				continue;

			// for each node on grid location B
			for (int iSubNodeB = iGridNodeB; iSubNodeB != -1; iSubNodeB = m_pNodes[iSubNodeB].m_iSubNode)
			{
				NAVNODE *pNodeB = &m_pNodes[iSubNodeB];
				// ignore subnode if it's edge is filled
				if (pNodeB->m_vEdges[OPPOSITE_EDGE(iEdgeAB)] != -1)
					continue;

				// see if these two nodes make the best fit pair
				float fDistance = (pNodeA->m_Position - pNodeB->m_Position).Length();
				if (fDistance < fBestPairDistance)
				{
					iBestA = iSubNodeA;
					iBestB = iSubNodeB;
					fBestPairDistance = fDistance;
				}
			}
		}

		// generate an edge between the best fit pair
		if (iBestA != -1 && iBestB != -1)
		{
			NAVEDGE NewEdge;
			NewEdge.m_vNodes[0] = iBestA;
			NewEdge.m_vNodes[1] = iBestB;
			int iNewEdge = vecEdges.size();
			vecEdges.push_back( NewEdge );

			// set the edge indices in the nodes
			m_pNodes[iBestA].m_vEdges[iEdgeAB] = iNewEdge;
			m_pNodes[iBestB].m_vEdges[OPPOSITE_EDGE(iEdgeAB)] = iNewEdge;
		}
		else
			bDone = true;
	}
}
#if 0
	// for each node
	for (int iNode=0; iNode < m_nNodes; iNode++)
	{
		NAVNODE *pNode = &m_pNodes[iNode];

		// for each edge
		for (int e=0; e < 4; e++)
		{
			// ignore already connected edge slots
			if (pNode->m_vEdges[e] != -1)
				continue;

			// find adjacent root node
			int iAdjNode = AdjacentNode( pNode->m_iGridX, pNode->m_iGridY, e );
			if (iAdjNode == -1)
				continue;

			// for each node in this grid slot (root node plus all subnodes)
			int iClosestNode = -1;
			while (iAdjNode != -1)
			{
				// find the closest node that isn't already connected along this edge
				if (m_pNodes[iAdjNode].m_vEdges[OPPOSITE_EDGE(e)] == -1)
				{
					if (iClosestNode != -1)
					{
						float fDistanceToAdj = (m_pNodes[iAdjNode].m_Position - pNode->m_Position).Length();
						float fDistanceToClosest = (m_pNodes[iClosestNode].m_Position - pNode->m_Position).Length();
						if (fDistanceToAdj < fDistanceToClosest)
							iClosestNode = iAdjNode;
					}
					else
						iClosestNode = iAdjNode;
				}

				// go to next subnode
				iAdjNode = m_pNodes[iAdjNode].m_iSubNode;
			}

			// if a valid adjacent node was found, create a new edge to it
			if (iClosestNode != -1)
			{
				NAVEDGE NewEdge;
				NewEdge.m_vNodes[0] = iNode;
				NewEdge.m_vNodes[1] = iClosestNode;
				int iNewEdge = vecEdges.size();
				vecEdges.push_back( NewEdge );

				// set the edge indices in the node and the closest node
				pNode->m_vEdges[e] = iNewEdge;
				m_pNodes[iClosestNode].m_vEdges[OPPOSITE_EDGE(e)] = iNewEdge;
			}
		}
	}
}
#endif

void CNavMeshObject::RebuildNodeGrid()
{
	if (m_iGridSizeX == 0 || m_iGridSizeY == 0)
		return;

	// allocate node grid if we don't have one yet
	int nGridNodes = m_iGridSizeX*m_iGridSizeY;

	if (m_pNodeGrid == NULL)
		m_pNodeGrid = new NAVNODE*[nGridNodes];
	if (!m_pNodeGrid)
		return;

	memset( m_pNodeGrid, 0, sizeof(m_pNodeGrid[0]) * nGridNodes );

	for (int i=0; i < m_nNodes; i++)
	{
		int iGridNode = m_pNodes[i].m_iGridX + m_pNodes[i].m_iGridY * m_iGridSizeX;
		// set grid node ptr to the first node in that grid slot, ignore the rest because they are subnodes
		if (m_pNodeGrid[iGridNode] == NULL)
			m_pNodeGrid[iGridNode] = &m_pNodes[i];
	}
}

inline int CNavMeshObject::AdjacentNode( int iGridX, int iGridY, int iEdge )
{
	assert( m_pNodeGrid != NULL ); // node grid must be initialized

	if (iEdge == NAV_EDGE_UP)
		iGridY--;
	else if (iEdge == NAV_EDGE_DOWN)
		iGridY++;
	else if (iEdge == NAV_EDGE_LEFT)
		iGridX--;
	else if (iEdge == NAV_EDGE_RIGHT)
		iGridX++;
	else
	{
		assert( false ); // bad edge direction given
		return -1;
	}

	if (iGridX < 0 || iGridX >= m_iGridSizeX)
		return -1;
	if (iGridY < 0 || iGridY >= m_iGridSizeY)
		return -1;

	return iGridX + iGridY * m_iGridSizeX;
}

void CNavMeshObject::Validate()
{
	for (int i=0; i < m_nNodes; i++)
	{
		NAVNODE *pNode = &m_pNodes[i];

		// verify that all subnodes are in the same grid square
		int iSubNode = pNode->m_iSubNode;
		while (iSubNode != -1)
		{
			assert( m_pNodes[iSubNode].m_iGridX == pNode->m_iGridX );
			assert( m_pNodes[iSubNode].m_iGridY == pNode->m_iGridY );
			iSubNode = m_pNodes[iSubNode].m_iSubNode;
		}

		// verify that each edge this node is connected to is likewise connected to this node
		for (int e=0; e < 4; e++)
		{
			if (pNode->m_vEdges[e] != -1)
			{
				assert( 
					m_pEdges[pNode->m_vEdges[e]].m_vNodes[0] == i || 
					m_pEdges[pNode->m_vEdges[e]].m_vNodes[1] == i );
			}
		}
	}
}
*/


// UNDONE: bnb - remove this global and allow all the subclasses of a CNavMeshObject to access the singleton somehow
CNavMeshObject *g_pNavMesh = NULL;

//=====================================
// Navigation Vertex
//=====================================

//=====================================
// Navigation Edge
//=====================================
float CNavEdge::GetLength() const
{
	Vec3 &pointA = g_pNavMesh->GetVertex(m_vVertices[0])->m_Position;
	Vec3 &pointB = g_pNavMesh->GetVertex(m_vVertices[1])->m_Position;

	return pointA.DistanceBetween( pointB );
}

//=====================================
// Navigation Subsector
//=====================================
float CNavSubsector::GetArea() const
{
	Vec3 &pointA = g_pNavMesh->GetVertex(m_vVertices[0])->m_Position;
	Vec3 &pointB = g_pNavMesh->GetVertex(m_vVertices[1])->m_Position;
	Vec3 &pointC = g_pNavMesh->GetVertex(m_vVertices[2])->m_Position;

	return (pointA-pointB).Cross( (pointC-pointB) ).Length();
}

Vec3 CNavSubsector::GetCenter() const
{
	Vec3 &pointA = g_pNavMesh->GetVertex(m_vVertices[0])->m_Position;
	Vec3 &pointB = g_pNavMesh->GetVertex(m_vVertices[1])->m_Position;
	Vec3 &pointC = g_pNavMesh->GetVertex(m_vVertices[2])->m_Position;

	return (pointA+pointB+pointC) / 3.0f;
}

//=====================================
// Navigation Sector
//=====================================

//=====================================
// Navigation Mesh Object
//=====================================
CNavMeshObject::CNavMeshObject(IHashString *parentName, IHashString *name, bool bAddToHier) :
	m_hszNameType( _T("CNavMeshObject") ),
	m_hszNavMeshRenderObject( _T("NavMeshRenderObject") ),
	OBJECTTEMPLATE(CNavMeshManager, CNavMeshObject, IObject, parentName, name)
{
	m_ToolBox = EngineGetToolBox();

	m_bAddToHierarchy = bAddToHier;
	if (bAddToHier)
	{
		AddToHierarchy();
	}

	g_pNavMesh = this;

	m_bVisible = false;
}

CNavMeshObject::~CNavMeshObject()
{
	if (m_bAddToHierarchy)
	{
		RemoveFromHierarchy();
	}
	DeInit();
}

void CNavMeshObject::Init()
{
	CNavVertex vertexA, vertexB, vertexC;
	vertexA.m_Position.Set( 0, 0, 0 );
	vertexB.m_Position.Set( 100, 0, 0 );
	vertexC.m_Position.Set( 0, 100, 0 );

	CNavEdge edgeA, edgeB, edgeC;
	edgeA.m_vVertices[0] = 0;
	edgeA.m_vVertices[1] = 1;
	edgeB.m_vVertices[0] = 1;
	edgeB.m_vVertices[1] = 2;
	edgeC.m_vVertices[0] = 2;
	edgeC.m_vVertices[1] = 0;
	edgeA.m_vAdjacentSubsectors[0] = 0;
	edgeB.m_vAdjacentSubsectors[0] = 0;
	edgeC.m_vAdjacentSubsectors[0] = 0;
	edgeA.m_Flags |= NAVFLAG_EDGE_PERIMETER | NAVFLAG_EDGE_SECTOR;
	edgeB.m_Flags |= NAVFLAG_EDGE_PERIMETER | NAVFLAG_EDGE_SECTOR;
	edgeC.m_Flags |= NAVFLAG_EDGE_PERIMETER | NAVFLAG_EDGE_SECTOR;

	CNavSubsector subsector;
	subsector.m_iParentSector = 0;
	subsector.m_vVertices[0] = 0;
	subsector.m_vVertices[1] = 1;
	subsector.m_vVertices[2] = 2;

	CNavSector sectorA;
	sectorA.m_Subsectors.push_back( 0 );

	m_Vertices.push_back( vertexA );
	m_Vertices.push_back( vertexB );
	m_Vertices.push_back( vertexC );
	m_Subsectors.push_back( subsector );
	m_Sectors.push_back( sectorA );


	// Test Insertion Functions
	DebugDumpNavMesh();
	InsertPointOnSubsectorEdge( 0, 0, 0.5f );
	DebugDumpNavMesh();
	InsertPointOnSubsectorEdge( 1, 2, 0.5f );
	DebugDumpNavMesh();
}

void CNavMeshObject::DeInit()
{
	VisualizeNavMesh( false );

	m_Sectors.clear();
	m_Subsectors.clear();
	m_Edges.clear();
	m_Vertices.clear();
}

void CNavMeshObject::Serialize( IArchive &ar )
{
	if(ar.IsReading())
	{
//		ar.Read( m_szNavFile, "File" );
	}
	else
	{
//		ar.Write( m_szNavFile, "File" );
	}
}

IHashString* CNavMeshObject::GetComponentType()
{
	return &m_hszNameType;
}

bool CNavMeshObject::IsKindOf( IHashString* compType )
{
	return (compType->GetUniqueID() == m_hszNameType.GetUniqueID() );
}

DWORD CNavMeshObject::OnGenerateNavMesh( DWORD size, void *params )
{
	return MSG_HANDLED_STOP;
}

DWORD CNavMeshObject::OnVisualizeNavMesh( DWORD size, void *params )
{
	VERIFY_MESSAGE_SIZE(sizeof(bool), size);
	bool *pMsg = (bool*)params;

	bool bVisible = *pMsg;

	VisualizeNavMesh( bVisible );

	return MSG_HANDLED_STOP;
}

void CNavMeshObject::VisualizeNavMesh( bool bVisible )
{
	if (!m_bVisible && bVisible)
	{
		CHashString hszNavRenderObjectType(_T("CNavMeshRenderObject"));
		CREATEOBJECTPARAMS cop;
		cop.name = &m_hszNavMeshRenderObject;
		cop.parentName = NULL;
		cop.typeName = &hszNavRenderObjectType;
		static DWORD msgHash_CreateObject = CHashString(_T("CreateObject")).GetUniqueID();
		m_ToolBox->SendMessage(msgHash_CreateObject, sizeof(cop), &cop );

		INITOBJECTPARAMS iop;
		iop.name = &m_hszNavMeshRenderObject;
		static DWORD msgHash_InitObject = CHashString(_T("InitObject")).GetUniqueID();
		m_ToolBox->SendMessage( msgHash_InitObject, sizeof(iop), &iop );
	}
	else if (m_bVisible && !bVisible)
	{
		DELETEOBJECTPARAMS dop;
		dop.name = &m_hszNavMeshRenderObject;
		static DWORD msgHash_DeleteObject = CHashString(_T("DeleteObject")).GetUniqueID();
		m_ToolBox->SendMessage( msgHash_DeleteObject, sizeof(dop), &dop );
	}
}

CNavVertex& CNavMeshObject::NewVertex( NAVINDEX *pNewIndex )
{
	CNavVertex temp;
	NAVINDEX iIndex = m_Vertices.size();
	m_Vertices.push_back( temp );
	if (pNewIndex)
		*pNewIndex = iIndex;
	return m_Vertices[iIndex];
}

CNavEdge& CNavMeshObject::NewEdge( NAVINDEX *pNewIndex )
{
	CNavEdge temp;
	NAVINDEX iIndex = m_Edges.size();
	m_Edges.push_back( temp );
	if (pNewIndex)
		*pNewIndex = iIndex;
	return m_Edges[iIndex];
}

CNavSubsector& CNavMeshObject::NewSubsector( NAVINDEX *pNewIndex )
{
	CNavSubsector temp;
	NAVINDEX iIndex = m_Subsectors.size();
	m_Subsectors.push_back( temp );
	if (pNewIndex)
		*pNewIndex = iIndex;
	return m_Subsectors[iIndex];
}

CNavSector& CNavMeshObject::NewSector( NAVINDEX *pNewIndex )
{
	CNavSector temp;
	NAVINDEX iIndex = m_Sectors.size();
	m_Sectors.push_back( temp );
	if (pNewIndex)
		*pNewIndex = iIndex;
	return m_Sectors[iIndex];
}

void CNavMeshObject::InsertPointOnSubsectorEdge( NAVINDEX iSubsector, int iEdge, float fEdgeDistance )
{
	// find new vertex position
	NAVINDEX v0, v1;
	GetSubsector(iSubsector)->GetEdgeVertices( iEdge, v0, v1 );
	Vec3 newVertexPos = (m_Vertices[v0].m_Position - m_Vertices[v1].m_Position) * fEdgeDistance + m_Vertices[v1].m_Position;

	// new vertex
	NAVINDEX iVertexNew;
	CNavVertex &vertexNew = NewVertex( &iVertexNew );
	vertexNew.m_Position = newVertexPos;

	// new subsectors
	NAVINDEX iAdjSubsector = GetSubsector(iSubsector)->m_AdjacentSubsectors[iEdge];
	NAVINDEX vNewSubsectors[2] = { INVALID_NAVINDEX, INVALID_NAVINDEX };
	NewSubsector( &vNewSubsectors[0] );
	if (iAdjSubsector != INVALID_NAVINDEX)
		NewSubsector( &vNewSubsectors[1] );

	// split adjacent sub sectors
	for (int iEdgeSide=0; iEdgeSide < 2; iEdgeSide++)
	{
		NAVINDEX iCurrSubSector;
		NAVINDEX vEdgeVerticesCCW[2];
		int iCurrEdge;
		if (iEdgeSide == 0)
		{
			iCurrSubSector = iSubsector;
		}
		else
		{
			iCurrSubSector = iAdjSubsector;
		}

		if (iCurrSubSector != INVALID_NAVINDEX)
		{
			CNavSubsector *pCurrSubsector = GetSubsector(iCurrSubSector);

			if (iEdgeSide == 0)
			{
				vEdgeVerticesCCW[0] = v0;
				vEdgeVerticesCCW[1] = v1;
				iCurrEdge = iEdge;
			}
			else
			{
				vEdgeVerticesCCW[0] = v1;
				vEdgeVerticesCCW[1] = v0;
				iCurrEdge = pCurrSubsector->GetEdgeFromVertices( v1, v0 );
			}

			// if this is a two sided edge, update this sector's adj sector with the new one on the other side
			pCurrSubsector->m_AdjacentSubsectors[iCurrEdge] = vNewSubsectors[!iEdgeSide];

			// find opposing vertex
			NAVINDEX iVertexOpposing = pCurrSubsector->GetNextVertexCCW( vEdgeVerticesCCW[1] );

			// make a new subsector
			CNavSubsector *pNewSubsector = GetSubsector( vNewSubsectors[iEdgeSide] );
			*pNewSubsector = *pCurrSubsector;
			pNewSubsector->m_vVertices[0] = iVertexOpposing;
			pNewSubsector->m_vVertices[1] = iVertexNew;
			pNewSubsector->m_vVertices[2] = vEdgeVerticesCCW[1]; // vertex B of original edge
			pNewSubsector->m_AdjacentSubsectors[0] = iCurrSubSector;
			pNewSubsector->m_AdjacentSubsectors[1] = pCurrSubsector->m_AdjacentSubsectors[iCurrEdge];
			pNewSubsector->m_AdjacentSubsectors[2] = pCurrSubsector->m_AdjacentSubsectors[(iCurrEdge+1)%3];

			// change original subsector's vertices
			pCurrSubsector->SetNextVertexCCW( vEdgeVerticesCCW[0], iVertexNew );

			// update new subsector's neighbors to refer to it
			for (int iAdjSector=0; iAdjSector < 3; iAdjSector++)
			{
				NAVINDEX newSubsectorV0, newSubsectorV1;
				pNewSubsector->GetEdgeVertices( iAdjSector, newSubsectorV0, newSubsectorV1 );
				CNavSubsector *pSubsector = GetSubsector(pNewSubsector->m_AdjacentSubsectors[iAdjSector]);
				if (pSubsector)
				{
					int iEdge = pSubsector->GetEdgeFromVertices(newSubsectorV0, newSubsectorV1);
					pSubsector->m_AdjacentSubsectors[iEdge] = vNewSubsectors[iEdgeSide];
				}
			}

			// add new subsector to list of subsector's in parent sector
			CNavSector *pSector = GetSector( pCurrSubsector->m_iParentSector );
			pSector->m_Subsectors.push_back( vNewSubsectors[iEdgeSide] );
		}
	}
}

void CNavMeshObject::DebugDumpNavMesh()
{
	TCHAR szTemp[1000];
	for (unsigned int i=0; i < m_Sectors.size(); i++)
	{
		sprintf( szTemp, "Sector[%i]\n", i );
		OutputDebugString( szTemp );
		for (unsigned int j=0; j < GetSector(i)->m_Subsectors.size(); j++)
		{
			sprintf( szTemp, "\tSubsector[%i]: %i\n", j, GetSector(i)->m_Subsectors[j] );
			OutputDebugString( szTemp );
		}
	}

	for (unsigned int i=0; i < m_Subsectors.size(); i++)
	{
		sprintf( szTemp, "Subsector[%i]\n\tParent: %i\n\tv0: %i\n\tv1: %i\n\tv2: %i\n\tAdj.Sector 0: %i\n\tAdj.Sector 1: %i\n\tAdj.Sector 2: %i\n", 
			i, 
			m_Subsectors[i].m_iParentSector, 
			m_Subsectors[i].m_vVertices[0], 
			m_Subsectors[i].m_vVertices[1], 
			m_Subsectors[i].m_vVertices[2], 
			m_Subsectors[i].m_AdjacentSubsectors[0], 
			m_Subsectors[i].m_AdjacentSubsectors[1], 
			m_Subsectors[i].m_AdjacentSubsectors[2]
			);
		OutputDebugString( szTemp );
	}

	for (unsigned int i=0; i < m_Edges.size(); i++)
	{
		sprintf( szTemp, "Edge[%i]\n\tv0: %i\n\tv1: %i\n\tAdj.Sector 0: %i\n\tAdj.Sector 1: %i\n", 
			i, 
			m_Edges[i].m_vVertices[0], 
			m_Edges[i].m_vVertices[1], 
			m_Edges[i].m_vAdjacentSubsectors[0], 
			m_Edges[i].m_vAdjacentSubsectors[1]
			);
		OutputDebugString( szTemp );
	}

/*	for (int i=0; i < m_Vertices.size(); i++)
	{
	}
*/
}
