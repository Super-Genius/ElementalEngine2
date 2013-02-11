///============================================================================
/// \file	CControllerHitReport.cpp
/// \brief	Callback report for collisions
/// \date	10-30-2006
/// \author	Kyle Swaim
/// \note	Copyright (c) 2005-2006 Signature Devices, Inc.
///============================================================================

#include "StdAfx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CControllerHitReport::CControllerHitReport()
{
	m_ToolBox = EngineGetToolBox();
}

CControllerHitReport::~CControllerHitReport()
{
}

NxControllerAction CControllerHitReport::onShapeHit( const NxControllerShapeHit& hit )
{
	/*
	NxShape* shape = hit.shape;
	NxActor* actor = &shape->getActor();
	CPhysicsObject* physObj = (CPhysicsObject*)actor->userData;
	
	// is the object kinematic?
	bool isKinematic = false;
	if( actor->isDynamic() )
		isKinematic = actor->readBodyFlag( NX_BF_KINEMATIC );

	// is the object flagged to push characters?
	bool canPushCharacter = physObj->CanPushCharacter();

	if( isKinematic && canPushCharacter )
	{
		return NX_ACTION_NONE;
	}
*/
	return NX_ACTION_NONE;
}

NxControllerAction CControllerHitReport::onControllerHit( const NxControllersHit& hit )
{
	/*
	// if its a pushable character
	NxController* thisController = hit.controller;
	NxController* otherController = hit.other;

	CPhysicsObject* thisPhysObject = (CPhysicsObject*)thisController->getActor()->userData;
	CPhysicsObject* otherPhysObject = (CPhysicsObject*)otherController->getActor()->userData;

	// Why won't the envoronmentphysicsobject trigger this itself???

	// If the controller we collided with is a character then we can push it.
	if( thisPhysObject->IsCharacter() )
	{
		//return NX_ACTION_PUSH; // not yet implemented
		
		Vec3 displacement;
		otherPhysObject->OnGetLinearVelocity( sizeof( Vec3 ), &displacement );
		//otherPhysObject->OnGetLinearMomentum( sizeof( Vec3 ), &displacement );

		m_ToolBox->Log( LOGWARNING,
			_T("CControllerHitReport::onControllerHit: Displacement: %f, %f, %f\n"),
			displacement.GetX(), displacement.GetY(), displacement.GetZ() );

		CHashString m_hsParentType( _T("CEntity") );
		IHashString* name = thisPhysObject->GetParentName();

		Vec3 position;
		static DWORD msgHash_GetGlobalPosition = CHashString(_T("GetGlobalPosition")).GetUniqueID();
		m_ToolBox->SendMessage( msgHash_GetGlobalPosition, sizeof(Vec3), &position, name, &m_hsParentType );
	
		position += displacement;

		static DWORD msgHash_SetGlobalPosition = CHashString(_T("SetGlobalPosition")).GetUniqueID();
		m_ToolBox->SendMessage( msgHash_SetGlobalPosition, sizeof(Vec3), &position, name, &m_hsParentType );
	}
*/
	return NX_ACTION_NONE;
}