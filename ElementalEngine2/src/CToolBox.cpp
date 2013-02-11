///============================================================================
/// \file		CToolBox.cpp
/// \brief		Implementation file for tool box class
/// \date		2-25-2005
/// \author		Kenneth Hurley
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
#include "CToolBox.h"
#include "stdarg.h"
#include <errno.h>

#ifdef _LIB
#include "./StaticLibSymbols.h"
INITDLLINFO ELEMENTALENGINE_INITDLLINFO = {
	NULL, 
	NULL, 
	NULL, 
	NULL,
	ELEMENTALENGINE_LIBEXTERNS
};
#endif

#ifdef _DEBUG
bool g_bFirstTimeInit = false;
#endif

//#define VERBOSE_MESSAGE_WARNINGS

namespace ElementalEngine
{
IToolBox *EngineGetToolBox()
{
	// dummy to force linking of all other objects?
	#ifdef _DEBUG
	if( !g_bFirstTimeInit )
	{
		g_bFirstTimeInit = true;
		//_CrtDumpMemoryLeaks();
		//_CrtSetBreakAlloc(5059);
		
	}
	#endif
	
	return SINGLETONINSTANCE(CToolBox);
}
}

CToolBox::CToolBox()
{
	m_CurMsgID = MSG_ID_FIRST;
	m_CurHashStringUnique = HASHSTRING_ID_FIRST;

	m_Timer = SINGLETONINSTANCE( CTimer );
}

CToolBox::~CToolBox()
{
	HASHSTRINGMAP::iterator hsmIter;
	MSGIDHANDLERMAP::iterator mlIter;
	MSGPRIORITYMAP *msgPriorityMap;
	MSGPRIORITYMAP::iterator mpmIter;
	MSGHANDLER *mhTemp;
	IHashString *className;

	for (hsmIter = m_HashStringMap.begin(); hsmIter != m_HashStringMap.end(); hsmIter++)
	{
		delete [] hsmIter->first;
	}

	// clear unused message handlers
	for (mlIter = m_MsgHandlers.begin(); mlIter != m_MsgHandlers.end(); mlIter++)
	{
		msgPriorityMap = mlIter->second;
		for (mpmIter = msgPriorityMap->begin(); mpmIter != 
				msgPriorityMap->end(); ++mpmIter)
		{
			mhTemp = &(mpmIter->second);
			className = mhTemp->m_ClassName;
			delete className;
		}

		msgPriorityMap->clear();
		delete mlIter->second;
	}
	m_MsgHandlers.clear();
}

//
// Register the component with the toolbox
//
void CToolBox::RegisterComponent(IHashString *ComponentName, 
				COMPONENTCREATEFUNC *createFunc, COMPONENTCREATEFUNC *getFunc)
{
	// add the component create function to the map
	m_ComponentsCreate[ComponentName->GetUniqueID()] = createFunc;
	// add the components get function to the map
	if (getFunc != NULL)
		m_ComponentsGet[ComponentName->GetUniqueID()] = getFunc;
}

//
// Register the component with the toolbox
//
void CToolBox::UnRegisterComponent(IHashString *componentName)
{
	// unregister component in the system
	m_ComponentsCreate.erase(componentName->GetUniqueID());
	m_ComponentsGet.erase(componentName->GetUniqueID());
}

//
// this will only work for singletons.
// others return NULL
IComponent *CToolBox::GetComponent(IHashString *componentName)
{
	COMPONENTMAP::iterator cmIter;
	COMPONENTCREATEFUNC *getFunc;
	IComponent *theComponent;

	cmIter = m_ComponentsGet.find(componentName->GetUniqueID());
	assert(cmIter != m_ComponentsGet.end());

	getFunc = cmIter->second;
	if (getFunc == NULL)
	{
		return NULL;
	}

	theComponent = (*getFunc)(0, NULL);
	assert(theComponent != NULL);

	return theComponent;
}

//
// Get the component by calling it's create function
IComponent *CToolBox::CreateComponent(IHashString *componentName, int nArgs, ...)
{
	va_list marker;

	COMPONENTMAP::iterator cmIter;
	COMPONENTCREATEFUNC *createFunc;
	IComponent *theComponent;

	cmIter = m_ComponentsCreate.find(componentName->GetUniqueID());
	if (cmIter == m_ComponentsCreate.end())
	{
		SetErrorValue(WARN_COMPONENT_NOT_FOUND);
		Log(LOGINFORMATION, _T("Unable to create component %s\n"), 
			(TCHAR *)componentName->GetString());
		return NULL;
	}

	createFunc = cmIter->second;
	assert(createFunc != NULL);

	va_start( marker, nArgs );
	theComponent = (*createFunc)(nArgs, marker);
	assert(theComponent != NULL);
    va_end(marker);

	return theComponent;
}

// save off message pump in our list
void CToolBox::RegisterMessagePump(IComponent *comp, MSGPUMPFUNC pumpFunc, 
								   DWORD priority, bool bHierarchical, bool bComponentFilter)
{
	MSGPUMPMAP::iterator mpmIter;
	for (mpmIter = m_MsgPumps.lower_bound(priority); 
			mpmIter != m_MsgPumps.upper_bound(priority); mpmIter++)
	{
		MSGPUMP *oldMsgPump = &(mpmIter->second);
		if (oldMsgPump->m_Comp == comp)
		{
			IHashString *compTypeName;
			compTypeName = comp->GetComponentType();
			assert(compTypeName != NULL);
			// already registered
			Log(LOGWARNING, _T("Component %s already registered message pump\n")
				_T("\tsecond definition ignored\n"), 
					compTypeName->GetString());
			return;
		}
	}

	MSGPUMP msgPump;
	msgPump.m_Comp = comp;
	msgPump.m_MsgPumpFunc = pumpFunc;
	msgPump.m_bHierarchical = bHierarchical;
	msgPump.m_bComponentFilter = bComponentFilter;
	m_MsgPumps.insert(pair<DWORD, MSGPUMP>(priority, msgPump));
}

void CToolBox::UnregisterMessagePump(IComponent *comp, DWORD priority)
{
	MSGPUMPMAP::iterator mpmIter;
	for (mpmIter = m_MsgPumps.lower_bound(priority); 
			mpmIter != m_MsgPumps.upper_bound(priority); mpmIter++)
	{
		MSGPUMP *oldMsgPump = &(mpmIter->second);
		if (oldMsgPump->m_Comp == comp)
		{
			m_MsgPumps.erase( priority );
			return;
		}
	}
}

void CToolBox::AddMessagePumpFilter( DWORD MsgPumpComponent, DWORD ComponentType )
{
	m_CompTypeToMsgPumpMap[ ComponentType ].insert( MsgPumpComponent );
}

// Register a message handler in the toolbox
void CToolBox::RegisterMessageHandler(DWORD msg, MSGFUNCTION mFunc, IHashString *msgClass, DWORD priority)
{
	MSGHANDLER mhTemp;
	MSGIDHANDLERMAP::iterator mlIter;
	MSGPRIORITYMAP *msgPriorityMap;

	mhTemp.m_ClassName = new CHashString(msgClass);
	mhTemp.m_MsgFunc = mFunc;
	
	// see if a message list is created yet with "msg" ID
	mlIter = m_MsgHandlers.find(msg);
	if (mlIter == m_MsgHandlers.end())
	{
		// nope so create one
		msgPriorityMap = new MSGPRIORITYMAP;
		m_MsgHandlers[msg] = msgPriorityMap;
	}
	else
	{
		msgPriorityMap = mlIter->second;
	}

	msgPriorityMap->insert(MHPAIR(priority, mhTemp));
}

// UnRegister a message handler in the toolbox
void CToolBox::UnRegisterMessageHandler(DWORD msg, IHashString *msgClass)
{
	MSGHANDLER *mhTemp;
	MSGIDHANDLERMAP::iterator mlIter;
	MSGPRIORITYMAP *msgPriorityMap;
	MSGPRIORITYMAP::iterator mpmIter;
	IHashString *className;

	// see if a message list is created yet with "msg" ID
	mlIter = m_MsgHandlers.find(msg);
	if (mlIter != m_MsgHandlers.end())
	{
		msgPriorityMap = mlIter->second;
		mpmIter = msgPriorityMap->begin();
		while (mpmIter != msgPriorityMap->end())
		{
			mhTemp = &(mpmIter->second);
			if (mhTemp->m_ClassName->GetUniqueID() == msgClass->GetUniqueID())
			{
				className = mhTemp->m_ClassName;
				mpmIter = msgPriorityMap->erase(mpmIter);
				delete className;
			}
			else
				++mpmIter;
		}
	}
}

// Get a unique message ID from a IHashString
DWORD CToolBox::GetMessageID(IHashString *msgName)
{
	return msgName->GetUniqueID();
}
/*
//
// send a message to a message handler through message pump
DWORD CToolBox::SendMessage(const TCHAR *msg, DWORD size, void *data, 
		IHashString *name, IHashString *compType)
{
#ifdef _DEBUG
	if( _tcscmp( msg, _T("") ) == 0 )
	{
		// This is useful for finding a particular Message
	}
#endif
	return SendMessage(GetMessageID(&CHashString(msg)), size, data, name, compType);
}
*/
//
// send a message to a message handler through message pump
DWORD CToolBox::SendMessage(DWORD msg, DWORD size, void *data, 
		IHashString *name, IHashString *compType)
{
#if defined _PROFILE || defined _DEBUG
	
	// Check if the message is in the map
	PROFILERMESSAGECOUNTMAP::iterator itr;
	itr = m_MessageProfileMap.find( msg );
	if (itr != m_MessageProfileMap.end())
	{
		// If it is then increment the count
		(itr->second)++;
	}
	else
	{
		// otherwise add it
		m_MessageProfileMap.insert( pair<DWORD,int>(msg,1) );
	}

#endif // _PROFILE

	bool bMsgHandledOnce = false;
	MSGPUMPMAP::iterator mplIter;
	MSGPUMPFUNC mpFunc;
	IComponent *comp;
	DWORD retVal;
	MSGIDHANDLERMAP::iterator mihmIter;
	MSGPRIORITYMAP *msgPriorityMap;
	MSGPRIORITYMAP::iterator mlIter;
	MSGHANDLER *msgHandler;
	MSGPUMP *msgPump;

	retVal = MSG_NOT_HANDLED;

	// get list of message handler for this particlar message
	mihmIter = m_MsgHandlers.find(msg);
	if (mihmIter == m_MsgHandlers.end())
	{
		return MSG_NOT_HANDLED;
	}

	// grab the map of messages
	msgPriorityMap = mihmIter->second;

	// grab msg pump filter
	COMPTYPETOMSGPUMPMAP::iterator itrComponentFilter; 
	bool bFilter = false;
	if( compType != NULL )
	{
		itrComponentFilter = m_CompTypeToMsgPumpMap.find( compType->GetUniqueID() );
		if( itrComponentFilter != m_CompTypeToMsgPumpMap.end() )
		{
			bFilter = true;
		}
	}

	// Loop though map of message handlers that respond to this message
	for (mlIter = msgPriorityMap->begin(); mlIter != msgPriorityMap->end(); ++mlIter)
	{
		msgHandler = &mlIter->second;
		retVal = MSG_NOT_HANDLED;

		// Check to see if this is going to a Singleton, skip pumphandlers if true
		bool bToSingleton = ( (compType != NULL) && (m_ComponentsGet.find(compType->GetUniqueID()) != m_ComponentsGet.end()) );
		// we can't send the message to message pumps if name is NULL & compType is NULL
		if (!bToSingleton && ((name != NULL) || (compType != NULL)))
		{
			// and if Component type is NULL or if it matches, then we can send
			if ((compType == NULL) || (*compType == *msgHandler->m_ClassName))
			{
				// iterate through the message pumps and send message
				for (mplIter = m_MsgPumps.begin(); mplIter != m_MsgPumps.end();
						++mplIter)
				{
					msgPump = &mplIter->second;
					// get the component that handles this message pump
					comp = msgPump->m_Comp;
					// get the function pointer
					mpFunc = msgPump->m_MsgPumpFunc;

					// If the MsgPump has a ComponentFilter, we want to check to make sure the compType matches up
					if( (msgPump->m_bComponentFilter) && (bFilter) )
					{
						// Check ComponentMap for MsgPump
						MSGPUMPSET::iterator itrMsgPump = itrComponentFilter->second.find( msgPump->m_Comp->GetComponentType()->GetUniqueID() );
						if( itrMsgPump == itrComponentFilter->second.end() )
						{
							continue;
						}
					}

					// call the function and see if the message was handled
					retVal = (comp->*mpFunc)(size, data, name, 
						msgHandler->m_ClassName, msgHandler->m_MsgFunc);

					// if anything but not handled
					if (retVal != MSG_NOT_HANDLED)
					{
						// ok, check if possible error or performance issue and log as warning
						// this is triggered if user doesn't specify compType
						if ((compType == NULL) && (!msgPump->m_bHierarchical))
						{
							Log(LOGWARNING, _T("Message %s sent to %s, without component type %s\n")
								_T("  Can cause performance or conflict errors\n"),
								GetHashString(msg), name->GetString(), 
								msgHandler->m_ClassName->GetString());
						}

						// switch MSG_HANDLED_XX to MSG_HANDLED and return on anything but proceed
						if (retVal != MSG_HANDLED_PROCEED)
						{
							if (retVal == MSG_HANDLED_STOP)
								retVal = MSG_HANDLED;

							return retVal;
						}
						else
						{
							// Message is handled by at least one pump handler...
							retVal = MSG_HANDLED;
							// ...lets make note of this
							bMsgHandledOnce = true;
						}
					}
				}
			}
		}


		// if the msg was not handled by any of the specific message
		// pumps it must be a singleton and use general message pump
		if (retVal == MSG_NOT_HANDLED)
		{
			// get the component
			COMPONENTCREATEFUNC *getFunc;
			COMPONENTMAP::iterator cmIter;
			IComponent *theComponent;

			// if the componentType is null, use all components with registered
			// message handler
			if ((compType == NULL) || (*compType == *msgHandler->m_ClassName))
			{
				IHashString *className = msgHandler->m_ClassName;
				// ok, let's call the class's get function if available
				cmIter = m_ComponentsGet.find(className->GetUniqueID());
				// hmm, this error happens, if component registers message handler
				// but doesn't have a get routine, which means they are not a singleton
				if (cmIter != m_ComponentsGet.end())
				{
					getFunc = cmIter->second;
					assert(getFunc != NULL);

					theComponent = (*getFunc)(0, NULL);
					assert(theComponent != NULL);

					// send the message to the handler
					retVal = (theComponent->*msgHandler->m_MsgFunc)(size, data);
					// return on error or MSG_HANDLED_STOP
					/*
					if ((retVal != MSG_HANDLED_PROCEED) && (retVal != MSG_NOT_HANDLED))
					{
						// switch MSG_HANDLED_XX to MSG_HANDLED
						if (retVal == MSG_HANDLED_STOP)
							retVal = MSG_HANDLED;
						return retVal;
					}
					*/
					if (retVal != MSG_NOT_HANDLED)
					{
						// switch MSG_HANDLED_XX to MSG_HANDLED and return on anything but proceed
						if (retVal != MSG_HANDLED_PROCEED)
						{
							if (retVal == MSG_HANDLED_STOP)
								retVal = MSG_HANDLED;

							return retVal;
						}
						else
						{
							retVal = MSG_HANDLED;
							// ...lets make note of this
							bMsgHandledOnce = true;
						}
					}
				}
				else
				{
//					SetErrorValue(WARN_OBJECT_NOT_FOUND);
//					Log(LOGWARNING, _T("Unable to send global message to non-singleton object %s\n"),
//						(TCHAR *)msgHandler->m_ClassName->GetString());
				}
			}
		}
	}

	// switch MSG_HANDLED_XX to MSG_HANDLED
	if ((retVal == MSG_HANDLED_PROCEED) || (retVal == MSG_HANDLED_STOP) || bMsgHandledOnce)
		retVal = MSG_HANDLED;

#ifdef VERBOSE_MESSAGE_WARNINGS
	if (retVal == MSG_NOT_HANDLED)
	{
		const TCHAR *szName = (name == NULL) ? _T("NULL") : name->GetString();
		const TCHAR *szCompName = (compType == NULL) ? _T("NULL") : compType->GetString();
		Log(LOGWARNING, _T("Message %s sent to %s of type %s, was not handled by anyone\n"),
				GetHashString(msg), szName, szCompName);
	}
#endif

	return retVal;
}

// Get a pointer to the HASHSTRINGMAP private variable
HASHSTRINGMAP *CToolBox::GetHashStringMap()
{
	return &m_HashStringMap;
}
// Get the next unique ID for a inserted HashString
DWORD CToolBox::GetNextUniqueID()
{
	return m_CurHashStringUnique++;
}

// Adds the hashstring to the table
TCHAR* CToolBox::AddHashString(const TCHAR* szName, DWORD uniqueID )
{
	TCHAR *m_szPtr;
	m_szPtr = _tcsdup(szName);

	m_HashStringMap[m_szPtr] = uniqueID;
	m_UniqueIDToString[uniqueID] = m_szPtr;

	return( m_szPtr );
}

// get a IHashString given a UniqueID
const TCHAR *CToolBox::GetHashString(DWORD uniqueID)
{
	UNIQUEIDTOSTRMAP::iterator utsIter;

	utsIter = m_UniqueIDToString.find(uniqueID);
	if (utsIter == m_UniqueIDToString.end())
	{
		return NULL;
	}

	return utsIter->second;

}

// get a relative path based on a full pathname
const TCHAR *CToolBox::GetRelativePath(const TCHAR *srcName)
{
	size_t sLen = _tcslen(m_ResourceDirectory.c_str());
	if (_tcsnicmp(m_ResourceDirectory.c_str(), srcName, sLen))
	{
		sLen = 0;
	}

	// skip leading \\ for returned pathname
	if (srcName[sLen] == '\\')
		sLen++;

	return srcName + sLen;
}
/*
// get a filename and extension from a full filename
void CToolBox::GetFilenameFromPath(TCHAR * filename, const TCHAR * filepath)
{
	TCHAR drive[_MAX_DRIVE];
	TCHAR dir[_MAX_DIR];
	TCHAR fname[_MAX_FNAME];
	TCHAR ext[_MAX_EXT];

	if (filepath == NULL)
	{
		filename = NULL;
		return;
	}
	_tsplitpath( filepath, drive, dir, fname, ext );

	StdString szOutput;
	szOutput += fname;
	szOutput += ext;

	_tcscpy(filename, szOutput);
}
*/

bool CToolBox::MakeDirectory(const TCHAR *dirpath)
{
#ifdef WIN32
	if (!_tmkdir(dirpath))
		return true;
	else
	{
		int error = errno;
		if (error == EEXIST)
		{
			Log(LOGWARNING, _T("MakeDirectory: Cannot create new directory at %s, already exists.\n"));
			return true;
		}
		if (error == ENOENT)
            Log(LOGWARNING, _T("MakeDirectory: Cannot create new directory at %s, path does not exist.\n"));
		return false;
	}
#else
	return false;
#endif
}

const TCHAR* CToolBox::GetDrive()
{
	return m_Drive.c_str();
}

void CToolBox::SetDirectories( StdString *BaseDirectory, StdString *ResourceDirectory)
{
	if (BaseDirectory )
	{
		m_ExecutableDirectory = *BaseDirectory;
		m_Drive = ExtractDrive( m_ExecutableDirectory );
	}

	if (ResourceDirectory )
	{
		m_ResourceDirectory = *ResourceDirectory;
	}

}

void CToolBox::GetDirectories( StdString *BaseDirectory, StdString *ResourceDirectory)
{
	if (BaseDirectory )
	{
		*BaseDirectory = m_ExecutableDirectory;
	}

	if (ResourceDirectory )
	{
		*ResourceDirectory = m_ResourceDirectory;
	}
}

// Log values to a file and/or assert
void CToolBox::Log(DWORD severity, const TCHAR *fmt, ...)
{
	TCHAR msgBuff[MAX_MESSAGE_SIZE+1];
	TCHAR msgBuff2[MAX_MESSAGE_SIZE+1];
	LOGMESSAGESTRUCT logMessage;
	va_list marker;

	va_start( marker, fmt );

	logMessage.severity = severity;
	logMessage.msg = msgBuff;

	_vsnprintf(msgBuff, MAX_MESSAGE_SIZE, fmt, marker);
	if (severity == LOGERROR)
	{
		_snprintf( msgBuff2, MAX_MESSAGE_SIZE, "ERROR: %s", msgBuff );
		logMessage.msg = msgBuff2;
	}
	else if (severity == LOGWARNING)
	{
		_snprintf( msgBuff2, MAX_MESSAGE_SIZE, "WARNING: %s", msgBuff );
		logMessage.msg = msgBuff2;
	}
	else if (severity == LOGFATALERROR)
	{
		_snprintf( msgBuff2, MAX_MESSAGE_SIZE, "FATAL ERROR: %s", msgBuff );
		logMessage.msg = msgBuff2;
	}

	DWORD msgHash_LogMessage = CHashString(_T("LogMessage")).GetUniqueID();
	SendMessage(msgHash_LogMessage, sizeof(LOGMESSAGESTRUCT), &logMessage);
    va_end(marker);
}

// Get the system wide error value
DWORD CToolBox::GetErrorValue()
{
	return m_GlobalErrorValue;
}

// Set a system wide error value
void CToolBox::SetErrorValue(DWORD value)
{
	m_GlobalErrorValue = value;
}

// Get the system wide error value in string form
const TCHAR *CToolBox::GetErrorString(DWORD value)
{
	static TCHAR *unKnownStr = _T("Unknown");
	return unKnownStr;
}

PROFILERMESSAGECOUNTMAP* CToolBox::GetMessageProfilerCountMap()
{
	return &m_MessageProfileMap;
}

ITimer* CToolBox::GetTimer()
{
	return m_Timer;
}
