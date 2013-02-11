///============================================================================
/// \file		LUAToolBox.cpp
/// \brief		Implementation file for CLuaToolBox
/// \date		05-26-2005
/// \author		Josh Lulewicz
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

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CLuaToolBox::CLuaToolBox(void)
{
	m_EngineToolBox = EngineGetToolBox();
	m_LuaManager = SINGLETONINSTANCE(CLuaManager);
}

CLuaToolBox::~CLuaToolBox(void)
{
}

//
// Register the component with the toolbox
//
void CLuaToolBox::RegisterComponent(IHashString *ComponentName, 
				COMPONENTCREATEFUNC *createFunc, COMPONENTCREATEFUNC *getFunc)
{
}

//
// UnRegister the component with the toolbox
//
void CLuaToolBox::UnRegisterComponent(IHashString *ComponentName)
{
}

//
// this will only work for singletons.
// others return NULL
IComponent *CLuaToolBox::GetComponent(IHashString *componentName)
{
	return m_EngineToolBox->GetComponent(componentName);
}

//
// Get the component by calling it's create function
IComponent *CLuaToolBox::CreateComponent(IHashString *componentName, int nArgs, ...)
{
	return m_EngineToolBox->CreateComponent(componentName, nArgs);
}

// save off message pump in our list
void CLuaToolBox::RegisterMessagePump(IComponent *comp, MSGPUMPFUNC pumpFunc, 
									 DWORD priority, bool bHierarchical, bool bComponentFilter)
{
	m_EngineToolBox->RegisterMessagePump(comp, pumpFunc, priority, bHierarchical, bComponentFilter);

}

void CLuaToolBox::UnregisterMessagePump(IComponent *comp, DWORD priority)
{
	m_EngineToolBox->UnregisterMessagePump(comp, priority);
}

void CLuaToolBox::AddMessagePumpFilter( DWORD MsgPumpComponent, DWORD ComponentType )
{
	m_EngineToolBox->AddMessagePumpFilter( MsgPumpComponent, ComponentType );
}

// Register a message handler in the toolbox
void CLuaToolBox::RegisterMessageHandler(DWORD msg, MSGFUNCTION mFunc, IHashString *msgClass, DWORD priority)
{
	m_EngineToolBox->RegisterMessageHandler(msg, mFunc, msgClass, priority);
}

// Register a message handler in the toolbox
void CLuaToolBox::UnRegisterMessageHandler(DWORD msg, IHashString *msgClass)
{
	m_EngineToolBox->UnRegisterMessageHandler(msg, msgClass);
}

// Get a unique message ID from a IHashString
DWORD CLuaToolBox::GetMessageID(IHashString *msgName)
{
	return m_EngineToolBox->GetMessageID(msgName);
}

void CLuaToolBox::MessageError(const TCHAR *error, const TCHAR *msg, const TCHAR *name)
{
	CLuaScript *execScript;
	execScript = m_LuaManager->GetCurrentExecutingScript();
	if (execScript != NULL)
	{
		const TCHAR *nameStr = (name != NULL) ? name : _T("unknown");
		m_EngineToolBox->Log(LOGWARNING, error, msg, nameStr);
		execScript->OutputScriptStack();
	}
}
//
// send a message to a message handler through message pump
UINT CLuaToolBox::SendMessage(const TCHAR *msg, DWORD size, void *data, 
		IHashString *name, IHashString *compType)
{
	static DWORD msgHash = CHashString(msg).GetUniqueID();
	DWORD retVal = m_EngineToolBox->SendMessage(msgHash, size, data, name, compType);
	if (retVal != MSG_HANDLED)
	{
		const TCHAR *nameStr = (name != NULL) ? name->GetString() : _T("unknown");
		MessageError(_T("LUA: %s Message TO %s failed:\n"), msg, nameStr);
	}
	return retVal;
}

UINT CLuaToolBox::SendMessage(IHashString *msg, DWORD size, void *data, 
		IHashString *name, IHashString *compType)
{
	if (msg != NULL)
	{
		DWORD retVal = m_EngineToolBox->SendMessage(msg->GetUniqueID(), size, data, name, compType);
		if (retVal != MSG_HANDLED)
		{
			const TCHAR *nameStr = (name != NULL) ? name->GetString() : _T("unknown");
			MessageError(_T("LUA: %s Message TO %s failed:\n"), msg->GetString(), nameStr);
		}
		return retVal;
	}
	else
	{
		const TCHAR *nameStr = (name != NULL) ? name->GetString() : _T("unknown");
		MessageError(_T("LUA: %s Message TO %s failed:\n"), _T("*NULLMESSAGEHASH*"), nameStr);
		return MSG_NOT_HANDLED;
	}
}
/*
UINT CLuaToolBox::SendMessage(IHashString *msg, DWORD size, DWORD data, IHashString *name, IHashString *compType)
{
	return SendMessage( msg, size, &data, name, compType );
}
*/
UINT CLuaToolBox::SendMessageDWORD(IHashString *msg, DWORD data, IHashString *name, IHashString *compType)
{
	return SendMessage( msg, sizeof(DWORD), &data, name, compType );
}

UINT CLuaToolBox::SendMessageFloat(IHashString *msg, float data, IHashString *name, IHashString *compType)
{
	return SendMessage( msg, sizeof(float), &data, name, compType );
}

//
// send a message to a message handler through message pump
UINT CLuaToolBox::SendMessage(const TCHAR *msg, DWORD size, TCHAR *data, 
		IHashString *name, IHashString *compType)
{
	static DWORD msgHash = CHashString(msg).GetUniqueID();
	DWORD  retVal = m_EngineToolBox->SendMessage(msgHash, size, (void*)data, name, compType);
	if (retVal != MSG_HANDLED)
	{
		const TCHAR *nameStr = (name != NULL) ? name->GetString() : _T("unknown");
		MessageError(_T("LUA: %s Message TO %s failed:\n"), msg, nameStr);
	}
	return retVal;
}

//
// send a message to a message handler through message pump
UINT CLuaToolBox::SendMessage(DWORD msg, DWORD size, void *data, 
		IHashString *name, IHashString *compType)
{
	DWORD retVal = m_EngineToolBox->SendMessage(msg, size, data, name, compType);
	if (retVal != MSG_HANDLED)
	{
		const TCHAR *msgStr = m_EngineToolBox->GetHashString(msg);
		const TCHAR *nameStr = (name != NULL) ? name->GetString() : _T("unknown");
		MessageError(_T("LUA: %s Message TO %s failed:\n"), msgStr, nameStr);
	}
	return retVal;
}

// Get a pointer to the HASHSTRINGMAP private variable
HASHSTRINGMAP *CLuaToolBox::GetHashStringMap()
{
	return m_EngineToolBox->GetHashStringMap();
}

// Get the next unique ID for a inserted HashString
DWORD CLuaToolBox::GetNextUniqueID()
{
	return m_EngineToolBox->GetNextUniqueID();
}

// Adds the hashstring to the table
TCHAR* CLuaToolBox::AddHashString(const TCHAR* szName, DWORD uniqueID )
{
	return m_EngineToolBox->AddHashString(szName, uniqueID);
}

// get a const TCHAR * given a UniqueID
const TCHAR *CLuaToolBox::GetHashString(DWORD uniqueID)
{
	return m_EngineToolBox->GetHashString(uniqueID);
}

const TCHAR *CLuaToolBox::GetRelativePath(const TCHAR *srcName)
{
	return m_EngineToolBox->GetRelativePath(srcName);
}

bool CLuaToolBox::MakeDirectory(const TCHAR *dirpath)
{
	return m_EngineToolBox->MakeDirectory(dirpath);
}

const TCHAR* CLuaToolBox::GetDrive()
{
	return m_EngineToolBox->GetDrive();
}

// set the directories
void CLuaToolBox::SetDirectories( StdString *BaseDirectory, StdString *ResourceDirectory)
{
	return m_EngineToolBox->SetDirectories(BaseDirectory, ResourceDirectory);
}

// get the directories
void CLuaToolBox::GetDirectories( StdString *BaseDirectory, StdString *ResourceDirectory)
{
	return m_EngineToolBox->GetDirectories(BaseDirectory, ResourceDirectory);
}

// Log values to a file and/or assert
void CLuaToolBox::Log(DWORD severity, const TCHAR *fmt, ...)
{
	m_EngineToolBox->Log(severity, fmt);
}

// Get the system wide error value
DWORD CLuaToolBox::GetErrorValue()
{
	return m_EngineToolBox->GetErrorValue();
}

// Set a system wide error value
void CLuaToolBox::SetErrorValue(DWORD value)
{
	m_EngineToolBox->SetErrorValue(value);
}

// Get the system wide error value in string form
const TCHAR *CLuaToolBox::GetErrorString(DWORD value)
{
	return m_EngineToolBox->GetErrorString(value);
}

void CLuaToolBox::LoadLuaDependency(TCHAR *fileName)
{
	if (m_EngineToolBox)
	{
		EXECUTESCRIPTSTRUCT ess;
		ess.fileName = fileName;
		ess.bAutoStart = true;			// should be true; it's a dependency
		ess.bScriptSingleton = true;	// only want one instance of the script loaded
		ess.scriptName = NULL;

		static DWORD msgHash_LoadAndStartScript = CHashString(_T("LoadAndStartScript")).GetUniqueID();
		m_EngineToolBox->SendMessage(msgHash_LoadAndStartScript, sizeof(EXECUTESCRIPTSTRUCT), &ess);
	}
}

void CLuaToolBox::StartLuaPerformanceProfiler(IHashString *name)
{
#ifndef XBOX
#if !defined(_DEBUG) && !defined(_PROFILE)
	if (m_EngineToolBox)
	{
		m_EngineToolBox->Log(LOGWARNING, _T("Profiler not active in this build mode; please comment out the line in script.\n"));		
	}
	return;
#endif

	if (name)
	{
		PERFPROFMAPBYNAME::iterator mapIter = m_PerformanceProfilerMap.find(name->GetUniqueID());
		if (mapIter == m_PerformanceProfilerMap.end())
		{
			CPerformanceProfiler profiler(name->GetString(), _T(""), false);			
			profiler.Start();
			m_PerformanceProfilerMap.insert(pair<DWORD, CPerformanceProfiler>(name->GetUniqueID(), profiler));			
		}
	}
#endif//XBOX
}

void CLuaToolBox::StartLuaPerformanceProfiler(IHashString *name, IHashString *type)
{
#ifndef XBOX
#if !defined(_DEBUG) && !defined(_PROFILE)
	if (m_EngineToolBox)
	{
		m_EngineToolBox->Log(LOGWARNING, _T("Profiler not active in this build mode; please comment out the line in script.\n"));		
	}
	return;
#endif

	if (name)
	{
		PERFPROFMAPBYNAME::iterator mapIter = m_PerformanceProfilerMap.find(name->GetUniqueID());
		if (mapIter == m_PerformanceProfilerMap.end())
		{
			CPerformanceProfiler profiler(name->GetString(), type->GetString(), false);			
			profiler.Start();
			m_PerformanceProfilerMap.insert(pair<DWORD, CPerformanceProfiler>(name->GetUniqueID(), profiler));
		}
	}
#endif//XBOX
}

void CLuaToolBox::StopLuaPerformanceProfiler(IHashString *name)
{
#ifndef XBOX
	if (name)
	{
		PERFPROFMAPBYNAME::iterator mapIter = m_PerformanceProfilerMap.find(name->GetUniqueID());
		if (mapIter != m_PerformanceProfilerMap.end())
		{
			mapIter->second.Stop();
			m_PerformanceProfilerMap.erase(mapIter);
		}
	}
#endif//XBOX
}