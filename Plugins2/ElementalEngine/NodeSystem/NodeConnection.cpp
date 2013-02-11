///============================================================================
/// \file		NodeConnection.cpp
/// \brief		Implmentation of CNodeConnection Class
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

#include "stdafx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

REGISTER_COMPONENT(CNodeConnection);
REGISTER_MESSAGE_HANDLER( GetSchema, OnGetSchema, CNodeConnection );

CNodeConnection::CNodeConnection( IHashString *parentName, IHashString *name ) :
	m_hszLabel(_T("")),
	m_hszNode1(_T("")),
	m_hszNode2(_T("")),
	OBJECTTEMPLATE( CNodeSystemManager, CNodeConnection, IObject, parentName, name )
{
	m_ToolBox = EngineGetToolBox();

	m_fVersion = 1.0f;
	m_hszLabel = name;

	m_NodeSystem = NULL;
	m_Node1 = NULL;
	m_Node2 = NULL;
	m_RenderObject = NULL;
	if( parentName != NULL )
	{
		CNodeSystemManager *amanager = GetManager();
		m_NodeSystem = amanager->GetNodeSystem( parentName );
	}

	AddToHierarchy();
}

CNodeConnection::~CNodeConnection()
{
	RemoveFromHierarchy();
}

void CNodeConnection::Init()
{
	IHashString *parentName = GetParentName();
	if( parentName != NULL )
	{
		CNodeSystemManager *amanager = GetManager();
		CNodeSystem *parentNodeSystem = amanager->GetNodeSystem( parentName );
		if( parentNodeSystem != NULL )
		{
			parentNodeSystem->AddNodeConnection( this );
		}
		else
		{
			m_ToolBox->Log( LOGERROR, _T("Could not find parent object %s for node connection %s."), parentName->GetString(), GetName()->GetString() );
		}
	}
	else
	{
		m_ToolBox->Log( LOGWARNING, _T("Parent object not specified for node connection %s."), GetName()->GetString() );
	}

	CHashString hszNodeConnectionName = GetName();
	StdString szNodeConnectionName = hszNodeConnectionName.GetString();

	StdString szNodeConnectionRenderObjectName = szNodeConnectionName + _T("_RenderObject");
	CHashString hszNodeConnectionRenderObjectName(szNodeConnectionRenderObjectName.c_str());
	static CHashString hszNodeConnectionRenderObjectComponentType(_T("CNodeConnectionRenderObject"));

	if( m_RenderObject == NULL )
	{
		IComponent *renderObjectComponent = m_ToolBox->CreateComponent( &hszNodeConnectionRenderObjectComponentType, 2, &hszNodeConnectionRenderObjectName, &hszNodeConnectionName );
		m_RenderObject = dynamic_cast<CNodeConnectionRenderObject*>(renderObjectComponent);
		m_RenderObject->Init();
	}
}

bool CNodeConnection::Update()
{
	return true;
}

void CNodeConnection::DeInit()
{
	if( m_Node1 != NULL )
	{
		m_Node1->DeregisterConnection( this );
	}
	if( m_Node2 != NULL )
	{
		m_Node2->DeregisterConnection( this );
	}

	IComponent *amanagerComponent = m_ToolBox->GetComponent( GetManagerName() );
	CNodeSystemManager *amanager = dynamic_cast<CNodeSystemManager*>( amanagerComponent );

	CNodeSystem *parentNodeSystem = amanager->GetNodeSystem( GetParentName() );
	if( parentNodeSystem != NULL )
	{
		parentNodeSystem->RemoveNodeConnection( this->GetName() );
	}

	m_RenderObject->DeInit();
	delete m_RenderObject;
	m_RenderObject = NULL;
}

IComponent *CNodeConnection::Create(int nArgs, va_list argptr)
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
		self = new CNodeConnection( parentName, name );
	}
	catch(...)
	{
		return NULL;
	}

	return self;
}

void CNodeConnection::Serialize( IArchive &ar )
{
	if( ar.IsReading() )
	{
		float fVersion;
		StdString tempStr;
		ar.Read( fVersion, _T("Version") );
		ar.Read( tempStr, _T("Node1") );
		CHashString tempHash( tempStr );
		SetBeginNode( &tempHash );
		ar.Read( tempStr, _T("Node2") );
		tempHash = tempStr.c_str();
		SetEndNode( &tempHash );
	}
	else
	{
		ar.Write( m_fVersion, _T("Version"));
		ar.Write( m_hszNode1.GetString(), _T("Node1") );
		ar.Write( m_hszNode2.GetString(), _T("Node2") );
	}
}

Vec3 &CNodeConnection::GetNode1Position()
{
	if( m_Node1 == NULL )
	{
		return GetPosition();
	}
	return m_Node1->GetPosition();
}

Vec3 &CNodeConnection::GetNode2Position()
{
	if( m_Node2 == NULL )
	{
		return GetPosition();
	}
	return m_Node2->GetPosition();
}

IHashString *CNodeConnection::GetNode1Name()
{
	return &m_hszNode1;
}

IHashString *CNodeConnection::GetNode2Name()
{
	return &m_hszNode2;
}

DWORD CNodeConnection::OnGetSchema( DWORD size, void *params )
{
	VERIFY_MESSAGE_SIZE(size, sizeof(GETSCHEMAPARAMS));
	GETSCHEMAPARAMS *getSchemaParams = (GETSCHEMAPARAMS*)params;
	IXMLArchive *archive = getSchemaParams->ar;
	if( archive != NULL )
	{
		int nodesMapCount = 0;
		IDTOOBJECTMAP *nodesMap = NULL;
		if( m_NodeSystem != NULL )
		{
			nodesMap = m_NodeSystem->GetNodesMap();
			nodesMapCount = (int)nodesMap->size();
		}

		IDTOOBJECTMAP::iterator nodesMapIterator;

		TCHAR szComboNum[] = _T("ComboItem_XXXX");
		LPTSTR szNum = szComboNum + sizeof(szComboNum) / sizeof(*szComboNum) - 4;

		archive->StartClass( GetComponentType()->GetString() );
		archive->Write( m_Name.GetString(), _T("Name") );
		archive->Write( 3 );

		archive->StartClass(_T("hidden") );
		archive->Write( _T("float"), _T("Type") );
		archive->EndClass();

		archive->StartClass( _T("combo") );
		archive->Write( _T("string"), _T("Type") );
		archive->Write( _T("Node 1"), _T("Name") );
		archive->Write( false, _T("Key") );
		archive->Write( _T(""), _T("DefSelect") );
		archive->Write( nodesMapCount, _T("Count") );
		int comboItemCounter = 0;
		if( nodesMap != NULL )
		{
			for( nodesMapIterator = nodesMap->begin(); nodesMapIterator != nodesMap->end(); nodesMapIterator++ )
			{
				_ltot( ++comboItemCounter, szNum, 10 );
				CNode *iteratedNode = (CNode*)nodesMapIterator->second;
				archive->Write( iteratedNode->GetName()->GetString(), szComboNum );
			}
		}
		archive->EndClass();

		archive->StartClass( _T("combo") );
		archive->Write( _T("string"), _T("Type") );
		archive->Write( _T("Node 2"), _T("Name") );
		archive->Write( false, _T("Key") );
		archive->Write( _T(""), _T("DefSelect") );
		archive->Write( nodesMapCount, _T("Count") );
		comboItemCounter = 0;
		if( nodesMap != NULL )
		{
			for( nodesMapIterator = nodesMap->begin(); nodesMapIterator != nodesMap->end(); nodesMapIterator++ )
			{
				_ltot( ++comboItemCounter, szNum, 10 );
				CNode *iteratedNode = (CNode*)nodesMapIterator->second;
				archive->Write( iteratedNode->GetName()->GetString(), szComboNum );
			}
		}
		archive->EndClass();

		archive->EndClass();
	}
	else
	{
		m_ToolBox->Log( LOGERROR, _T("Attempt to get CNodeSystem schema. No archive specified.\n" ) );
	}

	return MSG_HANDLED_STOP;
}

CNodeSystemManager *CNodeConnection::GetManager()
{
	IHashString *managerName = GetManagerName();
	IComponent *amanagerComponent = m_ToolBox->GetComponent( managerName );
	CNodeSystemManager *amanager = dynamic_cast<CNodeSystemManager*>( amanagerComponent );

	return amanager;
}

void CNodeConnection::SetBeginNode( IHashString *hszBeginNode )
{
	if( hszBeginNode != NULL )
	{
		CNodeSystemManager *nodeSystemManager = GetManager();

		CNode *tempNode = nodeSystemManager->GetNode( hszBeginNode );
		if( tempNode == NULL ) // Fail-safe default if we cannot find the specified node
		{
			tempNode = (CNode*)m_NodeSystem->GetNodesMap()->begin()->second;
		}
		if( m_Node1 != NULL )
		{
			m_Node1->DeregisterConnection( this );
		}

		m_Node1 = tempNode;

		if( m_Node1 != NULL )
		{
			m_Node1->RegisterConnection( this );
		}
		if( tempNode != NULL )
		{
			m_hszNode1 = m_Node1->GetName();
		}
		else
		{
			m_hszNode1 = _T("");
		}
	}
}

void CNodeConnection::SetEndNode( IHashString *hszEndNode )
{
	if( hszEndNode != NULL )
	{
		CNodeSystemManager *nodeSystemManager = GetManager();

		CNode *tempNode = nodeSystemManager->GetNode( hszEndNode );
		if( tempNode == NULL ) // Fail-safe default if we cannot find the specified node
		{
			tempNode = (CNode*)m_NodeSystem->GetNodesMap()->begin()->second;
		}
		if( m_Node2 != NULL )
		{
			m_Node2->DeregisterConnection( this );
		}

		m_Node2 = tempNode;

		if( m_Node2 != NULL )
		{
			m_Node2->RegisterConnection( this );
		}
		if( tempNode != NULL )
		{
			m_hszNode1 = m_Node2->GetName();
		}
		else
		{
			m_hszNode1 = _T("");
		}
	}
}