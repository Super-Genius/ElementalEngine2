///============================================================================
/// \file       DNSMUtility.cpp
/// \brief      Implementation file for Lua helpers related to State Machine
/// \date       08-19-2005
/// \author     Brent Arias
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

using namespace ElementalEngine;

EVENT* CreateEvent(char* evtName,unsigned uSize)
{
    CREATEEVENT newEvent;
	CHashString temp( evtName );
	newEvent.EventName = &temp;
    newEvent.Size=uSize;
	static DWORD msgHash_CreateEvent = CHashString("CreateEvent").GetUniqueID();
    EngineGetToolBox()->SendMessage(msgHash_CreateEvent,sizeof(CREATEEVENT),&newEvent);
    return newEvent.Event;
};
