///============================================================================
/// \file		NodeSystem.cpp
/// \brief		Implmentation of CNodeSystem Class
/// \date		07-11-2007
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

#include "limits.h"
#include "stdafx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

REGISTER_COMPONENT(CNodeSystem);
REGISTER_MESSAGE_HANDLER( GetGlobalPosition, OnGetGlobalPosition, CNodeSystem );
REGISTER_MESSAGE_HANDLER( SetGlobalPosition, OnSetGlobalPosition, CNodeSystem );
REGISTER_MESSAGE_HANDLER( GetSchema, OnGetSchema, CNodeSystem );
REGISTER_MESSAGE_HANDLER( GetVertices, OnGetVertices, CNodeSystem );
REGISTER_MESSAGE_HANDLER( GetOrderedVertices, OnGetOrderedVertices, CNodeSystem );
REGISTER_MESSAGE_HANDLER( GetBoundingBox, OnGetBoundingBox, CNodeSystem );

CNodeSystem::CNodeSystem( IHashString *parentName, IHashString *name ) :
	SCENEOBJECTTEMPLATE( CNodeSystemManager, CNodeSystem, ISceneObject, parentName, name )
{
	m_fVersion = 1.1f;
	m_fPointSize = 4;
	m_iPointR = 255;
	m_iPointG = 0;
	m_iPointB = 0;

	m_fLineSize = 1;
	m_iLineR = 0;
	m_iLineG = 255;
	m_iLineB = 255;

	AddToHierarchy();
}

CNodeSystem::~CNodeSystem()
{
	RemoveFromHierarchy();
}

void CNodeSystem::Init()
{
	UpdateTransformsFromParent();
}

bool CNodeSystem::Update()
{
	if (m_bTransformIsDirty)
	{
		UpdateTransformsFromParent();
		m_bTransformIsDirty = false;
	}

	return true;
}

void CNodeSystem::DeInit()
{
	DELETEOBJECTPARAMS deleteObjectParams;
	IDTOOBJECTMAP::iterator objectIterator;

	objectIterator = m_Nodes.begin();
	while( objectIterator != m_Nodes.end() )
	{
		int oldNodesCount = (int)m_Nodes.size();
		deleteObjectParams.name = objectIterator->second->GetName();
		static DWORD msgHash_DeleteObject = CHashString(_T("DeleteObject")).GetUniqueID();
		DWORD retVal = m_ToolBox->SendMessage(msgHash_DeleteObject, sizeof(DELETEOBJECTPARAMS), &deleteObjectParams );
		if( retVal != MSG_HANDLED || m_Nodes.size() == oldNodesCount )
		{
			m_ToolBox->Log( LOGERROR, _T("Could not delete object %s from node system %s."), deleteObjectParams.name->GetString(), GetName()->GetString() );
		}
		objectIterator = m_Nodes.begin();
	}

	objectIterator = m_NodeConnections.begin();
	while( objectIterator != m_NodeConnections.end() )
	{
		int oldNodeConnectionsCount = (int)m_NodeConnections.size();
		deleteObjectParams.name = objectIterator->second->GetName();
		static DWORD msgHash_DeleteObject = CHashString(_T("DeleteObject")).GetUniqueID();
		DWORD retVal = m_ToolBox->SendMessage(msgHash_DeleteObject, sizeof(DELETEOBJECTPARAMS), &deleteObjectParams );
		if( retVal != MSG_HANDLED || m_NodeConnections.size() == oldNodeConnectionsCount )
		{
			m_ToolBox->Log( LOGERROR, _T("Could not delete object %s from node system %s."), deleteObjectParams.name->GetString(), GetName()->GetString() );
		}
		objectIterator = m_NodeConnections.begin();
	}

	objectIterator = m_NodeFaces.begin();
	while( objectIterator != m_NodeFaces.end() )
	{
		int oldNodeFacesCount = (int)m_NodeFaces.size();
		deleteObjectParams.name = objectIterator->second->GetName();
		static DWORD msgHash_DeleteObject = CHashString(_T("DeleteObject")).GetUniqueID();
		DWORD retVal = m_ToolBox->SendMessage(msgHash_DeleteObject, sizeof(DELETEOBJECTPARAMS), &deleteObjectParams );
		if( retVal != MSG_HANDLED || m_NodeFaces.size() == oldNodeFacesCount )
		{
			m_ToolBox->Log( LOGERROR, _T("Could not delete object %s from node system %s."), deleteObjectParams.name->GetString(), GetName()->GetString() );
		}
		objectIterator = m_NodeFaces.begin();
	}

	objectIterator = m_HeightNodes.begin();
	while( objectIterator != m_HeightNodes.end() )
	{
		int oldHeightNodesCount = (int)m_HeightNodes.size();
		deleteObjectParams.name = objectIterator->second->GetName();
		static DWORD msgHash_DeleteObject = CHashString(_T("DeleteObject")).GetUniqueID();
		DWORD retVal = m_ToolBox->SendMessage(msgHash_DeleteObject, sizeof(DELETEOBJECTPARAMS), &deleteObjectParams );
		if( retVal != MSG_HANDLED || m_HeightNodes.size() == oldHeightNodesCount )
		{
			m_ToolBox->Log( LOGERROR, _T("Could not delete object %s from node system %s."), deleteObjectParams.name->GetString(), GetName()->GetString() );
		}
		objectIterator = m_HeightNodes.begin();
	}
}

IComponent *CNodeSystem::Create(int nArgs, va_list argptr)
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
		self = new CNodeSystem( parentName, name );
	}
	catch(...)
	{
		return NULL;
	}

	return self;
}

void CNodeSystem::Serialize( IArchive &ar )
{
	if( ar.IsReading() )
	{
		float fVersion;
		StdString tempString;
		ar.Read( fVersion, _T("Version"));
		if( fVersion == 1.1f )
		{
			ar.Read( m_fPointSize, _T("PointSize") );
			ar.Read( m_iPointR, _T("PointR") );
			ar.Read( m_iPointG, _T("PointG") );
			ar.Read( m_iPointB, _T("PointB") );

			ar.Read( m_fLineSize, _T("LineSize") );
			ar.Read( m_iLineR, _T("LineR") );
			ar.Read( m_iLineG, _T("LineG") );
			ar.Read( m_iLineB, _T("LineB") );
		}
	}
	else
	{
		ar.Write( m_fVersion, _T("Version"));

		ar.Write( m_fPointSize, _T("PointSize") );
		ar.Write( m_iPointR, _T("PointR") );
		ar.Write( m_iPointG, _T("PointG") );
		ar.Write( m_iPointB, _T("PointB") );

		ar.Write( m_fLineSize, _T("LineSize") );
		ar.Write( m_iLineR, _T("LineR") );
		ar.Write( m_iLineG, _T("LineG") );
		ar.Write( m_iLineB, _T("LineB") );
	}
}

void CNodeSystem::AddNode( CNode *node )
{
	if( node != NULL )
	{
		DWORD nodeID = node->GetName()->GetUniqueID();
		m_Nodes[nodeID] = node;
		m_OrderedNodes.push_back( nodeID );
	}
}

void CNodeSystem::AddNodeConnection( CNodeConnection *nodeConnection )
{
	if( nodeConnection != NULL )
	{
		DWORD nodeConnectionID = nodeConnection->GetName()->GetUniqueID();
		m_NodeConnections[nodeConnectionID] = nodeConnection;
	}
}

void CNodeSystem::AddNodeFace( CNodeFace *nodeFace )
{
	if( nodeFace != NULL )
	{
		DWORD nodeFaceID = nodeFace->GetName()->GetUniqueID();
		m_NodeFaces[nodeFaceID] = nodeFace;
	}
}

void CNodeSystem::AddHeightNode( CHeightNode *heightNode )
{
	if( heightNode != NULL )
	{
		DWORD heightNodeID = heightNode->GetName()->GetUniqueID();
		m_HeightNodes[heightNodeID] = heightNode;
	}
}

void CNodeSystem::RemoveNode( IHashString *nodeName )
{
	if( nodeName != NULL )
	{
		DWORD nodeID = nodeName->GetUniqueID();
		m_Nodes.erase(nodeID);
		m_OrderedNodes.remove( nodeID );
	}
}

void CNodeSystem::RemoveNodeConnection( IHashString *nodeConnectionName )
{
	if( nodeConnectionName != NULL )
	{
		DWORD nodeConnectionID = nodeConnectionName->GetUniqueID();
		m_NodeConnections.erase(nodeConnectionID);
	}
}

void CNodeSystem::RemoveNodeFace( IHashString *nodeFaceName )
{
	if( nodeFaceName != NULL )
	{
		DWORD nodeFaceID = nodeFaceName->GetUniqueID();
		m_NodeFaces.erase(nodeFaceID);
	}
}

void CNodeSystem::RemoveHeightNode( IHashString *heightNodeName )
{
	if( heightNodeName != NULL )
	{
		DWORD heightNodeID = heightNodeName->GetUniqueID();
		m_HeightNodes.erase(heightNodeID);
	}
}

IDTOOBJECTMAP *CNodeSystem::GetNodesMap()
{
	return &m_Nodes;
}

IDTOOBJECTMAP *CNodeSystem::GetHeightNodesMap()
{
	return &m_HeightNodes;
}

Vec3 &CNodeSystem::GetPosition()
{
	static DWORD msgHash_GetGlobalPosition = CHashString(_T("GetGlobalPosition")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_GetGlobalPosition, sizeof(Vec3), &m_vPosition, GetParentName());

	return m_vPosition;
}

void CNodeSystem::SetPosition( const Vec3 &position )
{
	m_vPosition.Set( position );
	static DWORD msgHash_SetGlobalPosition = CHashString(_T("SetGlobalPosition")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_SetGlobalPosition, sizeof(Vec3), &m_vPosition, GetParentName());
}

float CNodeSystem::GetPointSize()
{
	return m_fPointSize;
}

int CNodeSystem::GetPointR()
{
	return m_iPointR;
}

int CNodeSystem::GetPointG()
{
	return m_iPointG;
}

int CNodeSystem::GetPointB()
{
	return m_iPointB;
}

float CNodeSystem::GetLineSize()
{
	return m_fLineSize;
}

int CNodeSystem::GetLineR()
{
	return m_iLineR;
}

int CNodeSystem::GetLineG()
{
	return m_iLineG;
}

int CNodeSystem::GetLineB()
{
	return m_iLineB;
}

DWORD CNodeSystem::OnGetGlobalPosition( DWORD size, void *params )
{
	VERIFY_MESSAGE_SIZE( size, sizeof(Vec3) );
	Vec3 *vPosition = (Vec3*)params;

	static DWORD msgHash_GetGlobalPosition = CHashString(_T("GetGlobalPosition")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_GetGlobalPosition, sizeof(Vec3), vPosition, GetParentName());

	m_vPosition.Set( *vPosition );

	return MSG_HANDLED_STOP;
}

DWORD CNodeSystem::OnGetBoundingBox( DWORD size, void* params )
{
	DWORD retVal = MSG_ERROR;
	VERIFY_MESSAGE_SIZE( size, sizeof( GETBOUNDINGBOXPARAMS ) );
	GETBOUNDINGBOXPARAMS *getBoundingBoxParams = (GETBOUNDINGBOXPARAMS*)params;
	if( getBoundingBoxParams && getBoundingBoxParams->position && getBoundingBoxParams->extents && getBoundingBoxParams->rotation )
	{
		IDTOOBJECTMAP::iterator nodesIterator = m_Nodes.begin();
		CNode *iteratedNode = NULL;
		Vec3 nodePosition( 0.0, 0.0, 0.0 );
		Vec3 vMin( 0.0, 0.0, 0.0 );
		Vec3 vMax( 0.0, 0.0, 0.0 );

		if( nodesIterator != m_Nodes.end() )
		{
			iteratedNode = (CNode*)nodesIterator->second;
			iteratedNode->GetGlobalPosition( &nodePosition );
			vMin.Set( nodePosition );
			vMax.Set( nodePosition );
		}
		for( ; nodesIterator != m_Nodes.end(); nodesIterator++ )
		{
			CNode *iteratedNode = (CNode*)nodesIterator->second;
			Vec3 nodePosition( 0.0, 0.0, 0.0 );
			iteratedNode->GetGlobalPosition( &nodePosition );
			if( nodePosition.x < vMin.x )
			{
				vMin.x = nodePosition.x;
			}
			if( nodePosition.x > vMax.x )
			{
				vMax.x = nodePosition.x;
			}

			if( nodePosition.y < vMin.y )
			{
				vMin.y = nodePosition.y;
			}
			if( nodePosition.y > vMax.y )
			{
				vMax.y = nodePosition.y;
			}

			if( nodePosition.z < vMin.z )
			{
				vMin.z = nodePosition.z;
			}
			if( nodePosition.z > vMax.z )
			{
				vMax.z = nodePosition.z;
			}
		}

		IDTOOBJECTMAP::iterator heightNodesIterator = m_HeightNodes.begin();
		CHeightNode *iteratedHeightNode = NULL;
		if( heightNodesIterator != m_HeightNodes.end() )
		{
			iteratedHeightNode = (CHeightNode*)heightNodesIterator->second;
			iteratedHeightNode->GetGlobalPosition( &nodePosition );
			vMin.Set( nodePosition );
			vMax.Set( nodePosition );
		}
		for( ; heightNodesIterator != m_HeightNodes.end(); heightNodesIterator++ )
		{
			CHeightNode *iteratedHeightNode = (CHeightNode*)heightNodesIterator->second;
			Vec3 heightNodePosition( 0.0, 0.0, 0.0 );
			iteratedHeightNode->GetGlobalPosition( &heightNodePosition );
			if( heightNodePosition.x < vMin.x )
			{
				vMin.x = heightNodePosition.x;
			}
			if( heightNodePosition.x > vMax.x )
			{
				vMax.x = heightNodePosition.x;
			}

			if( heightNodePosition.y < vMin.y )
			{
				vMin.y = heightNodePosition.y;
			}
			if( heightNodePosition.y + iteratedHeightNode->GetHeight() > vMax.y )
			{
				vMax.y = heightNodePosition.y + iteratedHeightNode->GetHeight();
			}

			if( heightNodePosition.z < vMin.z )
			{
				vMin.z = heightNodePosition.z;
			}
			if( heightNodePosition.z > vMax.z )
			{
				vMax.z = heightNodePosition.z;
			}
		}

		getBoundingBoxParams->position->Set( vMin );
		(*getBoundingBoxParams->position) += vMax;
		(*getBoundingBoxParams->position) /= 2.0f;

		getBoundingBoxParams->extents->Set( vMax );
		(*getBoundingBoxParams->extents) -= vMin ;
		(*getBoundingBoxParams->extents) /= 2.0f;

		retVal = MSG_HANDLED;
	}
	return retVal;
}

DWORD CNodeSystem::OnSetGlobalPosition( DWORD size, void *params )
{
	VERIFY_MESSAGE_SIZE( size, sizeof(Vec3) );
	Vec3 *vPosition = (Vec3*)params;

	m_vPosition.Set( *vPosition );
	static DWORD msgHash_SetGlobalPosition = CHashString(_T("SetGlobalPosition")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_SetGlobalPosition, sizeof(Vec3), &m_vPosition, GetParentName());

	return MSG_HANDLED_STOP;
}

DWORD CNodeSystem::OnGetSchema( DWORD size, void *params )
{
	VERIFY_MESSAGE_SIZE(size, sizeof(GETSCHEMAPARAMS));
	GETSCHEMAPARAMS *getSchemaParams = (GETSCHEMAPARAMS*)params;
	IXMLArchive *archive = getSchemaParams->ar;
	if( archive != NULL )
	{
		archive->StartClass( GetComponentType()->GetString() );
		archive->Write( m_Name.GetString(), _T("Name") );
		archive->Write( 9 );

		archive->StartClass(_T("hidden") );
		archive->Write( _T("float"), _T("Type") );
		archive->EndClass();

		archive->StartClass( _T("edit") );
		archive->Write( _T("float"), _T("Type") );
		archive->Write( _T("Point Size"), _T("Name") );
		archive->Write( false, _T("Key") );
		archive->EndClass();

		archive->StartClass( _T("edit") );
		archive->Write( _T("int"), _T("Type") );
		archive->Write( _T("Point Red"), _T("Name") );
		archive->Write( false, _T("Key") );
		archive->EndClass();

		archive->StartClass( _T("edit") );
		archive->Write( _T("int"), _T("Type") );
		archive->Write( _T("Point Green"), _T("Name") );
		archive->Write( false, _T("Key") );
		archive->EndClass();

		archive->StartClass( _T("edit") );
		archive->Write( _T("int"), _T("Type") );
		archive->Write( _T("Point Blue"), _T("Name") );
		archive->Write( false, _T("Key") );
		archive->EndClass();

		archive->StartClass( _T("edit") );
		archive->Write( _T("float"), _T("Type") );
		archive->Write( _T("Line Size"), _T("Name") );
		archive->Write( false, _T("Key") );
		archive->EndClass();

		archive->StartClass( _T("edit") );
		archive->Write( _T("int"), _T("Type") );
		archive->Write( _T("Line Red"), _T("Name") );
		archive->Write( false, _T("Key") );
		archive->EndClass();

		archive->StartClass( _T("edit") );
		archive->Write( _T("int"), _T("Type") );
		archive->Write( _T("Line Green"), _T("Name") );
		archive->Write( false, _T("Key") );
		archive->EndClass();

		archive->StartClass( _T("edit") );
		archive->Write( _T("int"), _T("Type") );
		archive->Write( _T("Line Blue"), _T("Name") );
		archive->Write( false, _T("Key") );
		archive->EndClass();

		archive->EndClass();
	}
	else
	{
		m_ToolBox->Log( LOGERROR, _T("Attempt to get CNodeSystem schema. No archive specified.\n" ) );
	}

	return MSG_HANDLED_STOP;
}

DWORD CNodeSystem::OnGetVertices( DWORD size, void *params )
{
	VERIFY_MESSAGE_SIZE( size, sizeof(GETVERTICES) );
	GETVERTICES *gv = (GETVERTICES *)params;
//		IArchive *archive = *(IArchive**)params;
	gv->archive->SetIsWriting( true );

	IHashString *managerName = GetManagerName();
	IComponent *amanagerComponent = m_ToolBox->GetComponent( managerName );
	CNodeSystemManager *amanager = dynamic_cast<CNodeSystemManager*>( amanagerComponent );

	int numVerts = (int)m_Nodes.size() + (int)m_HeightNodes.size() * 2;
	gv->archive->Write( numVerts, _T("NumVerts") );
	map< DWORD, int > nodeToIndexMap;

	// TODO <rnguyen>: Code redundancy between CNode and CHeightNode. Will want to consilidate all this.
	IDTOOBJECTMAP::iterator nodesIterator = m_Nodes.begin();
	int index = 0;
	for( ; nodesIterator != m_Nodes.end(); nodesIterator++ )
	{
		CNode *iteratedNode = (CNode*)(nodesIterator->second);
		nodeToIndexMap[ iteratedNode->GetName()->GetUniqueID() ] = index;
		Vec3 nodePosition = iteratedNode->GetPosition();
		gv->archive->Write( nodePosition, _T("NodePosition") );
		index++;
	}

	map< DWORD, int > nodeHeightToIndexMap;
	IDTOOBJECTMAP::iterator heightNodesIterator = m_HeightNodes.begin();
	for( ; heightNodesIterator != m_HeightNodes.end(); heightNodesIterator++ )
	{
		CHeightNode *iteratedHeightNode = (CHeightNode*)(heightNodesIterator->second);
		nodeToIndexMap[ iteratedHeightNode->GetName()->GetUniqueID() ] = index;
		Vec3 heightNodePosition = iteratedHeightNode->GetPosition();
		gv->archive->Write( heightNodePosition, _T("NodePosition") );
		index++;

		nodeHeightToIndexMap[ iteratedHeightNode->GetName()->GetUniqueID() ] = index;
		Vec3 nodeHeightPosition = heightNodePosition;
		nodeHeightPosition.y += iteratedHeightNode->GetHeight();
		gv->archive->Write( nodeHeightPosition, _T("NodePosition") );
		index++;
	}

	int numHeightNodes = (int)m_HeightNodes.size();
	int numFaces = (int)m_NodeFaces.size() + numHeightNodes * 2;
	if( numHeightNodes > 2 )
	{
		numFaces += (numHeightNodes - 2) * 2;
	}

	// BUILD GENERIC FACES
	// If we have NodeFaces, build their data here:
	gv->archive->Write( numFaces, _T("NumFaces") );
	IDTOOBJECTMAP::iterator facesIterator = m_NodeFaces.begin();
	for( ; facesIterator != m_NodeFaces.end(); facesIterator++ )
	{
		CNodeFace *iteratedFace = (CNodeFace*)(facesIterator->second);
		int index1 = nodeToIndexMap[ iteratedFace->GetNode1Name()->GetUniqueID() ];
		int index2 = nodeToIndexMap[ iteratedFace->GetNode2Name()->GetUniqueID() ];
		int index3 = nodeToIndexMap[ iteratedFace->GetNode3Name()->GetUniqueID() ];
		gv->archive->Write( index1, _T("NodeFace") );
		gv->archive->Write( index2, _T("NodeFace") );
		gv->archive->Write( index3, _T("NodeFace") );
	}

	// HEIGHT NODE : Build Side Faces
	// Side Faces are generated here by crawling along the sides in order.
	heightNodesIterator = m_HeightNodes.begin();
	for( ; heightNodesIterator != m_HeightNodes.end(); heightNodesIterator++ )
	{
		CHeightNode *iteratedHeightNode = (CHeightNode*)(heightNodesIterator->second);
		CHeightNode *connectedHeightNode = iteratedHeightNode->GetConnectedHeightNode();
		if( connectedHeightNode == NULL )
		{
			connectedHeightNode = iteratedHeightNode;
		}

		int index1 = nodeToIndexMap[ iteratedHeightNode->GetName()->GetUniqueID() ];
		int index2 = nodeHeightToIndexMap[ iteratedHeightNode->GetName()->GetUniqueID() ];
		int index3 = nodeToIndexMap[ connectedHeightNode->GetName()->GetUniqueID() ];

		int index4 = nodeToIndexMap[ connectedHeightNode->GetName()->GetUniqueID() ];
		int index5 = nodeHeightToIndexMap[ iteratedHeightNode->GetName()->GetUniqueID() ];
		int index6 = nodeHeightToIndexMap[ connectedHeightNode->GetName()->GetUniqueID() ];

		gv->archive->Write( index1, _T("NodeFace") );
		gv->archive->Write( index2, _T("NodeFace") );
		gv->archive->Write( index3, _T("NodeFace") );

		gv->archive->Write( index4, _T("NodeFace") );
		gv->archive->Write( index5, _T("NodeFace") );
		gv->archive->Write( index6, _T("NodeFace") );
	}

	// HEIGHT NODE : Build Top and Bottom Faces
	vector<int> polygonFaces;
	map< int, CHeightNode* > connectedIndexToNodeMap;
	if( numHeightNodes > 2 )
	{
		
		CNodeVolumeTessellator polygonTessellator;
		heightNodesIterator = m_HeightNodes.begin();
		CHeightNode *heightNodeBegin = (CHeightNode*)(heightNodesIterator->second);
		CHeightNode *heightNode = heightNodeBegin;
		do
		{
			connectedIndexToNodeMap[connectedIndexToNodeMap.size()] = heightNode;
			Vec3 v;
			heightNode->GetLocalPosition(&v);
			polygonTessellator.pushPoint( v.x, v.z );

			heightNode = heightNode->GetConnectedHeightNode();
			if( heightNode == NULL )
			{
				m_ToolBox->Log( LOGERROR, _T("Failed to connec vertices for height node top/bottom faces.\n") );
				break;
			}
		} while (heightNode != heightNodeBegin);

		polygonTessellator.tessellatePoints( polygonFaces );

		if ((numHeightNodes - 2) != (polygonFaces.size() / 3))
		{
			m_ToolBox->Log( LOGERROR, _T("Failed to generate correct number of vertices for height node top/bottom faces.") );
		}
	}

	// HEIGHT NODE : Write Top and Bottom Faces
	for (unsigned int i=0; i < (polygonFaces.size() / 3); i++)
	{
		CHeightNode *heightNode1 = connectedIndexToNodeMap[ polygonFaces[i*3+0] ];
		CHeightNode *heightNode2 = connectedIndexToNodeMap[ polygonFaces[i*3+1] ];
		CHeightNode *heightNode3 = connectedIndexToNodeMap[ polygonFaces[i*3+2] ];

		int index1 = nodeToIndexMap[ heightNode1->GetName()->GetUniqueID() ];
		int index2 = nodeToIndexMap[ heightNode2->GetName()->GetUniqueID() ];
		int index3 = nodeToIndexMap[ heightNode3->GetName()->GetUniqueID() ];

		int index4 = nodeHeightToIndexMap[ heightNode1->GetName()->GetUniqueID() ];
		int index5 = nodeHeightToIndexMap[ heightNode2->GetName()->GetUniqueID() ];
		int index6 = nodeHeightToIndexMap[ heightNode3->GetName()->GetUniqueID() ];

		gv->archive->Write( index1, _T("NodeFace") );
		gv->archive->Write( index2, _T("NodeFace") );
		gv->archive->Write( index3, _T("NodeFace") );

		gv->archive->Write( index4, _T("NodeFace") );
		gv->archive->Write( index5, _T("NodeFace") );
		gv->archive->Write( index6, _T("NodeFace") );
	}

	return MSG_HANDLED_STOP;
}

DWORD CNodeSystem::OnGetOrderedVertices( DWORD size, void *params )
{
	VERIFY_MESSAGE_SIZE( size, sizeof(GETVERTICES) );
	GETVERTICES *gv = (GETVERTICES *)params;
	gv->archive->SetIsWriting( true );

	IHashString *managerName = GetManagerName();
	IComponent *amanagerComponent = m_ToolBox->GetComponent( managerName );
	CNodeSystemManager *amanager = dynamic_cast<CNodeSystemManager*>( amanagerComponent );

	int numVerts = 0;

	numVerts = (int)m_HeightNodes.size();
	if (numVerts > 0)
	{
		gv->archive->Write( numVerts, _T("NumVerts") );
		IDTOOBJECTMAP::iterator heightnodeIDIterator = m_HeightNodes.begin();
		CHeightNode *iteratedHeightNode  = (CHeightNode*)(heightnodeIDIterator->second);

		for (int i = 0; (i < numVerts) && (iteratedHeightNode != NULL); i++)
		{
			Vec3 v3Pos;
			iteratedHeightNode->GetGlobalPosition(&v3Pos);
			gv->archive->Write( v3Pos, _T("NodePosition") );
			iteratedHeightNode = iteratedHeightNode->GetConnectedHeightNode();
		}
		return MSG_HANDLED_STOP;
	}
	numVerts = (int)m_Nodes.size();
	if (numVerts > 0)
	{
		gv->archive->Write( numVerts, _T("NumVerts") );
		list<DWORD>::iterator nodeIDIterator = m_OrderedNodes.begin();
		for( ; nodeIDIterator != m_OrderedNodes.end(); nodeIDIterator++ )
		{
			DWORD nodeID = *nodeIDIterator;
			CNode *iteratedNode = (CNode*)m_Nodes[ nodeID ];
			Vec3 nodePosition = iteratedNode->GetPosition();
			gv->archive->Write( nodePosition, _T("NodePosition") );
		}
		return MSG_HANDLED_STOP;
	}
	return MSG_ERROR;
}

void CNodeSystem::UpdateTransformsFromParent()
{
	static DWORD msgHash_GetGlobalPosition = CHashString(_T("GetGlobalPosition")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_GetGlobalPosition, sizeof(Vec3), &m_vPosition, GetParentName());

	// TODO <rnguyen>: Only going to worry about position transformations for the moment

	//static DWORD msgHash_GetGlobalEuler = CHashString(_T("GetGlobalEuler")).GetUniqueID();
	//m_ToolBox->SendMessage(msgHash_GetGlobalEuler, sizeof(EulerAngle), &m_vOrientation, GetParentName());

	//static DWORD msgHash_GetGlobalScale = CHashString(_T("GetGlobalScale")).GetUniqueID();
	//m_ToolBox->SendMessage(msgHash_GetGlobalScale, sizeof(Vec3), &m_Scale, GetParentName());

	//static DWORD msgHash_GetGlobalTransform = CHashString(_T("GetGlobalTransform")).GetUniqueID();
	//m_ToolBox->SendMessage(msgHash_GetGlobalTransform, sizeof(Matrix4x4), &m_Transform, GetParentName());	

	//float fSectorSize = m_pTerrainMgr->GetMetrics().fSectorSize;
	//m_Position += Vec3( (float)m_iSectorX * fSectorSize, 0.0f, (float)m_iSectorY * fSectorSize );

	//m_Transform.SetTranslation(m_Position);
}