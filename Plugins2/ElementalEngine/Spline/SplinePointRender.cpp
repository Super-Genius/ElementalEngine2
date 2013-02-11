///============================================================================
/// \file		SplinePointRender.cpp
/// \brief		Implmentation of CSplinePointRenderObject Class
/// \date		09-10-2005
/// \author		D. Patrick Ghiocel
/// \note   Elemental Engine
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
#include "SplinePointRender.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

REGISTER_COMPONENT(CSplinePointRenderObject);
REGISTER_MESSAGE_HANDLER(SetGlobalPosition, OnSetGlobalPosition, CSplinePointRenderObject);

CSplinePointRenderObject::CSplinePointRenderObject(IHashString *parentName, IHashString *name, bool bAddToHier) :
	m_hszNameType( _T("CSplinePointRenderObject") ),
	SCENEOBJECTTEMPLATE(CRenderManager, CSplinePointRenderObject, IRenderObject, parentName, name)
{
	m_ToolBox = EngineGetToolBox();

	m_vPosition.Set( 0.0f, 0.0f, 0.0f );
	m_Ends.Set( 5.0f, 5.0f, 5.0f );

	m_bAddToHierarchy = bAddToHier;
	if (bAddToHier)
	{
		AddToHierarchy();
	}

	m_bRender = true;
}

CSplinePointRenderObject::~CSplinePointRenderObject( void )
{
	DeInit();
	if (m_bAddToHierarchy)
	{
		RemoveFromHierarchy();
	}
}

IComponent *CSplinePointRenderObject::Create(int nArgs, va_list argptr)
{
	IHashString *name;
	IHashString *parentName;
	IObject *self;
	bool bAddToHier;

	name = va_arg(argptr, IHashString *);
	parentName = va_arg(argptr, IHashString *);
	// create the object however you must create it
	try
	{
		// try and create an instance
		if (nArgs == 2)
		{
			self = new CSplinePointRenderObject(parentName, name, true);
		}
		else if (nArgs == 2)
		{
			bAddToHier = va_arg(argptr, bool);
			self = new CSplinePointRenderObject(parentName, name, bAddToHier);
		}
	}
	catch(...)
	{
		return NULL;
	}

	return self;
}

void CSplinePointRenderObject::Init()
{
	GETRENDERERINTERFACEPARAMS renderInterfaceParam;
	static DWORD msgHash_GetRendererInterface = CHashString(_T("GetRendererInterface")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_GetRendererInterface, sizeof(GETRENDERERINTERFACEPARAMS), &renderInterfaceParam, NULL, NULL );
	m_IRenderer = renderInterfaceParam.m_RendererInterface;

	UpdateGlobalObject();
}

void CSplinePointRenderObject::DeInit()
{
	RemoveBoundingObject();

	static DWORD msgHash_ParentRemoveBoundingBox = CHashString(_T("ParentRemoveBoundingBox")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_ParentRemoveBoundingBox, 0, NULL, &m_ParentName);
}

void CSplinePointRenderObject::Serialize( IArchive &ar )
{

}

IHashString* CSplinePointRenderObject::GetComponentType( void )
{
	return &m_hszNameType;
}

bool CSplinePointRenderObject::IsKindOf( IHashString* compType )
{
	return (compType->GetUniqueID() == m_hszNameType.GetUniqueID() );
}

bool CSplinePointRenderObject::Render( UINT pass, IEffect * override )
{
	if( m_IRenderer && m_bRender )
	{
		m_IRenderer->DrawPoint( m_vPosition.x, m_vPosition.y, m_vPosition.z, 255, 64, 64, 4 );
	}

	return true;
}

bool CSplinePointRenderObject::IsLessThan( const IRenderObject * obj, DWORD sortMethod, int curRenderPass )
{
	return true;
}

DWORD CSplinePointRenderObject::GetRenderPriority( void )
{
	return 0;
}

UINT CSplinePointRenderObject::GetNumPass( void )
{
	return 1;
}

void CSplinePointRenderObject::SetPosition( const Vec3 &position )
{
	m_vPosition.x = position.x;
	m_vPosition.y = position.y;
	m_vPosition.z = position.z;
}

Vec3& CSplinePointRenderObject::GetPosition( void )
{
	return m_vPosition;
}

IHashString* CSplinePointRenderObject::GetBaseSortClass( void )
{
	return GetComponentType();
}

bool CSplinePointRenderObject::IsAlphaObject( void )
{
	return false;
}

IMeshObject* CSplinePointRenderObject::GetMeshObject( void )
{
	return NULL;
}

bool CSplinePointRenderObject::GetBoundingBox( Vec3 &pos, Vec3 &dim, EulerAngle &rotation )
{
	pos.Set( m_vPosition );
	dim.Set( m_Ends );

	rotation.SetRoll( 0.0f );
	rotation.SetPitch( 0.0f );
	rotation.SetYaw( 0.0f );

	return true;
}

DWORD CSplinePointRenderObject::OnSetGlobalPosition( DWORD size, void *params )
{
	VERIFY_MESSAGE_SIZE(sizeof(Vec3), size);
	Vec3 * pos = (Vec3 *)params;
	SetPosition( *pos );

	return MSG_HANDLED_STOP;
}

void CSplinePointRenderObject::SetToRender( bool value )
{
	m_bRender = value;
}