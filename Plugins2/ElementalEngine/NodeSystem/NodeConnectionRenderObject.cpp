///============================================================================
/// \file		NodeConnectionRenderObject.cpp
/// \brief		Implmentation of CNodeConnectionRenderObject Class
/// \date		07-18-2007
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

REGISTER_COMPONENT(CNodeConnectionRenderObject);

CNodeConnectionRenderObject::CNodeConnectionRenderObject(IHashString *parentName, IHashString *name) :
	m_hszComponentType( _T("CNodeConnectionRenderObject") ),
	SCENEOBJECTTEMPLATE( CNodeSystemManager, CNodeConnectionRenderObject, IRenderObject, parentName, name )
{
	m_ToolBox = EngineGetToolBox();
}

CNodeConnectionRenderObject::~CNodeConnectionRenderObject()
{
}

IComponent *CNodeConnectionRenderObject::Create(int nArgs, va_list argptr)
{
	IHashString *name;
	IHashString *parentName;
	IObject *self;

	name = va_arg(argptr, IHashString *);
	parentName = va_arg(argptr, IHashString *);
	// create the object however you must create it
	try
	{
		self = new CNodeConnectionRenderObject( parentName, name );
	}
	catch(...)
	{
		return NULL;
	}

	return self;
}

void CNodeConnectionRenderObject::Init()
{
	GETRENDERERINTERFACEPARAMS renderInterfaceParam;
	static DWORD msgHash_GetRendererInterface = CHashString(_T("GetRendererInterface")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_GetRendererInterface, sizeof(GETRENDERERINTERFACEPARAMS), &renderInterfaceParam, NULL, NULL );
	m_IRenderer = renderInterfaceParam.m_RendererInterface;

	UpdateGlobalObject();

	m_NodeConnection = NULL;
	m_NodeSystem = NULL;
	IHashString *parentName = GetParentName();
	if( parentName != NULL )
	{
		DWORD parentID = parentName->GetUniqueID();
		static CHashString hszNodeSystemManager( _T("CNodeSystemManager") );
		CNodeSystemManager *manager = dynamic_cast<CNodeSystemManager*>( m_ToolBox->GetComponent( &hszNodeSystemManager ) );
		if( manager != NULL )
		{
			static CHashString hszNodeComponentType( _T("CNodeConnection") );
			IDTOOBJECTMAP *nodeObjectMap = manager->GetObjectMap( &hszNodeComponentType );
			if( nodeObjectMap != NULL && nodeObjectMap->find(parentID) != nodeObjectMap->end() )
			{
				IObject *nodeObject = (*nodeObjectMap)[parentID];
				m_NodeConnection = dynamic_cast<CNodeConnection*>(nodeObject);

				parentID = m_NodeConnection->GetParentName()->GetUniqueID();
				static CHashString hszNodeSystemComponentType( _T("CNodeSystem") );
				IDTOOBJECTMAP *nodeSystemObjectMap = manager->GetObjectMap( &hszNodeSystemComponentType );
				if( nodeSystemObjectMap != NULL && nodeSystemObjectMap->find(parentID) != nodeSystemObjectMap->end() )
				{
					IObject *nodeSystemObject = (*nodeSystemObjectMap)[parentID];
					m_NodeSystem = dynamic_cast<CNodeSystem*>(nodeSystemObject);
				}
			}
			else
			{
				m_ToolBox->Log( LOGERROR, _T("Could not find object map for CNodeConnection.\n") );
			}
		}
		else
		{
			m_ToolBox->Log( LOGERROR, _T("CNodeSystemManager instance could not be found.\n") );
		}
	}
	else
	{
		m_ToolBox->Log( LOGERROR, _T("Parent not specified for node render object %s.\n"), GetName()->GetString() );
	}
}

void CNodeConnectionRenderObject::DeInit()
{
	RemoveBoundingObject();
}

void CNodeConnectionRenderObject::Serialize( IArchive &ar )
{
}

IHashString *CNodeConnectionRenderObject::GetComponentType()
{
	return &m_hszComponentType;
}

bool CNodeConnectionRenderObject::IsKindOf(IHashString *compType)
{
	bool retVal = false;
	if( compType != NULL )
	{
		retVal = m_hszComponentType.GetUniqueID() == compType->GetUniqueID();
	}

	return retVal;
}

bool CNodeConnectionRenderObject::Render( UINT pass, IEffect * override )
{
	IHashString *managerName = GetManagerName();
	IComponent *amanagerComponent = m_ToolBox->GetComponent( managerName );
	CNodeSystemManager *amanager = dynamic_cast<CNodeSystemManager*>( amanagerComponent );
	bool bRender = amanager->IsRendering();

	if( m_IRenderer != NULL && bRender )
	{
		Vec3 vGlobalPosition( 0.0f, 0.0f, 0.0f );
		if( m_NodeSystem != NULL )
		{
			vGlobalPosition = m_NodeSystem->GetPosition();
		}

		Vec3 node1Position = m_NodeConnection->GetNode1Position() + vGlobalPosition;
		Vec3 node2Position = m_NodeConnection->GetNode2Position() + vGlobalPosition;

		float size = 1;
		int r = 255;
		int g = 255;
		int b = 255;

		if( m_NodeSystem != NULL )
		{
			size = m_NodeSystem->GetLineSize();
			r = m_NodeSystem->GetLineR();
			g = m_NodeSystem->GetLineG();
			b = m_NodeSystem->GetLineB();
		}

		m_IRenderer->Draw3DLine(
			node1Position.x,
			node1Position.y,
			node1Position.z,
			node2Position.x,
			node2Position.y,
			node2Position.z,
			r, g, b, size );
	}

	return true;
}

bool CNodeConnectionRenderObject::IsLessThan( const IRenderObject * obj, DWORD sortMethod, int curRenderPass )
{
	return true;
}

DWORD CNodeConnectionRenderObject::GetRenderPriority()
{
	return 0;
}

UINT CNodeConnectionRenderObject::GetNumPass()
{
	return 1;
}

void CNodeConnectionRenderObject::SetPosition( const Vec3 &position )
{
	m_vPosition.Set( position );
}

Vec3 &CNodeConnectionRenderObject::GetPosition()
{
	return m_vPosition;
}

IHashString *CNodeConnectionRenderObject::GetBaseSortClass()
{
	return GetComponentType();
}

bool CNodeConnectionRenderObject::IsAlphaObject()
{
	return false;
}

IMeshObject *CNodeConnectionRenderObject::GetMeshObject()
{
	return NULL;
}

UINT CNodeConnectionRenderObject::GetNumGeometry()
{
	return 0;
}

IGeometry *CNodeConnectionRenderObject::GetGeometry( int index ){ return NULL; }

bool CNodeConnectionRenderObject::GetBoundingBox( Vec3 &pos, Vec3 &dim, EulerAngle &rotation )
{
	pos.Set( m_vPosition );

	dim.Set( 0.0f, 0.0f, 0.0f );

	rotation.SetRoll( 0.0f );
	rotation.SetPitch( 0.0f );
	rotation.SetYaw( 0.0f );

	return true;
}

void CNodeConnectionRenderObject::AttachLight( ILightObject * light )
{
}

void CNodeConnectionRenderObject::DetachAllLights()
{
}

IMaterial *CNodeConnectionRenderObject::GetMaterial( int index )
{
	return NULL;
}

UINT CNodeConnectionRenderObject::GetNumMaterial()
{
	return 0;
}

ISceneSettings *CNodeConnectionRenderObject::GetSceneSettings()
{
	return NULL;
}