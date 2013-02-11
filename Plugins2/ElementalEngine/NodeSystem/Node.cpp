///============================================================================
/// \file		Node.cpp
/// \brief		Implmentation of CNode Class
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

REGISTER_COMPONENT(CNode);
REGISTER_MESSAGE_HANDLER( GetGlobalPosition, OnGetGlobalPosition, CNode );
REGISTER_MESSAGE_HANDLER( SetGlobalPosition, OnSetGlobalPosition, CNode );
REGISTER_MESSAGE_HANDLER( GetSchema, OnGetSchema, CNode );

CNode::CNode( IHashString *parentName, IHashString *name ) :
	m_hszLabel( _T("") ),
	m_hszNodeSystemComponentType( _T("CNodeSystem") ),
	OBJECTTEMPLATE( CNodeSystemManager, CNode, IObject, parentName, name )
{
	m_ToolBox = EngineGetToolBox();

	m_hszLabel = name;
	m_fVersion = 1.0f;

	m_NodeSystem = NULL;
	m_RenderObject = NULL;
	if( parentName != NULL )
	{
		CNodeSystemManager *amanager = GetManager();
		m_NodeSystem = amanager->GetNodeSystem( parentName );
	}

	AddToHierarchy();
}

CNode::~CNode()
{
	RemoveFromHierarchy();
}

void CNode::Init()
{
	m_NodeSystem = NULL;
	IHashString *parentName = GetParentName();
	if( parentName != NULL )
	{
		CNodeSystemManager *amanager = GetManager();
		m_NodeSystem = amanager->GetNodeSystem( parentName );
		if( m_NodeSystem != NULL )
		{
			m_NodeSystem->AddNode( this );
		}
		else
		{
			m_ToolBox->Log( LOGERROR, _T("Could not find parent object %s for node %s.\n"), parentName->GetString(), GetName()->GetString() );
		}
	}
	else
	{
		m_ToolBox->Log( LOGWARNING, _T("Parent object not specified for node %s.\n"), GetName()->GetString() );
	}

	CHashString hszNodeName = GetName();
	StdString szNodeName = hszNodeName.GetString();

	StdString szNodeRenderObjectName = szNodeName + _T("_RenderObject");
	CHashString hszNodeRenderObjectName(szNodeRenderObjectName.c_str());
	static CHashString hszNodeRenderObjectComponentType(_T("CNodeRenderObject"));

	if( m_RenderObject == NULL )
	{
		IComponent *renderObjectComponent = m_ToolBox->CreateComponent( &hszNodeRenderObjectComponentType, 2, &hszNodeRenderObjectName, &hszNodeName );
		m_RenderObject = dynamic_cast<CNodeRenderObject*>(renderObjectComponent);
		m_RenderObject->Init();
	}
}

bool CNode::Update()
{
	return true;
}

void CNode::DeInit()
{
	IComponent *amanagerComponent = m_ToolBox->GetComponent( GetManagerName() );
	CNodeSystemManager *amanager = dynamic_cast<CNodeSystemManager*>( amanagerComponent );

	CNodeSystem *parentNodeSystem = amanager->GetNodeSystem( GetParentName() );
	if( parentNodeSystem != NULL )
	{
		parentNodeSystem->RemoveNode( this->GetName() );
	}

	if( m_RenderObject != NULL )
	{
		m_RenderObject->DeInit();
		delete m_RenderObject;
		m_RenderObject = NULL;
	}

	std::list<CNodeConnection*>::iterator connectionsIterator = m_ConnectionList.begin();
	for( ; connectionsIterator != m_ConnectionList.end(); connectionsIterator++ )
	{
		CNodeConnection *iteratedConnection = *connectionsIterator;
		if( connectionsIterator == m_ConnectionList.begin() )
		{
			m_ConnectionList.remove( iteratedConnection );
			connectionsIterator = m_ConnectionList.begin();
		}
		else
		{
			connectionsIterator--;
			m_ConnectionList.remove( iteratedConnection );
		}
		

		DELETEOBJECTPARAMS deleteObjectParams;
		deleteObjectParams.name = iteratedConnection->GetName();
		static DWORD msgHash_DeleteObject = CHashString(_T("DeleteObject")).GetUniqueID();
		DWORD retVal = m_ToolBox->SendMessage(msgHash_DeleteObject, sizeof(DELETEOBJECTPARAMS), &deleteObjectParams );
	}
}

IComponent *CNode::Create(int nArgs, va_list argptr)
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
		self = new CNode( parentName, name );
	}
	catch(...)
	{
		return NULL;
	}

	return self;
}

void CNode::Serialize( IArchive &ar )
{
	if( ar.IsReading() )
	{
		float fVersion;

		ar.Read( fVersion, _T("Version"));
		ar.Read( m_vPosition.x, _T("X") );
		ar.Read( m_vPosition.y, _T("Y") );
		ar.Read( m_vPosition.z, _T("Z") );
	}
	else
	{
		ar.Write( m_fVersion, _T("Version"));
		ar.Write( m_vPosition.x, _T("X") );
		ar.Write( m_vPosition.y, _T("Y") );
		ar.Write( m_vPosition.z, _T("Z") );
	}
}

void CNode::GetLocalPosition( Vec3 *v )
{
	if( v != NULL )
	{
		v->Set( m_vPosition );
	}
}

void CNode::GetGlobalPosition( Vec3 *v )
{
	if( v != NULL )
	{
		if( m_NodeSystem!= NULL )
		{
			v->Set( m_NodeSystem->GetPosition() );
		}
		else
		{
			v->Set( 0, 0, 0 );
		}
		(*v) += m_vPosition;
	}
}

void CNode::RegisterConnection( CNodeConnection *nodeConnection )
{
	m_ConnectionList.remove( nodeConnection ); // Prevent duplicates
	m_ConnectionList.push_back( nodeConnection );
}

void CNode::DeregisterConnection( CNodeConnection *nodeConnection )
{
	m_ConnectionList.remove( nodeConnection );
}

DWORD CNode::OnGetGlobalPosition( DWORD size, void *params )
{
	VERIFY_MESSAGE_SIZE( size, sizeof(Vec3) );
	Vec3 *vPosition = (Vec3*)params;
	GetGlobalPosition( vPosition );

	return MSG_HANDLED_STOP;
}

DWORD CNode::OnSetGlobalPosition( DWORD size, void *params )
{
	VERIFY_MESSAGE_SIZE( size, sizeof(Vec3) );
	Vec3 *vPosition = (Vec3*)params;
	m_vPosition = (*vPosition);

	if( m_NodeSystem != NULL )
	{
		m_vPosition -= m_NodeSystem->GetPosition();
	}

	return MSG_HANDLED_STOP;
}

DWORD CNode::OnGetSchema( DWORD size, void *params )
{
	VERIFY_MESSAGE_SIZE(size, sizeof(GETSCHEMAPARAMS));
	GETSCHEMAPARAMS *getSchemaParams = (GETSCHEMAPARAMS*)params;
	IXMLArchive *archive = getSchemaParams->ar;
	if( archive != NULL )
	{
		archive->StartClass( GetComponentType()->GetString() );
		archive->Write( m_Name.GetString(), _T("Name") );
		archive->Write( 4 );

		archive->StartClass(_T("hidden") );
		archive->Write( _T("float"), _T("Type") );
		archive->EndClass();

		archive->StartClass( _T("static") );
		archive->Write( _T("float"), _T("Type") );
		archive->Write( _T("X"), _T("Name") );
		archive->Write( false, _T("Key") );
		archive->EndClass();

		archive->StartClass( _T("static") );
		archive->Write( _T("float"), _T("Type") );
		archive->Write( _T("Y"), _T("Name") );
		archive->Write( false, _T("Key") );
		archive->EndClass();

		archive->StartClass( _T("static") );
		archive->Write( _T("float"), _T("Type") );
		archive->Write( _T("Z"), _T("Name") );
		archive->Write( false, _T("Key") );
		archive->EndClass();

		archive->EndClass();
	}
	else
	{
		m_ToolBox->Log( LOGERROR, _T("Attempt to get CNode schema. No archive specified.\n" ) );
	}

	return MSG_HANDLED_STOP;
}

CNodeSystemManager *CNode::GetManager()
{
	IHashString *managerName = GetManagerName();
	IComponent *amanagerComponent = m_ToolBox->GetComponent( managerName );
	CNodeSystemManager *amanager = dynamic_cast<CNodeSystemManager*>( amanagerComponent );

	return amanager;
}