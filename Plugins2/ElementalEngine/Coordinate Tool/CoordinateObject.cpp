///============================================================================
/// \file		CoordinateObject.cpp
/// \brief		Implmentation of CCoordinateObject Object Class
/// \date		09-18-2006
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
#include "CoordinateObject.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

REGISTER_COMPONENT(CCoordinateObject);

REGISTER_MESSAGE_HANDLER(GetGlobalPosition, OnGetGlobalPosition, CCoordinateObject);
REGISTER_MESSAGE_HANDLER(SetGlobalPosition, OnGetGlobalPosition, CCoordinateObject);

/// Public Methods

CCoordinateObject::CCoordinateObject( IHashString *parentName, IHashString *name ) :
	m_hszNameType( _T("CCoordinateObject") ),
	OBJECTTEMPLATE( CCoordinateToolManager, CCoordinateObject, IObject, parentName, name )
{
	// create shader callback
	m_ToolBox = EngineGetToolBox();
}

CCoordinateObject::~CCoordinateObject( void )
{
}

IComponent *CCoordinateObject::Create(int nArgs, va_list argptr)
{
	IHashString *name;
	IHashString *parentName;
	IObject *self;

	name = va_arg(argptr, IHashString *);
	parentName = va_arg(argptr, IHashString *);

	// create the object however you must create it
	try
	{
		self = new CCoordinateObject( parentName, name );
	}
	catch(...)
	{
		return NULL;
	}

	return self;
}

void CCoordinateObject::Serialize( IArchive &ar )
{
	if(ar.IsReading())
	{
	}
	else
	{
	}
}

void CCoordinateObject::Init( void )
{
	CHashString hszCoordinateObjectName = GetName();
	StdString szCoordinateObjectName = hszCoordinateObjectName.GetString();

	StdString szCoordinateRenderObjectName = szCoordinateObjectName + _T("_RenderObject");
	CHashString hszCoordinateRenderObjectName(szCoordinateRenderObjectName.c_str());
	static CHashString hszCoordinateRenderObjectComponentType(_T("CCoordinateRenderObject"));

	IComponent *renderObjectComponent = m_ToolBox->CreateComponent( &hszCoordinateRenderObjectComponentType, 2, &hszCoordinateRenderObjectName, &hszCoordinateObjectName );
	m_RenderObject = dynamic_cast<CCoordinateRenderObject*>(renderObjectComponent);
	m_RenderObject->Init();
}

bool CCoordinateObject::Update( void )
{
	return true;
}

void CCoordinateObject::DeInit( void )
{

}

IHashString* CCoordinateObject::GetComponentType( void )
{
	return &m_hszNameType;
}

bool CCoordinateObject::IsKindOf( IHashString* compType )
{
	return (compType->GetUniqueID() == m_hszNameType.GetUniqueID() );
}

DWORD CCoordinateObject::OnGetGlobalPosition(DWORD size, void *params)
{
	VERIFY_MESSAGE_SIZE( size, sizeof(Vec3) );

	Vec3 *pos = (Vec3*)params;
	pos->Set( GetPosition() );
	return MSG_HANDLED_PROCEED;
}

DWORD CCoordinateObject::OnSetGlobalPosition(DWORD size, void *params)
{
	VERIFY_MESSAGE_SIZE( size, sizeof(Vec3) );

	Vec3 *pos = (Vec3*)params;
	SetPosition( *pos );
	return MSG_HANDLED_PROCEED;
}