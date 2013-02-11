///============================================================================
/// \file		NodeRenderObject.cpp
/// \brief		Implmentation of CNodeRenderObject Class
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

REGISTER_COMPONENT(CNodeRenderObject);

CNodeRenderObject::CNodeRenderObject(IHashString *parentName, IHashString *name) :
	m_hszComponentType( _T("CNodeRenderObject") ),
	SCENEOBJECTTEMPLATE( CNodeSystemManager, CNodeRenderObject, IRenderObject, parentName, name )
{
	m_ToolBox = EngineGetToolBox();
}

CNodeRenderObject::~CNodeRenderObject()
{
}

IComponent *CNodeRenderObject::Create(int nArgs, va_list argptr)
{
	IHashString *name;
	IHashString *parentName;
	IObject *self;

	name = va_arg(argptr, IHashString *);
	parentName = va_arg(argptr, IHashString *);
	// create the object however you must create it
	try
	{
		self = new CNodeRenderObject( parentName, name );
	}
	catch(...)
	{
		return NULL;
	}

	return self;
}

void CNodeRenderObject::Init()
{
	GETRENDERERINTERFACEPARAMS renderInterfaceParam;
	static DWORD msgHash_GetRendererInterface = CHashString(_T("GetRendererInterface")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_GetRendererInterface, sizeof(GETRENDERERINTERFACEPARAMS), &renderInterfaceParam, NULL, NULL );
	m_IRenderer = renderInterfaceParam.m_RendererInterface;

	UpdateGlobalObject();

	m_Node = NULL;
	m_NodeSystem = NULL;
	IHashString *parentName = GetParentName();
	if( parentName != NULL )
	{
		DWORD parentID = parentName->GetUniqueID();
		static CHashString hszNodeSystemManager( _T("CNodeSystemManager") );
		CNodeSystemManager *manager = dynamic_cast<CNodeSystemManager*>( m_ToolBox->GetComponent( &hszNodeSystemManager ) );
		if( manager != NULL )
		{
			static CHashString hszNodeComponentType( _T("CNode") );
			IDTOOBJECTMAP *nodeObjectMap = manager->GetObjectMap( &hszNodeComponentType );
			if( nodeObjectMap != NULL && nodeObjectMap->find(parentID) != nodeObjectMap->end() )
			{
				IObject *nodeObject = (*nodeObjectMap)[parentID];
				m_Node = dynamic_cast<CNode*>(nodeObject);

				parentID = m_Node->GetParentName()->GetUniqueID();
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
				m_ToolBox->Log( LOGERROR, _T("Could not find object map for CNode.\n") );
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

void CNodeRenderObject::DeInit()
{
	RemoveBoundingObject();
}

void CNodeRenderObject::Serialize( IArchive &ar )
{
}

IHashString *CNodeRenderObject::GetComponentType()
{
	return &m_hszComponentType;
}

bool CNodeRenderObject::IsKindOf(IHashString *compType)
{
	bool retVal = false;
	if( compType != NULL )
	{
		retVal = m_hszComponentType.GetUniqueID() == compType->GetUniqueID();
	}

	return retVal;
}

bool CNodeRenderObject::Render( UINT pass, IEffect * override )
{
	IHashString *managerName = GetManagerName();
	IComponent *amanagerComponent = m_ToolBox->GetComponent( managerName );
	CNodeSystemManager *amanager = dynamic_cast<CNodeSystemManager*>( amanagerComponent );
	bool bRender = amanager->IsRendering();

	if( m_IRenderer != NULL && m_Node != NULL && bRender )
	{
		Vec3 vGlobalPosition( 0.0f, 0.0f, 0.0f );
		if( m_NodeSystem != NULL )
		{
			vGlobalPosition = m_NodeSystem->GetPosition();
		}

		m_vPosition = vGlobalPosition + m_Node->GetPosition();

		float size = 5.0;
		int r = 255;
		int g = 255;
		int b = 255;

		if( m_NodeSystem != NULL )
		{
			size = m_NodeSystem->GetPointSize();
			r = m_NodeSystem->GetPointR();
			g = m_NodeSystem->GetPointG();
			b = m_NodeSystem->GetPointB();
		}
		m_IRenderer->DrawPoint( m_vPosition.x, m_vPosition.y, m_vPosition.z, r, g, b, size );
	}

	return true;
}

bool CNodeRenderObject::IsLessThan( const IRenderObject * obj, DWORD sortMethod, int curRenderPass )
{
	return true;
}

DWORD CNodeRenderObject::GetRenderPriority()
{
	return 0;
}

UINT CNodeRenderObject::GetNumPass()
{
	return 1;
}

void CNodeRenderObject::SetPosition( const Vec3 &position )
{
	m_vPosition.Set( position );
}

Vec3 &CNodeRenderObject::GetPosition()
{
	return m_vPosition;
}

IHashString *CNodeRenderObject::GetBaseSortClass()
{
	return GetComponentType();
}

bool CNodeRenderObject::IsAlphaObject()
{
	return false;
}

IMeshObject *CNodeRenderObject::GetMeshObject()
{
	return NULL;
}

UINT CNodeRenderObject::GetNumGeometry()
{
	return 0;
}

IGeometry *CNodeRenderObject::GetGeometry( int index ){ return NULL; }

bool CNodeRenderObject::GetBoundingBox( Vec3 &pos, Vec3 &dim, EulerAngle &rotation )
{
	pos.Set( m_vPosition );

	dim.Set( 0.0f, 0.0f, 0.0f );

	rotation.SetRoll( 0.0f );
	rotation.SetPitch( 0.0f );
	rotation.SetYaw( 0.0f );

	return true;
}

void CNodeRenderObject::AttachLight( ILightObject * light )
{
}

void CNodeRenderObject::DetachAllLights()
{
}

IMaterial *CNodeRenderObject::GetMaterial( int index )
{
	return NULL;
}

UINT CNodeRenderObject::GetNumMaterial()
{
	return 0;
}

ISceneSettings *CNodeRenderObject::GetSceneSettings()
{
	return NULL;
}