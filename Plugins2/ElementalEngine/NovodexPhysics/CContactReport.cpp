///============================================================================
/// \file	CContactReport.cpp
/// \brief	Callback report for collisions
/// \date	10-30-2006
/// \author	Kyle Swaim
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

#include "StdAfx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CContactReport::CContactReport()
{
	m_ToolBox = EngineGetToolBox();
}

CContactReport::~CContactReport()
{
}

void CContactReport::onContactNotify( NxContactPair& pair, NxU32 events )
{
	// Get the EE parent objects of the actors that collided
	NxActor* actorA = pair.actors[0];
	NxActor* actorB = pair.actors[1];

	PHYSICSUSERDATA* userDataA = NULL;
	PHYSICSUSERDATA* userDataB = NULL;
	CPhysicsObject* physicsObjectA = NULL;
	CPhysicsObject* physicsObjectB = NULL;
	IHashString* ParentA = NULL;
	IHashString* ParentB = NULL;

	// make sure the actors have valid userdata
	if( actorA->userData )
	{
		userDataA = (PHYSICSUSERDATA*)actorA->userData;
		physicsObjectA = userDataA->physObj;
		ParentA = physicsObjectA->GetParentName();
	}
	if( actorB->userData )
	{
		userDataB = (PHYSICSUSERDATA*)actorB->userData;
		physicsObjectB = userDataB->physObj;
		ParentB = physicsObjectB->GetParentName();
	}
	if( !userDataA || !userDataB )
	{
/*
		m_ToolBox->Log(LOGINFORMATION, _T("ContactReport: Error with collision on actors A and B:\n"));
		if (!userDataA)
			m_ToolBox->Log(LOGINFORMATION, _T("  Actor A: NULL\n"));
		else
			m_ToolBox->Log(LOGINFORMATION, _T("  Actor A: %s\n"), ParentA->GetString());
		if (!userDataB)
			m_ToolBox->Log(LOGINFORMATION, _T("  Actor B: NULL\n"));
		else
			m_ToolBox->Log(LOGINFORMATION, _T("  Actor B: %s\n"), ParentB->GetString());
*/
		return;
	}
	// Get information about the contact
	NxContactStreamIterator contactStreamIter( pair.stream );
	// Just return the first contact point for now. But we can also get all contact points,
	// what shapes are contacting and contact normals.
	NxContactStreamIterator i(pair.stream);

	Vec3 contactPoint;

    while(i.goNextPair())
    {
        while(i.goNextPatch())
        {
            const NxVec3& contactNormal = i.getPatchNormal();
            while(i.goNextPoint())
            {
                const NxVec3& nxContactPoint = i.getPoint();
				contactPoint = Vec3( nxContactPoint.x, nxContactPoint.y, nxContactPoint.z ); 
            }
        }
    }

	//BuildContactEvent( ParentA, ParentB, contactPoint );
	//BuildContactEvent( ParentB, ParentA, contactPoint );

	// check physics objects for surrogates
	if( physicsObjectA->HasCollisionSurrogate() )
	{
		BuildContactEvent( physicsObjectA->GetCollisionSurrogate(), ParentA, ParentB, contactPoint );
	}
	else
	{
		BuildContactEvent( ParentA, ParentA, ParentB, contactPoint );
	}

	if( physicsObjectB->HasCollisionSurrogate() )
	{
		BuildContactEvent( physicsObjectB->GetCollisionSurrogate(), ParentB, ParentA, contactPoint );
	}
	else
	{
		BuildContactEvent( ParentB, ParentB, ParentA, contactPoint );
	}
/*
	if ((ParentA) && (ParentB))
	{
		m_ToolBox->Log( LOGINFORMATION, _T("CPhysicsManager: Collision Detected: %s, %s\n"), ParentA->GetString(), ParentB->GetString() );	
	}
	else
	{
		m_ToolBox->Log( LOGWARNING, _T("CPhysicsManager: Collision Detected, tho one or the other is null\n") );	
	}
*/
}

void CContactReport::ExecuteContactEventQueue()
{
	while ( !m_ContactEventQueue.empty() )
	{
		CONTACTEVENT *pContactEvent = &m_ContactEventQueue.front();
	
		TRIGGEREVENTPARAMS event;
		event.EventParamsArchive = pContactEvent->pArchive;
		event.EventName = &pContactEvent->szTriggerEventName;
		static DWORD msgHash_TriggerEvent = CHashString("TriggerEvent").GetUniqueID();
		m_ToolBox->SendMessage(msgHash_TriggerEvent, sizeof(event), &event, &pContactEvent->szTriggerObject);
	
		pContactEvent->pArchive->Close();
	
		m_ContactEventQueue.pop();
	}
}

void CContactReport::BuildContactEvent( IHashString* receivingObject, IHashString* firstObject, IHashString* secondObject, Vec3& contactPoint )
{
	DWORD areturn = MSG_NOT_HANDLED;

	CHashString memType(_T("Memory"));
	IArchive *ar = NULL;

	CREATEARCHIVE ca;
	ca.mode = STREAM_MODE_WRITE | STREAM_MODE_READ;
	ca.streamData = NULL;
	ca.streamSize = 0;
	ca.streamType = &memType;

	static DWORD msgHash_CreateArchive = CHashString(_T("CreateArchive")).GetUniqueID();
	areturn = m_ToolBox->SendMessage(msgHash_CreateArchive, sizeof(CREATEARCHIVE), &ca);
	if (areturn != MSG_HANDLED)
	{
		StdString error;
		error = _T(__FILE__ " onContactNotify(): cannot create archive for ParentA\n");
		EngineGetToolBox()->Log( LOGWARNING, error );
		return;
	}

	ar = ca.archive;
	ar->Write( secondObject->GetString() );
	ar->Write( contactPoint );
	ar->Write( firstObject->GetString() );

	CHashString collisionEventName = _T("Collision_Event");
	CHashString messageRecipient = _T("CQHStateObject");

	// Cache the event for the first object
	CONTACTEVENT event;
	event.pArchive = ar;
	event.szTriggerEventName = collisionEventName;
	event.szTriggerObject = receivingObject;
	m_ContactEventQueue.push( event );
}
