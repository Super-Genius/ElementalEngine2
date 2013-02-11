///============================================================================
/// \file	CCollisionReport.h
/// \brief	Implementation of Novodex's collision report interface
///			Only for use with physics
/// \date	8-30-2005
/// \author	Kyle Swaim
/// \note	Copyright (c) 2005 Signature Devices, Inc.
///============================================================================

#ifndef CCOLLISIONREPORT_H
#define CCOLLISIONREPORT_H

#include "NxPhysics.h"
#include "CPhysObject.hpp"

///============================================================================
/// \class	CCollisionReport
/// \brief	Implementation of Novodex's collision report interface
///============================================================================
class CCollisionReport : public NxUserContactReport
{
	void onContactNotify(NxContactPair& pair, NxU32 events)	
	{
		if( events & NX_NOTIFY_ON_START_TOUCH )
		{
			IPhysObject * obj1 = (IPhysObject*)pair.actors[0]->userData;
			IPhysObject * obj2 = (IPhysObject*)pair.actors[1]->userData;

			IHashString *firstObjectName = obj1->GetParentName();
			IHashString *secondObjectName = obj2->GetParentName();

			// Let the objects know who they collided with
			obj2->OnCollide( firstObjectName );
			obj1->OnCollide( secondObjectName );
		}
	}
};

#endif	// CCOLLISIONREPORT_H