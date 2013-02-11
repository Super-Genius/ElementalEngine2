///============================================================================
/// \file		HeightNodeRenderObject.cpp
/// \brief		Implmentation of CHeightNodeRenderObject Class
/// \date		08-09-2007
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

REGISTER_COMPONENT(CHeightNodeRenderObject);

CHeightNodeRenderObject::CHeightNodeRenderObject(IHashString *parentName, IHashString *name) :
	m_hszComponentType( _T("CHeightNodeRenderObject") ),
	SCENEOBJECTTEMPLATE( CNodeSystemManager, CHeightNodeRenderObject, IRenderObject, parentName, name )
{
	m_ToolBox = EngineGetToolBox();
}

CHeightNodeRenderObject::~CHeightNodeRenderObject()
{
}

IComponent *CHeightNodeRenderObject::Create(int nArgs, va_list argptr)
{
	IHashString *name;
	IHashString *parentName;
	IObject *self;

	name = va_arg(argptr, IHashString *);
	parentName = va_arg(argptr, IHashString *);
	// create the object however you must create it
	try
	{
		self = new CHeightNodeRenderObject( parentName, name );
	}
	catch(...)
	{
		return NULL;
	}

	return self;
}

void CHeightNodeRenderObject::Init()
{
	GETRENDERERINTERFACEPARAMS renderInterfaceParam;
	static DWORD msgHash_GetRendererInterface = CHashString(_T("GetRendererInterface")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_GetRendererInterface, sizeof(GETRENDERERINTERFACEPARAMS), &renderInterfaceParam, NULL, NULL );
	m_IRenderer = renderInterfaceParam.m_RendererInterface;

	UpdateGlobalObject();

	m_HeightNode = NULL;
	m_NodeSystem = NULL;
	IHashString *parentName = GetParentName();
	if( parentName != NULL )
	{
		DWORD parentID = parentName->GetUniqueID();
		static CHashString hszNodeSystemManager( _T("CNodeSystemManager") );
		CNodeSystemManager *manager = dynamic_cast<CNodeSystemManager*>( m_ToolBox->GetComponent( &hszNodeSystemManager ) );
		if( manager != NULL )
		{
			static CHashString hszNodeComponentType( _T("CHeightNode") );
			IDTOOBJECTMAP *nodeObjectMap = manager->GetObjectMap( &hszNodeComponentType );
			if( nodeObjectMap != NULL && nodeObjectMap->find(parentID) != nodeObjectMap->end() )
			{
				IObject *nodeObject = (*nodeObjectMap)[parentID];
				m_HeightNode = dynamic_cast<CHeightNode*>(nodeObject);

				parentID = m_HeightNode->GetParentName()->GetUniqueID();
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
				m_ToolBox->Log( LOGERROR, _T("Could not find object map for CHeightNode.\n") );
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

void CHeightNodeRenderObject::DeInit()
{
	RemoveBoundingObject();
}

void CHeightNodeRenderObject::Serialize( IArchive &ar )
{
}

IHashString *CHeightNodeRenderObject::GetComponentType()
{
	return &m_hszComponentType;
}

bool CHeightNodeRenderObject::IsKindOf(IHashString *compType)
{
	bool retVal = false;
	if( compType != NULL )
	{
		retVal = m_hszComponentType.GetUniqueID() == compType->GetUniqueID();
	}

	return retVal;
}

bool CHeightNodeRenderObject::Render( UINT pass, IEffect * override )
{
	IHashString *managerName = GetManagerName();
	IComponent *amanagerComponent = m_ToolBox->GetComponent( managerName );
	CNodeSystemManager *amanager = dynamic_cast<CNodeSystemManager*>( amanagerComponent );
	bool bRender = amanager->IsRendering();

	if( m_IRenderer != NULL && m_HeightNode != NULL && bRender )
	{
		CHeightNode *connectedNode = m_HeightNode->GetConnectedHeightNode();
		if( connectedNode == NULL )
		{
			connectedNode = m_HeightNode;
		}

		Vec3 vGlobalPosition( 0.0f, 0.0f, 0.0f );
		if( m_NodeSystem != NULL )
		{
			vGlobalPosition = m_NodeSystem->GetPosition();
		}

		m_vPosition = vGlobalPosition + m_HeightNode->GetPosition();
		Vec3 heightPosition = m_vPosition;
		heightPosition.y += m_HeightNode->GetHeight();

		Vec3 connectedPosition = vGlobalPosition + connectedNode->GetPosition();
		Vec3 connectedHeightPosition = connectedPosition;
		connectedHeightPosition.y += connectedNode->GetHeight();


		float size = m_NodeSystem->GetPointSize();
		int r = m_NodeSystem->GetPointR();
		int g = m_NodeSystem->GetPointG();
		int b = m_NodeSystem->GetPointB();
		m_IRenderer->DrawPoint( m_vPosition.x, m_vPosition.y, m_vPosition.z, r, g, b, size );
		m_IRenderer->DrawPoint( heightPosition.x, heightPosition.y, heightPosition.z, r, g, b, size );

		size = m_NodeSystem->GetLineSize();
		r = m_NodeSystem->GetLineR();
		g = m_NodeSystem->GetLineG();
		b = m_NodeSystem->GetLineB();
		m_IRenderer->Draw3DLine(
			m_vPosition.x,
			m_vPosition.y,
			m_vPosition.z,
			heightPosition.x,
			heightPosition.y,
			heightPosition.z,
			r, g, b, size );

		m_IRenderer->Draw3DLine(
			heightPosition.x,
			heightPosition.y,
			heightPosition.z,
			connectedHeightPosition.x,
			connectedHeightPosition.y,
			connectedHeightPosition.z,
			r, g, b, size );

		m_IRenderer->Draw3DLine(
			connectedHeightPosition.x,
			connectedHeightPosition.y,
			connectedHeightPosition.z,
			connectedPosition.x,
			connectedPosition.y,
			connectedPosition.z,
			r, g, b, size );

		m_IRenderer->Draw3DLine(
			connectedPosition.x,
			connectedPosition.y,
			connectedPosition.z,
			m_vPosition.x,
			m_vPosition.y,
			m_vPosition.z,
			r, g, b, size );

		m_IRenderer->Draw3DLine(
			m_vPosition.x,
			m_vPosition.y,
			m_vPosition.z,
			connectedHeightPosition.x,
			connectedHeightPosition.y,
			connectedHeightPosition.z,
			r, g, b, size );

		
	}

	return true;
}

bool CHeightNodeRenderObject::IsLessThan( const IRenderObject * obj, DWORD sortMethod, int curRenderPass )
{
	return true;
}

DWORD CHeightNodeRenderObject::GetRenderPriority()
{
	return 0;
}

UINT CHeightNodeRenderObject::GetNumPass()
{
	return 1;
}

void CHeightNodeRenderObject::SetPosition( const Vec3 &position )
{
	m_vPosition.Set( position );
}

Vec3 &CHeightNodeRenderObject::GetPosition()
{
	return m_vPosition;
}

IHashString *CHeightNodeRenderObject::GetBaseSortClass()
{
	return GetComponentType();
}

bool CHeightNodeRenderObject::IsAlphaObject()
{
	return false;
}

IMeshObject *CHeightNodeRenderObject::GetMeshObject()
{
	return NULL;
}

UINT CHeightNodeRenderObject::GetNumGeometry()
{
	return 0;
}

IGeometry *CHeightNodeRenderObject::GetGeometry( int index ){ return NULL; }

bool CHeightNodeRenderObject::GetBoundingBox( Vec3 &pos, Vec3 &dim, EulerAngle &rotation )
{
	pos.Set( m_vPosition );

	dim.Set( 0.0f, 0.0f, 0.0f );

	rotation.SetRoll( 0.0f );
	rotation.SetPitch( 0.0f );
	rotation.SetYaw( 0.0f );

	return true;
}

void CHeightNodeRenderObject::AttachLight( ILightObject * light )
{
}

void CHeightNodeRenderObject::DetachAllLights()
{
}

IMaterial *CHeightNodeRenderObject::GetMaterial( int index )
{
	return NULL;
}

UINT CHeightNodeRenderObject::GetNumMaterial()
{
	return 0;
}

ISceneSettings *CHeightNodeRenderObject::GetSceneSettings()
{
	return NULL;
}