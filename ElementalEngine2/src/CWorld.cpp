///==========================================================================
/// \file	CWorld.cpp
/// \brief	Fundamental world class
/// \date	3-13-2007
/// \author Brian Bazyk
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
///==========================================================================

#include "StdAfx.h"
#include "CWorld.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

REGISTER_COMPONENT(CWorld);
REGISTER_MESSAGE_HANDLER( GetGlobalPosition, OnGetGlobalPosition, CWorld );
REGISTER_MESSAGE_HANDLER( GetGlobalEuler, OnGetGlobalEuler, CWorld );
REGISTER_MESSAGE_HANDLER( GetGlobalQuaternion, OnGetGlobalQuaternion, CWorld );
REGISTER_MESSAGE_HANDLER( GetGlobalScale, OnGetGlobalScale, CWorld );
REGISTER_MESSAGE_HANDLER( GetGlobalTransform, OnGetGlobalTransform, CWorld );

IComponent *CWorld::Create(int nArgs, va_list argptr)
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
		self = new CWorld(parentName, name);
	}
	catch(...)
	{
		return NULL;
	}

	return self;
}

CWorld::CWorld(IHashString *parentName, IHashString *name)
	: OBJECTTEMPLATE(\0, CWorld, IObject, parentName, name)
{
	m_fWorldVersion = 1.0f;
	AddToHierarchy();
}

CWorld::~CWorld()
{
	RemoveFromHierarchy();
}

void CWorld::Serialize( IArchive &ar )
{
	if( ar.IsReading() )
	{
		ar.Read( m_fWorldVersion, "Version" );
	}
	else
	{
		ar.Write( m_fWorldVersion, "Version" );
	}
}

DWORD CWorld::OnGetGlobalPosition( DWORD size, void* params )
{
	VERIFY_MESSAGE_SIZE( size, sizeof(Vec3) );
	Vec3* position = (Vec3*)params;
	position->Set( 0, 0, 0 );
	return MSG_HANDLED_STOP;
}

DWORD CWorld::OnGetGlobalEuler( DWORD size, void* params )
{
	VERIFY_MESSAGE_SIZE( size, sizeof(Vec3) );
	Vec3* euler = (Vec3*)params;
	euler->Set( 0, 0, 0 );
	return MSG_HANDLED_STOP;
}

DWORD CWorld::OnGetGlobalQuaternion( DWORD size, void* params )
{
	VERIFY_MESSAGE_SIZE( size, sizeof(Quaternion) );
	Quaternion* quaternion = (Quaternion*)params;
	quaternion->Set( 0, 0, 0, 1 );
	return MSG_HANDLED_STOP;
}

DWORD CWorld::OnGetGlobalScale( DWORD size, void* params )
{
	VERIFY_MESSAGE_SIZE( size, sizeof(Vec3) );
	Vec3* scale = (Vec3*)params;
	scale->Set( 1, 1, 1 );
	return MSG_HANDLED_STOP;
}

DWORD CWorld::OnGetGlobalTransform( DWORD size, void* params )
{
	VERIFY_MESSAGE_SIZE( size, sizeof(Matrix4x4) );
	Matrix4x4* transform = (Matrix4x4*)params;
	transform->SetIdentity();
	return MSG_HANDLED_STOP;
}
