///============================================================================
/// \file		NodeFace.cpp
/// \brief		Implmentation of CNodeFace Class
/// \date		07-30-2007
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

REGISTER_COMPONENT(CNodeFace);
REGISTER_MESSAGE_HANDLER( GetSchema, OnGetSchema, CNodeFace );

CNodeFace::CNodeFace( IHashString *parentName, IHashString *name ) :
	m_hszLabel(_T("")),
	m_hszNode1(_T("")),
	m_hszNode2(_T("")),
	m_hszNode3(_T("")),
	OBJECTTEMPLATE( CNodeSystemManager, CNodeFace, IObject, parentName, name )
{
	m_ToolBox = EngineGetToolBox();

	m_fVersion = 1.0f;
	m_hszLabel = name;

	m_NodeSystem = NULL;
	m_RenderObject = NULL;
	if( parentName != NULL )
	{
		CNodeSystemManager *amanager = GetManager();
		m_NodeSystem = amanager->GetNodeSystem( parentName );
	}

	AddToHierarchy();
}

CNodeFace::~CNodeFace()
{
	RemoveFromHierarchy();
}

void CNodeFace::Init()
{
	IHashString *parentName = GetParentName();
	if( parentName != NULL )
	{
		CNodeSystemManager *amanager = GetManager();
		CNodeSystem *parentNodeSystem = amanager->GetNodeSystem( parentName );
		if( parentNodeSystem != NULL )
		{
			parentNodeSystem->AddNodeFace( this );
		}
		else
		{
			m_ToolBox->Log( LOGERROR, _T("Could not find parent object %s for node Face %s."), parentName->GetString(), GetName()->GetString() );
		}
	}
	else
	{
		m_ToolBox->Log( LOGWARNING, _T("Parent object not specified for node Face %s."), GetName()->GetString() );
	}

	CHashString hszNodeFaceName = GetName();
	StdString szNodeFaceName = hszNodeFaceName.GetString();

	StdString szNodeFaceRenderObjectName = szNodeFaceName + _T("_RenderObject");
	CHashString hszNodeFaceRenderObjectName(szNodeFaceRenderObjectName.c_str());
	static CHashString hszNodeFaceRenderObjectComponentType(_T("CNodeFaceRenderObject"));

	if( m_RenderObject == NULL )
	{
		IComponent *renderObjectComponent = m_ToolBox->CreateComponent( &hszNodeFaceRenderObjectComponentType, 2, &hszNodeFaceRenderObjectName, &hszNodeFaceName );
		m_RenderObject = dynamic_cast<CNodeFaceRenderObject*>(renderObjectComponent);
		m_RenderObject->Init();
	}
}

bool CNodeFace::Update()
{
	return true;
}

void CNodeFace::DeInit()
{
	IComponent *amanagerComponent = m_ToolBox->GetComponent( GetManagerName() );
	CNodeSystemManager *amanager = dynamic_cast<CNodeSystemManager*>( amanagerComponent );

	CNodeSystem *parentNodeSystem = amanager->GetNodeSystem( GetParentName() );
	if( parentNodeSystem != NULL )
	{
		parentNodeSystem->RemoveNodeFace( this->GetName() );
	}

	m_RenderObject->DeInit();
	delete m_RenderObject;
	m_RenderObject = NULL;
}

IComponent *CNodeFace::Create(int nArgs, va_list argptr)
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
		self = new CNodeFace( parentName, name );
	}
	catch(...)
	{
		return NULL;
	}

	return self;
}

void CNodeFace::Serialize( IArchive &ar )
{
	if( ar.IsReading() )
	{
		float fVersion;
		StdString tempStr;
		ar.Read( fVersion, _T("Version") );
		ar.Read( tempStr, _T("Node1") );
		m_hszNode1 = tempStr;
		ar.Read( tempStr, _T("Node2") );
		m_hszNode2 = tempStr;
		ar.Read( tempStr, _T("Node3") );
		m_hszNode3 = tempStr;

		CNodeSystemManager *nodeSystemManager = GetManager();
		m_Node1 = nodeSystemManager->GetNode( &m_hszNode1 );
		m_Node2 = nodeSystemManager->GetNode( &m_hszNode2 );
		m_Node3 = nodeSystemManager->GetNode( &m_hszNode3 );

		if( m_Node1 == NULL )
		{
			m_Node1 = (CNode*)m_NodeSystem->GetNodesMap()->begin()->second;
		}
		if( m_Node2 == NULL )
		{
			m_Node2 = (CNode*)m_NodeSystem->GetNodesMap()->begin()->second;
		}
		if( m_Node3 == NULL )
		{
			m_Node3 = (CNode*)m_NodeSystem->GetNodesMap()->begin()->second;
		}
	}
	else
	{
		ar.Write( m_fVersion, _T("Version"));
		ar.Write( m_hszNode1.GetString(), _T("Node1") );
		ar.Write( m_hszNode2.GetString(), _T("Node2") );
		ar.Write( m_hszNode3.GetString(), _T("Node3") );
	}
}

Vec3 &CNodeFace::GetNode1Position()
{
	if( m_Node1 == NULL )
	{
		return GetPosition();
	}
	return m_Node1->GetPosition();
}

Vec3 &CNodeFace::GetNode2Position()
{
	if( m_Node2 == NULL )
	{
		return GetPosition();
	}
	return m_Node2->GetPosition();
}

Vec3 &CNodeFace::GetNode3Position()
{
	if( m_Node3 == NULL )
	{
		return GetPosition();
	}
	return m_Node3->GetPosition();
}

IHashString *CNodeFace::GetNode1Name()
{
	return &m_hszNode1;
}

IHashString *CNodeFace::GetNode2Name()
{
	return &m_hszNode2;
}

IHashString *CNodeFace::GetNode3Name()
{
	return &m_hszNode3;
}

DWORD CNodeFace::OnGetSchema( DWORD size, void *params )
{
	VERIFY_MESSAGE_SIZE(size, sizeof(GETSCHEMAPARAMS));
	GETSCHEMAPARAMS *getSchemaParams = (GETSCHEMAPARAMS*)params;
	IXMLArchive *archive = getSchemaParams->ar;
	if( archive != NULL )
	{
		IDTOOBJECTMAP *nodesMap = m_NodeSystem->GetNodesMap();
		int nodesMapCount = (int)nodesMap->size();
		IDTOOBJECTMAP::iterator nodesMapIterator;

		TCHAR szComboNum[] = _T("ComboItem_XXXX");
		LPTSTR szNum = szComboNum + sizeof(szComboNum) / sizeof(*szComboNum) - 4;

		archive->StartClass( GetComponentType()->GetString() );
		archive->Write( m_Name.GetString(), _T("Name") );
		archive->Write( 4 );

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
		for( nodesMapIterator = nodesMap->begin(); nodesMapIterator != nodesMap->end(); nodesMapIterator++ )
		{
			_ltot( ++comboItemCounter, szNum, 10 );
			CNode *iteratedNode = (CNode*)nodesMapIterator->second;
			archive->Write( iteratedNode->GetName()->GetString(), szComboNum );
		}
		archive->EndClass();

		archive->StartClass( _T("combo") );
		archive->Write( _T("string"), _T("Type") );
		archive->Write( _T("Node 2"), _T("Name") );
		archive->Write( false, _T("Key") );
		archive->Write( _T(""), _T("DefSelect") );
		archive->Write( nodesMapCount, _T("Count") );
		comboItemCounter = 0;
		for( nodesMapIterator = nodesMap->begin(); nodesMapIterator != nodesMap->end(); nodesMapIterator++ )
		{
			_ltot( ++comboItemCounter, szNum, 10 );
			CNode *iteratedNode = (CNode*)nodesMapIterator->second;
			archive->Write( iteratedNode->GetName()->GetString(), szComboNum );
		}
		archive->EndClass();

		archive->StartClass( _T("combo") );
		archive->Write( _T("string"), _T("Type") );
		archive->Write( _T("Node 3"), _T("Name") );
		archive->Write( false, _T("Key") );
		archive->Write( _T(""), _T("DefSelect") );
		archive->Write( nodesMapCount, _T("Count") );
		comboItemCounter = 0;
		for( nodesMapIterator = nodesMap->begin(); nodesMapIterator != nodesMap->end(); nodesMapIterator++ )
		{
			_ltot( ++comboItemCounter, szNum, 10 );
			CNode *iteratedNode = (CNode*)nodesMapIterator->second;
			archive->Write( iteratedNode->GetName()->GetString(), szComboNum );
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

CNodeSystemManager *CNodeFace::GetManager()
{
	IHashString *managerName = GetManagerName();
	IComponent *amanagerComponent = m_ToolBox->GetComponent( managerName );
	CNodeSystemManager *amanager = dynamic_cast<CNodeSystemManager*>( amanagerComponent );

	return amanager;
}