///============================================================================
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
#include "QHStateMachineDefines.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CGUIStateWrapper::CGUIStateWrapper()
{
	m_szStateObjName = _T("");
	m_vEvents.clear();
}

CGUIStateWrapper::~CGUIStateWrapper()
{
	m_vEvents.clear();
}

void CGUIStateWrapper::Update()
{
	UINT eventSize = m_vEvents.size();
	if (m_vEvents.size() > 0)
	{
		GUIEVENT firstEvent = m_vEvents[0];
		// trigger event, specific to qhstatemachine
		CHashString eventHash = EngineGetToolBox()->GetHashString(firstEvent.event);
		TRIGGEREVENTPARAMS tep;
		tep.EventName = &eventHash;
		tep.EventParamsArchive = firstEvent.ar;
		static CHashString stateObjectType = _T("CQHStateObject");
		static DWORD msgHash_TriggerEvent = CHashString(_T("TriggerEvent")).GetUniqueID();
		EngineGetToolBox()->SendMessage(msgHash_TriggerEvent, sizeof(TRIGGEREVENTPARAMS), &tep, &m_szStateObjName, &stateObjectType);
		
		// remove from queue
		m_vEvents.erase(m_vEvents.begin());
	}
}

void CGUIStateWrapper::AddToQueue(GUIEVENT event)
{
	for (UINT i=0; i<m_vEvents.size(); i++)
	{
		if (event.event == m_vEvents[i].event)
		{
			return; // do not push the same event on the queue more than once
		}
	}
	m_vEvents.push_back(event);
}