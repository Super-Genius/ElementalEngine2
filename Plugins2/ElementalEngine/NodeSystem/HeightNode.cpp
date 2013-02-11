///============================================================================
/// \file		HeightNode.cpp
/// \brief		Implmentation of CHeightNode Class
/// \date		08-08-2007
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

REGISTER_COMPONENT(CHeightNode);
REGISTER_MESSAGE_HANDLER( GetGlobalPosition, OnGetGlobalPosition, CHeightNode );
REGISTER_MESSAGE_HANDLER( SetGlobalPosition, OnSetGlobalPosition, CHeightNode );
REGISTER_MESSAGE_HANDLER( GetSchema, OnGetSchema, CHeightNode );

CHeightNode::CHeightNode( IHashString *parentName, IHashString *name ) :
	m_hszLabel(_T("")),
	m_hszConnectedHeightNode(_T("")),
	OBJECTTEMPLATE( CNodeSystemManager, CHeightNode, IObject, parentName, name )
{
	m_ToolBox = EngineGetToolBox();

	m_fVersion = 1.0f;
	m_hszLabel = name;
	m_fHeight = 1.0f;

	m_ConnectedHeightNode = NULL;
	m_NodeSystem = NULL;
	m_RenderObject = NULL;
	if( parentName != NULL )
	{
		CNodeSystemManager *amanager = GetManager();
		m_NodeSystem = amanager->GetNodeSystem( parentName );
	}

	AddToHierarchy();
}

CHeightNode::~CHeightNode()
{
	RemoveFromHierarchy();
}

void CHeightNode::Init()
{
	IHashString *parentName = GetParentName();
	if( parentName != NULL )
	{
		CNodeSystemManager *amanager = GetManager();
		CNodeSystem *parentNodeSystem = amanager->GetNodeSystem( parentName );
		if( parentNodeSystem != NULL )
		{
			parentNodeSystem->AddHeightNode( this );
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

	CHashString hszHeightNodeName = GetName();
	StdString szHeightNodeName = hszHeightNodeName.GetString();

	StdString szHeightNodeRenderObjectName = szHeightNodeName + _T("_RenderObject");
	CHashString hszHeightNodeRenderObjectName(szHeightNodeRenderObjectName.c_str());
	CHashString hszHeightNodeRenderObjectComponentType(_T("CHeightNodeRenderObject"));

	//TODO: Fix this so that the RenderObject DOESN'T get slammed into the quad tree in the actual game 
	if( m_RenderObject == NULL )
	{
		IHashString *managerName = GetManagerName();
		IComponent *amanagerComponent = m_ToolBox->GetComponent( managerName );
		CNodeSystemManager *amanager = dynamic_cast<CNodeSystemManager*>( amanagerComponent );
		bool bRender = amanager->IsRendering();
		if( bRender )
		{
			IComponent *renderObjectComponent = m_ToolBox->CreateComponent( &hszHeightNodeRenderObjectComponentType, 2, &hszHeightNodeRenderObjectName, &hszHeightNodeName );
			m_RenderObject = dynamic_cast<CHeightNodeRenderObject*>(renderObjectComponent);
			m_RenderObject->Init();
		}
	}

}

bool CHeightNode::Update()
{
	return true;
}

void CHeightNode::DeInit()
{
	IComponent *amanagerComponent = m_ToolBox->GetComponent( GetManagerName() );
	CNodeSystemManager *amanager = dynamic_cast<CNodeSystemManager*>( amanagerComponent );

	CNodeSystem *parentNodeSystem = amanager->GetNodeSystem( GetParentName() );
	if( parentNodeSystem != NULL )
	{
		parentNodeSystem->RemoveHeightNode( this->GetName() );
	}

	if (m_RenderObject)
	{
		m_RenderObject->DeInit();
		delete m_RenderObject;
		m_RenderObject = NULL;
	}
}

IComponent *CHeightNode::Create(int nArgs, va_list argptr)
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
		self = new CHeightNode( parentName, name );
	}
	catch(...)
	{
		return NULL;
	}

	return self;
}

void CHeightNode::Serialize( IArchive &ar )
{
	if( ar.IsReading() )
	{
		float fVersion;
		StdString tempStr;
		ar.Read( fVersion, _T("Version") );
		ar.Read( m_vPosition.x, _T("X") );
		ar.Read( m_vPosition.y, _T("Y") );
		ar.Read( m_vPosition.z, _T("Z") );
		ar.Read( m_fHeight, _T("Height") );
		ar.Read( tempStr, _T("ConnectedHeightNode") );
		m_hszConnectedHeightNode = tempStr;

		CNodeSystemManager *nodeSystemManager = GetManager();
		if( nodeSystemManager != NULL )
		{
			m_ConnectedHeightNode = nodeSystemManager->GetHeightNode( &m_hszConnectedHeightNode );
		}
		else
		{
			m_ConnectedHeightNode = NULL;
		}
	}
	else
	{
		ar.Write( m_fVersion, _T("Version"));
		ar.Write( m_vPosition.x, _T("X") );
		ar.Write( m_vPosition.y, _T("Y") );
		ar.Write( m_vPosition.z, _T("Z") );
		ar.Write( m_fHeight, _T("Height") );
		ar.Write( m_hszConnectedHeightNode.GetString(), _T("ConnectedHeightNode") );
	}
}

CHeightNode *CHeightNode::GetConnectedHeightNode()
{
	CNodeSystemManager *nodeSystemManager = GetManager();
	if( nodeSystemManager != NULL )
	{
		m_ConnectedHeightNode = nodeSystemManager->GetHeightNode( &m_hszConnectedHeightNode );
	}
	else
	{
		m_ConnectedHeightNode = NULL;
	}
	return m_ConnectedHeightNode;
}

float CHeightNode::GetHeight()
{
	return m_fHeight;
}

void CHeightNode::GetLocalPosition( Vec3 *v )
{
	if( v != NULL )
	{
		v->Set( m_vPosition );
	}
}

void CHeightNode::GetGlobalPosition( Vec3 *v )
{
	if( v != NULL )
	{
		v->Set( m_NodeSystem->GetPosition() );
		(*v) += m_vPosition;
	}
}

DWORD CHeightNode::OnGetGlobalPosition( DWORD size, void *params )
{
	VERIFY_MESSAGE_SIZE( size, sizeof(Vec3) );
	Vec3 *vPosition = (Vec3*)params;
	GetGlobalPosition( vPosition );

	return MSG_HANDLED_STOP;
}

DWORD CHeightNode::OnSetGlobalPosition( DWORD size, void *params )
{
	VERIFY_MESSAGE_SIZE( size, sizeof(Vec3) );
	Vec3 *vPosition = (Vec3*)params;
	m_vPosition = (*vPosition);
	m_vPosition -= m_NodeSystem->GetPosition();

	return MSG_HANDLED_STOP;
}

DWORD CHeightNode::OnGetSchema( DWORD size, void *params )
{
	VERIFY_MESSAGE_SIZE(size, sizeof(GETSCHEMAPARAMS));
	GETSCHEMAPARAMS *getSchemaParams = (GETSCHEMAPARAMS*)params;
	IXMLArchive *archive = getSchemaParams->ar;
	if( archive != NULL )
	{
		archive->StartClass( GetComponentType()->GetString() );
		archive->Write( m_Name.GetString(), _T("Name") );
		archive->Write( 6 );

		archive->StartClass(_T("hidden") );
		archive->Write( _T("float"), _T("Type") );
		archive->EndClass();

		archive->StartClass( _T("edit") );
		archive->Write( _T("float"), _T("Type") );
		archive->Write( _T("X"), _T("Name") );
		archive->Write( false, _T("Key") );
		archive->EndClass();

		archive->StartClass( _T("edit") );
		archive->Write( _T("float"), _T("Type") );
		archive->Write( _T("Y"), _T("Name") );
		archive->Write( false, _T("Key") );
		archive->EndClass();

		archive->StartClass( _T("edit") );
		archive->Write( _T("float"), _T("Type") );
		archive->Write( _T("Z"), _T("Name") );
		archive->Write( false, _T("Key") );
		archive->EndClass();

		archive->StartClass( _T("edit") );
		archive->Write( _T("float"), _T("Type") );
		archive->Write( _T("Height"), _T("Name") );
		archive->Write( false, _T("Key") );
		archive->EndClass();

		IDTOOBJECTMAP *nodesMap = m_NodeSystem->GetHeightNodesMap();
		int nodesMapCount = (int)nodesMap->size();
		IDTOOBJECTMAP::iterator nodesMapIterator;
		int comboItemCounter = 0;

		TCHAR szComboNum[] = _T("ComboItem_XXXX");
		LPTSTR szNum = szComboNum + sizeof(szComboNum) / sizeof(*szComboNum) - 4;

		archive->StartClass( _T("combo") );
		archive->Write( _T("string"), _T("Type") );
		archive->Write( _T("Connected Height Node"), _T("Name") );
		archive->Write( false, _T("Key") );
		archive->Write( _T(""), _T("DefSelect") );
		archive->Write( nodesMapCount, _T("Count") );
		comboItemCounter = 0;
		for( nodesMapIterator = nodesMap->begin(); nodesMapIterator != nodesMap->end(); nodesMapIterator++ )
		{
			_ltot( ++comboItemCounter, szNum, 10 );
			CHeightNode *iteratedNode = (CHeightNode*)nodesMapIterator->second;
			archive->Write( iteratedNode->GetName()->GetString(), szComboNum );
		}
		archive->EndClass();

		archive->EndClass();
	}
	else
	{
		m_ToolBox->Log( LOGERROR, _T("Attempt to get CHeightNode schema. No archive specified.\n" ) );
	}

	return MSG_HANDLED_STOP;
}

CNodeSystemManager *CHeightNode::GetManager()
{
	IHashString *managerName = GetManagerName();
	IComponent *amanagerComponent = m_ToolBox->GetComponent( managerName );
	CNodeSystemManager *amanager = dynamic_cast<CNodeSystemManager*>( amanagerComponent );

	return amanager;
}