///=====================================================================
/// \file	CListenerFactory.cpp
/// \brief	Implements of CListenerFactory Class
///         This file implements a CListenerFactory which is the adapter
///			between vision listeners and our System
/// \date	03/09/2005
/// \author	Kenneth Hurley
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
///=====================================================================

#include "StdAfx.h"

#include "CListenerFactory.h"
#include "PerformanceProfiler.h"

REGISTER_COMPONENT_SINGLETON(CListenerFactory);
REGISTER_MESSAGE_HANDLER(GlobalUpdateTick, GlobalTickMessage, CListenerFactory);

/// \brief	Function to get this component.  This is a singleton
/// \param	void
/// \return	IComponent*: This Manager
IComponent *CListenerFactory::Create(int nArgs, va_list argptr)
{
	return SINGLETONINSTANCE(CListenerFactory);
}

/// \brief The Default Destructor for CListenerFactory
/// \return void
CListenerFactory::~CListenerFactory( )
{
}

/// \brief	Initializes the Listener Factory
/// \return	bool: Success
bool CListenerFactory::Init()
{
	return true;
}


void CListenerFactory::Register(IListener *sourceListener, DWORD priority)
{
	m_RegisteredListeners.insert(pair<DWORD, IListener *>(priority,
			sourceListener));
}

bool CListenerFactory::UnRegister(IListener *sourceListener)
{
	LISTENERKEYMAP::iterator lkmIter;
	LISTENERMMAP::iterator lmmIter;
	DWORD key;

	// first get the key for the listener
	lkmIter = m_ListenerKeys.find(sourceListener);
	if (lkmIter != m_ListenerKeys.end())
	{
		// now grab the key associated with this listener
		key = lkmIter->second;
		// see if multimap has at least one of the keys
		lmmIter = m_RegisteredListeners.find(key);
		if (lmmIter != m_RegisteredListeners.end())
		{
			// look though registered keys and find our listener
			for (;lmmIter->first == key; lmmIter++)
			{
				// is this the correct listener?
				if (lmmIter->second == sourceListener)
				{
					// yep. Erase it from the list
					m_RegisteredListeners.erase(lmmIter);
					return true;
				}
			}
		}
	}

	return false;
}

// GlobalTickMessage handler
DWORD CListenerFactory::GlobalTickMessage(DWORD size, void *param)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(GLOBALTICKPARAMS));
	GLOBALTICKPARAMS *gtp = (GLOBALTICKPARAMS*)param;

	// we should pass a tick value? - KEN
	Update(gtp->tickCount);
	return MSG_HANDLED_PROCEED;
}

/// \brief	Update function for the ListenerFactory
/// \return	void
void CListenerFactory::Update(DWORD tickCount)
{
	LISTENERMMAP::iterator lmmIter;		/// iterator
	IListener *curListener;

	UINT noNameNum = 0;

	// iterate through listeners by priority
	for (lmmIter = m_RegisteredListeners.begin(); 
			lmmIter != m_RegisteredListeners.end(); lmmIter++)
	{
		curListener = lmmIter->second;
		
		StdString profileName;
		IHashString *compName = curListener->GetName();
		if (compName == NULL)
		{
			profileName = _T("NoParentName_");
			TCHAR buf[10];
			_stprintf(buf, "%d", noNameNum);
			profileName += buf;
			noNameNum++;
		}
		else
		{
			profileName = compName->GetString();
		}
//		profileName += _T("_Update");

		PERFORMANCE_PROFILER_TYPE_START(profileName, _T("Update tick"));
		
		// call it's update function
		curListener->Update(tickCount);

		PERFORMANCE_PROFILER_TYPE_STOP(profileName, _T("Update tick"));
	}
}

/// \brief	DeInitializes the ListenerFactory
/// \return	bool: Success
bool CListenerFactory::DeInit( void )
{
	return true;
}

// should we serialize the listener factory?
void CListenerFactory::Serialize(IArchive &ar)
{
}

IHashString *CListenerFactory::GetComponentType()
{
	static CHashString LFCT(_T("CListenerFactory"));
	return &LFCT;
}

bool CListenerFactory::IsKindOf(IHashString *compType)
{
	return (compType->GetUniqueID() == GetComponentType()->GetUniqueID());
}
