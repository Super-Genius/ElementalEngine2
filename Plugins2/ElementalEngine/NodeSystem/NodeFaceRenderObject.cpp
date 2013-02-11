///============================================================================
/// \file		NodeFaceRenderObject.cpp
/// \brief		Implmentation of CNodeFaceRenderObject Class
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

REGISTER_COMPONENT(CNodeFaceRenderObject);

CNodeFaceRenderObject::CNodeFaceRenderObject(IHashString *parentName, IHashString *name) :
	m_hszComponentType( _T("CNodeFaceRenderObject") ),
	SCENEOBJECTTEMPLATE( CNodeSystemManager, CNodeFaceRenderObject, IRenderObject, parentName, name )
{
	m_ToolBox = EngineGetToolBox();
}

CNodeFaceRenderObject::~CNodeFaceRenderObject()
{
}

IComponent *CNodeFaceRenderObject::Create(int nArgs, va_list argptr)
{
	IHashString *name;
	IHashString *parentName;
	IObject *self;

	name = va_arg(argptr, IHashString *);
	parentName = va_arg(argptr, IHashString *);
	// create the object however you must create it
	try
	{
		self = new CNodeFaceRenderObject( parentName, name );
	}
	catch(...)
	{
		return NULL;
	}

	return self;
}

void CNodeFaceRenderObject::Init()
{
	GETRENDERERINTERFACEPARAMS renderInterfaceParam;
	static DWORD msgHash_GetRendererInterface = CHashString(_T("GetRendererInterface")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_GetRendererInterface, sizeof(GETRENDERERINTERFACEPARAMS), &renderInterfaceParam, NULL, NULL );
	m_IRenderer = renderInterfaceParam.m_RendererInterface;

	UpdateGlobalObject();

	m_NodeFace = NULL;
	m_NodeSystem = NULL;
	IHashString *parentName = GetParentName();
	if( parentName != NULL )
	{
		DWORD parentID = parentName->GetUniqueID();
		static CHashString hszNodeSystemManager( _T("CNodeSystemManager") );
		CNodeSystemManager *manager = dynamic_cast<CNodeSystemManager*>( m_ToolBox->GetComponent( &hszNodeSystemManager ) );
		if( manager != NULL )
		{
			static CHashString hszNodeComponentType( _T("CNodeFace") );
			IDTOOBJECTMAP *nodeObjectMap = manager->GetObjectMap( &hszNodeComponentType );
			if( nodeObjectMap != NULL && nodeObjectMap->find(parentID) != nodeObjectMap->end() )
			{
				IObject *nodeObject = (*nodeObjectMap)[parentID];
				m_NodeFace = dynamic_cast<CNodeFace*>(nodeObject);

				parentID = m_NodeFace->GetParentName()->GetUniqueID();
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
				m_ToolBox->Log( LOGERROR, _T("Could not find object map for CNodeFace.\n") );
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

void CNodeFaceRenderObject::DeInit()
{
	RemoveBoundingObject();
}

void CNodeFaceRenderObject::Serialize( IArchive &ar )
{
}

IHashString *CNodeFaceRenderObject::GetComponentType()
{
	return &m_hszComponentType;
}

bool CNodeFaceRenderObject::IsKindOf(IHashString *compType)
{
	bool retVal = false;
	if( compType != NULL )
	{
		retVal = m_hszComponentType.GetUniqueID() == compType->GetUniqueID();
	}

	return retVal;
}

bool CNodeFaceRenderObject::Render( UINT pass, IEffect * override )
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

		Vec3 node1Position = m_NodeFace->GetNode1Position() + vGlobalPosition;
		Vec3 node2Position = m_NodeFace->GetNode2Position() + vGlobalPosition;
		Vec3 node3Position = m_NodeFace->GetNode3Position() + vGlobalPosition;

		float size = m_NodeSystem->GetLineSize();
		int r = m_NodeSystem->GetLineR();
		int g = m_NodeSystem->GetLineG();
		int b = m_NodeSystem->GetLineB();

		m_IRenderer->Draw3DLine(
			node1Position.x,
			node1Position.y,
			node1Position.z,
			node2Position.x,
			node2Position.y,
			node2Position.z,
			r, g, b, size );

		m_IRenderer->Draw3DLine(
			node2Position.x,
			node2Position.y,
			node2Position.z,
			node3Position.x,
			node3Position.y,
			node3Position.z,
			r, g, b, size );

		m_IRenderer->Draw3DLine(
			node3Position.x,
			node3Position.y,
			node3Position.z,
			node1Position.x,
			node1Position.y,
			node1Position.z,
			r, g, b, size );
	}

	return true;
}

bool CNodeFaceRenderObject::IsLessThan( const IRenderObject * obj, DWORD sortMethod, int curRenderPass )
{
	return true;
}

DWORD CNodeFaceRenderObject::GetRenderPriority()
{
	return 0;
}

UINT CNodeFaceRenderObject::GetNumPass()
{
	return 1;
}

void CNodeFaceRenderObject::SetPosition( const Vec3 &position )
{
	m_vPosition.Set( position );
}

Vec3 &CNodeFaceRenderObject::GetPosition()
{
	return m_vPosition;
}

IHashString *CNodeFaceRenderObject::GetBaseSortClass()
{
	return GetComponentType();
}

bool CNodeFaceRenderObject::IsAlphaObject()
{
	return false;
}

IMeshObject *CNodeFaceRenderObject::GetMeshObject()
{
	return NULL;
}

UINT CNodeFaceRenderObject::GetNumGeometry()
{
	return 0;
}

IGeometry *CNodeFaceRenderObject::GetGeometry( int index ){ return NULL; }

bool CNodeFaceRenderObject::GetBoundingBox( Vec3 &pos, Vec3 &dim, EulerAngle &rotation )
{
	pos.Set( m_vPosition );

	dim.Set( 0.0f, 0.0f, 0.0f );

	rotation.SetRoll( 0.0f );
	rotation.SetPitch( 0.0f );
	rotation.SetYaw( 0.0f );

	return true;
}

void CNodeFaceRenderObject::AttachLight( ILightObject * light )
{
}

void CNodeFaceRenderObject::DetachAllLights()
{
}

IMaterial *CNodeFaceRenderObject::GetMaterial( int index )
{
	return NULL;
}

UINT CNodeFaceRenderObject::GetNumMaterial()
{
	return 0;
}

ISceneSettings *CNodeFaceRenderObject::GetSceneSettings()
{
	return NULL;
}