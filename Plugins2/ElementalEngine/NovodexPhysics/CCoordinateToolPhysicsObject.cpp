///==========================================================================
/// \file	CCoordinateToolPhysicsObject.cpp
/// \brief	An OOBB used for picking coordinate tool axes
/// \date	11/07/2007
/// \author	Dmitriy Sergeyev
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
///==========================================================================

#include "StdAfx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// ------------- Register Object ---------------- //
REGISTER_COMPONENT(CCoordinateToolPhysicsObject);

CCoordinateToolPhysicsObject::CCoordinateToolPhysicsObject( IHashString *parentName, IHashString *name ) : 
	CPickingPhysicsObject( parentName, name, false )
{
}

CCoordinateToolPhysicsObject::~CCoordinateToolPhysicsObject()
{
}

IComponent *CCoordinateToolPhysicsObject::Create( int nArgs, va_list argptr )
{
	IHashString *name;
	IHashString *parentName;
	IObject *self;

	name = va_arg( argptr, IHashString* );
	parentName = va_arg( argptr, IHashString* );
	try
	{
		if( nArgs == 2 )
		{
			self = new CCoordinateToolPhysicsObject( parentName, name );
		}
		else
		{
			EngineGetToolBox()->Log( LOGERROR, _T("Invalid # - %d passed for argument to Create Coordinate Tool Physics Object\n"), nArgs );
			return NULL;
		}
	}
	catch(...)
	{
		EngineGetToolBox()->Log( LOGERROR, _T("CCoordinateToolPhysicsObject: Failed to create CCoordinateToolPhysicsObject!\n") );
		return NULL;
	}

	return self;
}

IHashString *CCoordinateToolPhysicsObject::GetComponentType()
{
	static CHashString componentType( _T("CCoordinateToolPhysicsObject") );
	return &componentType;
}

bool CCoordinateToolPhysicsObject::IsKindOf( IHashString *componentType )
{
	return
		CPickingPhysicsObject::IsKindOf( componentType ) ||
		( componentType->GetUniqueID() == GetComponentType()->GetUniqueID() );
}

void CCoordinateToolPhysicsObject::GetParentTransform( NxMat34& transform )
{
	Matrix4x4 parentTrans;
	static DWORD msgHash_GetGlobalTransform = CHashString(_T("GetGlobalTransform")).GetUniqueID();
	m_ToolBox->SendMessage( msgHash_GetGlobalTransform, sizeof(parentTrans), &parentTrans, GetParentName(), &m_hsParentType );
	
	// remove scale and rotation
	parentTrans.SetRotation( EulerAngle() );

	transform.setColumnMajor44( parentTrans.GetMatrix() );
}
