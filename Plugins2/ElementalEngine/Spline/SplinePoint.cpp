///============================================================================
/// \file		SplinePoint.cpp
/// \brief		Implmentation of CSplinePointObject Class
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
#include "SplinePoint.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

REGISTER_COMPONENT(CSplinePointObject);

REGISTER_MESSAGE_HANDLER( GetGlobalPosition, OnGetGlobalPosition, CSplinePointObject );
REGISTER_MESSAGE_HANDLER( SetGlobalPosition, OnSetGlobalPosition, CSplinePointObject );

CSplinePointObject::CSplinePointObject(IHashString *parentName, IHashString *name, bool bAddToHier) :
	m_hszNameType( _T("CSplinePointObject") ),
	OBJECTTEMPLATE(CSplineManager, CSplinePointObject, IObject, parentName, name)
{
	m_ToolBox = EngineGetToolBox();

	IHashString *managerName = GetManagerName();
	if (managerName)
	{
		m_pManager = dynamic_cast<CSplineManager*>(m_ToolBox->GetComponent(managerName));
	}	

	m_bAddToHierarchy = bAddToHier;
	if (bAddToHier)
	{
		AddToHierarchy();
	}

	m_vPosition.Set( 0.0f, 0.0f, 0.0f );
	m_pRenderObject = NULL;
}

CSplinePointObject::~CSplinePointObject( void )
{
	DeInit();
	if (m_bAddToHierarchy)
	{
		RemoveFromHierarchy();
	}
}

IComponent *CSplinePointObject::Create(int nArgs, va_list argptr)
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
			self = new CSplinePointObject(parentName, name, true);
		}
		else if (nArgs == 3)
		{
			bAddToHier = va_arg(argptr, bool);
			self = new CSplinePointObject(parentName, name, bAddToHier);
		}
	}
	catch(...)
	{
		return NULL;
	}

	return self;
}

void CSplinePointObject::Init()
{
	CHashString hszSplinePointName = GetName();
	StdString wszSplinePointName = hszSplinePointName.GetString();

	StdString wszSplinePointROName = wszSplinePointName + _T("_RenderObject");
	CHashString hszROName(wszSplinePointROName);
	static CHashString hszROType(_T("CSplinePointRenderObject"));

    IComponent *tmpComponent;
	tmpComponent = m_ToolBox->CreateComponent( &hszROType, 2, &hszROName, &hszSplinePointName );
	m_pRenderObject = dynamic_cast<CSplinePointRenderObject*>(tmpComponent);
	m_pRenderObject->Init();
	m_pRenderObject->SetPosition( m_vPosition );

	if (m_pManager)
	{
		bool bToRender = m_pManager->GetGlobalRenderFlag();
		SetToRender(bToRender);
	}

	static DWORD msgHash_AddPointObject = CHashString(_T("AddPointObject")).GetUniqueID();
	static CHashString hsSplineObjectType(_T("CSplineObject"));
	m_ToolBox->SendMessage(msgHash_AddPointObject, sizeof(this), this, GetParentName(), &hsSplineObjectType );
}

void CSplinePointObject::DeInit()
{
	if( m_pRenderObject )
	{
		m_pRenderObject->DeInit();
		delete m_pRenderObject;
		m_pRenderObject = NULL;
	}

	static DWORD msgHash_OnRemovePoint = CHashString(_T("OnRemovePoint")).GetUniqueID();
	static CHashString hsSplineObjectType(_T("CSplineObject"));
	m_ToolBox->SendMessage(msgHash_OnRemovePoint, sizeof(IHashString*), GetName(), GetParentName(), &hsSplineObjectType );
}

void CSplinePointObject::Serialize( IArchive &ar )
{
	if(ar.IsReading())
	{
		ar.Read( m_vPosition, _T("Point") );
	}

	else
	{
		ar.Write( m_vPosition, _T("Point") );
	}
}

IHashString* CSplinePointObject::GetComponentType( void )
{
	return &m_hszNameType;
}

bool CSplinePointObject::IsKindOf( IHashString* compType )
{
	return (compType->GetUniqueID() == m_hszNameType.GetUniqueID() );
}

void CSplinePointObject::SetPosition( Vec3* pPosition )
{
	if( pPosition != NULL )
	{
		m_vPosition.x = pPosition->x;
		m_vPosition.y = pPosition->y;
		m_vPosition.z = pPosition->z;

		if( m_pRenderObject != NULL )
		{
			m_pRenderObject->SetPosition( m_vPosition );
		}
	}
}

void CSplinePointObject::GetPosition( Vec3* pPosition )
{
	if( pPosition != NULL )
	{
		pPosition->Set( m_vPosition );
	}
}

DWORD CSplinePointObject::OnSetGlobalPosition( DWORD size, void *params )
{
	VERIFY_MESSAGE_SIZE(sizeof(Vec3), size);
	Vec3 * pos = (Vec3 *)params;
	
	SetPosition( pos ); 
	
	return MSG_HANDLED_STOP;
}

DWORD CSplinePointObject::OnGetGlobalPosition( DWORD size, void *params )
{
	VERIFY_MESSAGE_SIZE(sizeof(Vec3), size);
	Vec3 * pos = (Vec3 *)params;
	
	GetPosition( pos );
	
	return MSG_HANDLED_STOP;
}

void CSplinePointObject::SetToRender( bool value )
{
	if( m_pRenderObject )
	{
		m_pRenderObject->SetToRender( value );
	}
}
