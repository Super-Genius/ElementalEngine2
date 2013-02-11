///============================================================================
/// \file	CTriggerReport.cpp
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

CTriggerReport::CTriggerReport()
{
	m_ToolBox = EngineGetToolBox();

	CHashString hashString( _T("CPhysicsManager") );
	m_pManager = (CPhysicsManager*)m_ToolBox->GetComponent( &hashString );
}

CTriggerReport::~CTriggerReport()
{
}

void CTriggerReport::onTrigger( NxShape& triggerShape, NxShape& otherShape, NxTriggerFlag status )
{
	NxActor* triggerActor = &triggerShape.getActor();
	NxActor* otherActor = &otherShape.getActor();

	PHYSICSUSERDATA* triggerUserData = (PHYSICSUSERDATA*)triggerActor->userData;
	PHYSICSUSERDATA* otherUserData = (PHYSICSUSERDATA*)otherActor->userData;

	// make sure the actors have valid userdata
	if( triggerUserData == NULL || otherUserData == NULL )
	{
		//m_ToolBox->Log(LOGINFORMATION, _T("TriggerReport: Trigger on actors with no user data!\n"));

		if( triggerUserData != NULL )
		{
			CPhysicsObject* triggerObject = triggerUserData->physObj;
			IHashString* triggerParent = triggerObject->GetParentName();
			//m_ToolBox->Log(LOGINFORMATION, _T("TriggerReport: triggerActor = %s\n"), triggerParent->GetString());
		}

		if( otherUserData != NULL )
		{
			CPhysicsObject* otherObject = otherUserData->physObj;
			IHashString* otherParent = otherObject->GetParentName();
			//m_ToolBox->Log(LOGINFORMATION, _T("TriggerReport: otherActor = %s\n"), otherParent->GetString());
		}

		return;
	}

	CPhysicsObject* triggerObject = triggerUserData->physObj;
	CPhysicsObject* otherObject = otherUserData->physObj;

	// Get the trigger volume's information
	IHashString* triggerParent = triggerObject->GetParentName();
	
	// Get the other object's information
	IHashString* otherParent = otherObject->GetParentName();

	if( m_pManager->IsContactPair( triggerObject->GetContactGroup(), otherObject->GetContactGroup() ) )
	{

		CHashString memType(_T("Memory"));
		CREATEARCHIVE caA;
		caA.mode = STREAM_MODE_WRITE | STREAM_MODE_READ;
		caA.streamData = NULL;
		caA.streamSize = 0;
		caA.streamType = &memType;

		static DWORD msgHash_CreateArchive = CHashString(_T("CreateArchive")).GetUniqueID();
		DWORD areturn = m_ToolBox->SendMessage(msgHash_CreateArchive, sizeof(CREATEARCHIVE), &caA);
		if (areturn != MSG_HANDLED)
		{
			StdString error;
			error = _T(__FILE__ " onTrigger(): cannot create archive\n");
			EngineGetToolBox()->Log( LOGWARNING, error );
			return;
		}

		IArchive *ar = caA.archive;
		ar->Write( otherParent->GetString() );

		CHashString triggerEventName = "";

		if( status & NX_TRIGGER_ON_ENTER )
		{
			//m_ToolBox->Log( LOGINFORMATION, _T("CTriggerReport::onTrigger: Entry Trigger Detected: %s, %s\n"), otherParent->GetString(), triggerParent->GetString() );

			triggerEventName = _T("TriggerVolume_Entry_Event");
		}

		if( status & NX_TRIGGER_ON_STAY )
		{
			//m_ToolBox->Log( LOGINFORMATION, _T("CTriggerReport::onTrigger: STAY Trigger Detected: %s, %s\n"), otherParent->GetString(), triggerParent->GetString() );

			triggerEventName = _T("TriggerVolume_Stay_Event");
		}

		if( status & NX_TRIGGER_ON_LEAVE )
		{
			//m_ToolBox->Log( LOGINFORMATION, _T("CTriggerReport::onTrigger: Exit Trigger Detected: %s, %s\n"), otherParent->GetString(), triggerParent->GetString() );

			triggerEventName = _T("TriggerVolume_Exit_Event");
		}

		if( _tcscmp(triggerEventName.GetString(), _T("")) == 0 )
		{
			return;
		}

		TRIGGEREVENT event;
		event.pArchive = ar;
		event.szTriggerEventName = triggerEventName;
		event.szTriggerObject = triggerParent;
		m_TriggerEventQueue.push( event );
	}
}

void CTriggerReport::ExecuteTriggerEventQueue()
{
	while (!m_TriggerEventQueue.empty())
	{
		TRIGGEREVENT *pTriggerEvent = &m_TriggerEventQueue.front();

		TRIGGEREVENTPARAMS event;
		event.EventParamsArchive = pTriggerEvent->pArchive;
		event.EventName = &pTriggerEvent->szTriggerEventName;
		static DWORD msgHash_TriggerEvent = CHashString("TriggerEvent").GetUniqueID();
		m_ToolBox->SendMessage(msgHash_TriggerEvent, sizeof(event), &event, &pTriggerEvent->szTriggerObject);

		pTriggerEvent->pArchive->Close();

		m_TriggerEventQueue.pop();
	}
}
