///==========================================================================
/// \file	MathManager.cpp
/// \brief	Implementation of Math Manager
/// \date	08-12-2005
/// \author	Brian Bazyk
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
#include "MathManager.h"
#ifndef _LIB
#include "DLLVersion.h"
#endif

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

REGISTER_COMPONENT_SINGLETON( CMathManager );
REGISTER_MESSAGE_HANDLER(GetActiveMathImplementation, OnGetActiveMathImplementation, CMathManager);
REGISTER_MESSAGE_HANDLER(RegisterMathImplementation, OnRegisterMathImplementation, CMathManager);

IComponent *CMathManager::Create(int nArgs, va_list argptr)
{
	return SINGLETONINSTANCE( CMathManager );
}

CMathManager::CMathManager() : 
	CManager(_T("CMathManager"))
{
	m_iCurrentMathLibraryPriority = 9999999;
#ifndef _LIB
	LoadDLMs();
#endif
}

CMathManager::~CMathManager()
{
	list<HINSTANCE>::iterator itHandle;
	itHandle = m_DLLHandles.begin();
	while (itHandle != m_DLLHandles.end())
	{
#ifndef _LIB
		FreeLibrary(*itHandle);
#endif
		itHandle++;
	}
}

DWORD CMathManager::OnGetActiveMathImplementation(DWORD size, void *in_params)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(ACTIVEMATHIMPLEMENTATION));
	ACTIVEMATHIMPLEMENTATION *pParams = (ACTIVEMATHIMPLEMENTATION*)in_params;

	pParams->pImplementation = NULL;

	// get current math library
	MATHLIBRARYMAP::iterator itrMathLib = m_MathLibraryMap.find( m_dwCurrentMathLibrary );
	if (itrMathLib == m_MathLibraryMap.end())
		return MSG_ERROR; // current math library not found (logic error, should never happen)

	// get implementation map
	MATHIMPLEMENTATIONMAP *pImplementationMap = &itrMathLib->second;

	// get implementation
	MATHIMPLEMENTATIONMAP::iterator itrMathImp = pImplementationMap->find( pParams->dwImplementationName );
	if (itrMathImp == pImplementationMap->end())
		return MSG_ERROR; // requested implementation not supported by current math library

	// success
	pParams->pImplementation = itrMathImp->second;

	return MSG_HANDLED_STOP;
}

DWORD CMathManager::OnRegisterMathImplementation(DWORD size, void *in_params)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(REGISTERMATHIMPLEMENTATION));
	REGISTERMATHIMPLEMENTATION *pParams = (REGISTERMATHIMPLEMENTATION*)in_params;

	MATHIMPLEMENTATIONMAP *pDestImpMap = &m_MathLibraryMap[pParams->dwLibraryName];

	// merge registeree's map of implementations with the stored libraries map
	// of implementations
	// TODO: use a multimap and store individual implementation priorities and 
	// only overwrite an implementation if registeree's map is higher priority
	MATHIMPLEMENTATIONMAP::iterator itr = pParams->pImplementationMap->begin();
	while (itr != pParams->pImplementationMap->end())
	{
		(*pDestImpMap)[itr->first] = itr->second;
		itr++;
	}

	if (pParams->iPriority < m_iCurrentMathLibraryPriority)
	{
		m_dwCurrentMathLibrary = pParams->dwLibraryName;
		m_iCurrentMathLibraryPriority = pParams->iPriority;
	}

	return MSG_HANDLED_STOP;
}

void CMathManager::LoadDLMs()
{
	SearchDLMS(_T(".\\Plugins\\*.dlm"));
    InitAllDLMS();
}

void CMathManager::LoadDLM(const TCHAR *pathName)
{
#ifndef _LIB
	HINSTANCE dllHandle;
	GETDLLVERSION GetDLLVersion;
	GETPRIORITY GetPriority;
	DWORD priority;
	INITDLL initDLL;
	TCHAR Mess[2048];
	DWORD version;

	// try loading the dll
	dllHandle = LoadLibrary(pathName);
	if (dllHandle == NULL)
	{
		LPVOID lpMsgBuf = _T("");
#ifdef WIN32
		FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | 
				FORMAT_MESSAGE_IGNORE_INSERTS, NULL, GetLastError(), 
				MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
				(LPTSTR) &lpMsgBuf, 0, NULL);
#endif
		_stprintf( Mess, _T("Error Loading DLL:%s\n%s\n"), pathName, lpMsgBuf);
		EngineGetToolBox()->Log( LOGERROR, Mess );
		return;
	}

	// get version function
	GetDLLVersion = (GETDLLVERSION)GetProcAddress(dllHandle, _T("GetDLLVersion"));
	if (GetDLLVersion == NULL)
	{
		_stprintf( Mess, _T("Unable to load %s\n"
			"Unable to locate version function.\n"), pathName);
		EngineGetToolBox()->Log( LOGERROR, Mess );
		FreeLibrary(dllHandle);
		return;
	}

	// get version
	version = (*GetDLLVersion)() & ~1;
	// check version
	if (version != (INTERFACEDLLVERSION & ~1))
	{
		_stprintf( Mess, _T("Unable to load %s\n"
			"version %x doesn't match expected %x.\n"), pathName, version, INTERFACEDLLVERSION);
		EngineGetToolBox()->Log( LOGERROR, Mess );
		FreeLibrary(dllHandle);
		return;
	}

	// get the init function
	initDLL = (INITDLL)GetProcAddress(dllHandle, _T("InitDLL"));
	if (initDLL == NULL)
	{
		_stprintf( Mess, _T("Unable to load %s\n"
			"InitDLL routine failed.\n"), pathName);
		EngineGetToolBox()->Log( LOGERROR, Mess );
		FreeLibrary(dllHandle);
		return;
	}

	// get the priority function
	GetPriority = (GETPRIORITY)GetProcAddress(dllHandle, _T("GetPriority"));
	if (GetPriority == NULL)
	{
		_stprintf( Mess, _T("Unable to load %s\n"
				"GetPriority routine failed.\n"), pathName);
		EngineGetToolBox()->Log( LOGERROR, Mess );
		FreeLibrary(dllHandle);
		return;
	}

	priority = (*GetPriority)();

	m_DLLInits.insert(pair<DWORD, INITDLL>(priority, initDLL));
	m_DLLHandles.push_back( dllHandle );
#endif
}

void CMathManager::InitAllDLMS()
{
	DLLINITMAP::iterator dimIter;
	for (dimIter = m_DLLInits.begin(); dimIter != m_DLLInits.end(); ++dimIter)
	{
		// initialize the dll
		(*dimIter->second)();
	}
}

void CMathManager::SearchDLMS(const TCHAR *pathName)
{
#ifndef _LIB
	// this should change to use generic FINDDATA struct
	WIN32_FIND_DATA fData;
	HANDLE hFile;
	BOOL done;
	size_t wildIndex;

	TCHAR szPath[MAX_PATH];
	_tcscpy( szPath, pathName );
	size_t iPathLength = _tcslen( szPath );

	for (size_t i=iPathLength; i > 0 && szPath[i] != '\\'; i--)
		wildIndex = i;

	szPath[wildIndex] = '\0';

	hFile = FindFirstFile(pathName, &fData);
	done = (hFile == INVALID_HANDLE_VALUE);

	int libraryCount = 0;
	while (!done)
	{ 
		TCHAR dllName[MAX_PATH];
		_tcscpy( dllName, szPath );
		_tcscat( dllName, fData.cFileName );
		LoadDLM(dllName);
		libraryCount++;

	    if (!FindNextFile(hFile, &fData))
		{
	        done = TRUE;
	    }
	}

	if (hFile != INVALID_HANDLE_VALUE)
	{
		if (!FindClose(hFile))
		{
			assert(0);
		}
	}
#endif
} 
