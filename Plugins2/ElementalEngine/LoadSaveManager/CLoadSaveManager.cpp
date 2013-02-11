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
#include "CLoadSaveManager.hpp"
#include "PerformanceProfiler.h"
#include <sys/stat.h>

// Includes for previous file existence check
//#include <fcntl.h>
//#include <sys/types.h>
//#include <io.h>

// Include for file existence check thru EE messages
#include "FilePath.h"


REGISTER_COMPONENT_SINGLETON(CLoadSaveManager);
REGISTER_MESSAGE_HANDLER(LoadFileByExtension, OnLoadFile, CLoadSaveManager);
REGISTER_MESSAGE_HANDLER(SaveFileByExtension, OnSaveFile, CLoadSaveManager);
REGISTER_MESSAGE_HANDLER(UnloadFileByExtension, OnUnloadFile, CLoadSaveManager);
REGISTER_MESSAGE_HANDLER(RegisterLoadSaveComponent, OnRegisterLoadSave, CLoadSaveManager);
REGISTER_MESSAGE_HANDLER(GetFilterStrings, OnGetFilterStrings, CLoadSaveManager);
REGISTER_MESSAGE_HANDLER(GetFileVersion, OnGetFileVersion, CLoadSaveManager);
REGISTER_MESSAGE_HANDLER(SetFileVersion, OnSetFileVersion, CLoadSaveManager);

CLoadSaveManager::CLoadSaveManager() 
	: m_ComponentType(_T("CLoadSaveManager"))
{
	// save for convenience
	m_ToolBox = EngineGetToolBox();
	m_hszFileVersion = _T( "" );
	m_hsExtComponentType = _T( "" );
	m_strDefaultResourcePath = _T( "" );
}

CLoadSaveManager::~CLoadSaveManager( )
{
}

// create function for this world loader
IComponent *CLoadSaveManager::Create(int nArgs, va_list argptr)
{
	return SINGLETONINSTANCE( CLoadSaveManager );
}

DWORD CLoadSaveManager::LoadSaveFile(LPCTSTR filePathName, bool isLoad, bool isInternal)
{
	IHashString *componentName;
	DWORD retVal;
	StdString messageName;
	StdString szFilePathName( filePathName );
	bool bRelativePath = TRUE;
	IToolBox *toolBox = EngineGetToolBox();

	// If handed a null string, (happens often enough in normal use not to warrant an error).
	// then exit out without typing anything
    if (filePathName)
		if (filePathName[0] == 0)
			return MSG_HANDLED_PROCEED;

	// get the drive part of the path string
	TCHAR drive[MAX_PATH];
	_tsplitpath( filePathName, drive, NULL, NULL, NULL );

	// set the relativePath variable if we found a drive segment in the path
	if ( drive[0] != '\0' )
	{
		// if there is a drive defined, so it is not a relative path
		bRelativePath = FALSE;
	}

	// Check files in the common directory
	// Only for loading, we don't save there... and relative paths only
	if ( isLoad && bRelativePath )
	{	
		// check file existence
		bool bFileExists = CheckFileExists( szFilePathName );

		// if the file does not exist check the common directory
		if ( !bFileExists ) 
		{
			// check if the member variable is already set or not
			if ( m_strDefaultResourcePath.empty() )
			{
				// get the resource path from the environment
				static DWORD msgHash_getDefResPath = CHashString(_T("GetEnvironmentDefaultResourcePath")).GetUniqueID();
				CHashString hszEnvironmentManager( _T( "CWin32EnvironmentManager" ) );

				// retval could be checked for errors
				retVal = m_ToolBox->SendMessage( msgHash_getDefResPath, sizeof( StdString ), &m_strDefaultResourcePath, NULL, &hszEnvironmentManager );
			}

			// if we are here, the m_strDefaultResourcePath is correctly set. 
			StdString newPathInDefautRes = m_strDefaultResourcePath;

			// append '\' only once between the two parts of the path
			// perhaps better check for it exists as well
			if ( szFilePathName.substr( 0, 1 ) != "\\" )
			{
				newPathInDefautRes += "\\";
			}

			// append the relative path to the default directory
			newPathInDefautRes += szFilePathName;
			
			// new solution to check file existence
			bFileExists = CheckFileExists( newPathInDefautRes );
		
			// if the new path is valid but not the original, replace the original.
			if( bFileExists )
			{
				szFilePathName = newPathInDefautRes;
			}
		}
	}

	// prepare the path for analization 
	TCHAR splittedfileName[_MAX_FNAME];
	TCHAR splittedFileExt[_MAX_EXT];

	_tsplitpath(filePathName, NULL, NULL, splittedfileName, splittedFileExt);
	
	// if not found, then no extension
	if ((splittedFileExt == NULL) || ((componentName = FindExtensionMatch(splittedFileExt, isLoad)) == NULL))
	{
		EngineGetToolBox()->SetErrorValue(WARN_NAME_NOT_FOUND);
		EngineGetToolBox()->Log(LOGWARNING, _T("Unable to find extenstion match %s for loading\n"), 
			filePathName);
		return MSG_ERROR;
	}

	if (isLoad)
	{
		// Load/Create new resource
		static DWORD msgHash_LoadFile = CHashString(_T("LoadFile")).GetUniqueID();
		PERFORMANCE_PROFILER_TYPE_START(filePathName, _T("File Loader"));
		retVal = m_ToolBox->SendMessage(msgHash_LoadFile, sizeof(LPTSTR), const_cast<LPTSTR>(filePathName), NULL, componentName);
		PERFORMANCE_PROFILER_TYPE_STOP(filePathName, _T("File Loader"));
		if (retVal != MSG_HANDLED)
		{
			toolBox->Log(LOGWARNING, _T("File Load error loading %s. error %d\n"), 
				filePathName, toolBox->GetErrorValue());
			return retVal;
		}
		
		// is the file that is being loaded for external use?
		if (!isInternal)
		{
			// send out message in the form FileLoaded_XXX, where XXX is the
			// extension of the saved file
			messageName = _T("FileLoaded_");
			// bypass '.'
			messageName += _tcslwr( splittedFileExt + 1 );
			// send the message
			DWORD msgHash_messageName = CHashString(messageName).GetUniqueID();
			m_ToolBox->SendMessage(msgHash_messageName, sizeof(LPTSTR), const_cast<LPTSTR>(filePathName));
		}
	}
	else
	{
		// save resource
		static DWORD msgHash_SaveFile = CHashString(_T("SaveFile")).GetUniqueID();
		retVal = m_ToolBox->SendMessage(msgHash_SaveFile, sizeof(LPTSTR), const_cast<LPTSTR>(filePathName), NULL, componentName);
		if (retVal != MSG_HANDLED)
		{
			toolBox->Log(LOGWARNING, _T("File Save error saving %s. Error %d\n"), 
				filePathName, toolBox->GetErrorValue());
			return retVal;
		}

		// send out message in the form FileSaved_XXX, where XXX is the
		// extension of the saved file
		messageName = _T("FileSaved_");
		// HOW COULD IT BE tExt AND tExt + 1 JUST AS FOR LOADING????
		//messageName += _tcslwr(const_cast<TCHAR*>(tExt));
		messageName += _tcslwr( splittedFileExt + 1 );
		// send the message
		DWORD msgHash_messageName = CHashString(messageName).GetUniqueID();
		m_ToolBox->SendMessage(msgHash_messageName, sizeof(LPTSTR), const_cast<LPTSTR>(filePathName));
	}

	return MSG_HANDLED_PROCEED;
}

DWORD CLoadSaveManager::UnloadFile(LPCTSTR filePathName)
{
	LPCTSTR tExt;
	IHashString *componentName;
	DWORD retVal;
	StdString messageName;
	BOOL pathPresent;
	StdString basePath, resourcePath;
	IToolBox *toolBox = EngineGetToolBox();

	// If handed a null string, (happens often enough in normal use not to warrant an error).
	// then exit out without typing anything
    if (filePathName)
		if (filePathName[0] == 0)
			return MSG_HANDLED_PROCEED;

	// if some path is present switch to resource directory to handle relative paths
	pathPresent = (_tcschr(filePathName, '\\') != NULL);
	if (pathPresent)
	{
		m_ToolBox->GetDirectories(&basePath, &resourcePath);
	}

	// figure out type of resource wanted, by extension
	// get last . to find start of extension
	tExt = _tcsrchr(filePathName, '.');
	
	// if not found, then no extension
	if ((tExt == NULL) || ((componentName = FindExtensionMatch(tExt, true)) == NULL))
	{
		EngineGetToolBox()->SetErrorValue(WARN_NAME_NOT_FOUND);
		EngineGetToolBox()->Log(LOGWARNING, _T("Unable to find extenstion match %s for unloading\n"), 
			filePathName);
		return MSG_ERROR;
	}

	
	// Load/Create new resource
	static DWORD msgHash_LoadFile = CHashString(_T("UnloadFile")).GetUniqueID();
	PERFORMANCE_PROFILER_TYPE_START(filePathName, _T("File Loader"));
	retVal = m_ToolBox->SendMessage(msgHash_LoadFile, sizeof(LPTSTR), const_cast<LPTSTR>(filePathName), NULL, componentName);
	PERFORMANCE_PROFILER_TYPE_STOP(filePathName, _T("File Loader"));
	if (retVal != MSG_HANDLED)
	{
		toolBox->Log(LOGWARNING, _T("File Unload error freeing %s. error %d\n"), 
			filePathName, toolBox->GetErrorValue());
		return retVal;
	}
	
	/*
	// send out message in the form FileLoaded_XXX, where XXX is the
	// extension of the saved file
	messageName = _T("FileUnloaded_");
	// bypass '.'
	messageName += _tcslwr(tExt+1);
	// send the message
	DWORD msgHash_messageName = CHashString(messageName).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_messageName, sizeof(LPTSTR), const_cast<LPTSTR>(filePathName));
	*/

	return MSG_HANDLED_PROCEED;
}


DWORD CLoadSaveManager::OnLoadFile(DWORD size, void *params)
{
	LOADFILEEXTPARAMS *lfep;

	VERIFY_MESSAGE_SIZE(size, sizeof(LOADFILEEXTPARAMS));
	lfep =(LOADFILEEXTPARAMS *)params;
	return LoadSaveFile(lfep->fileName, true, lfep->bInternalLoad);
}

DWORD CLoadSaveManager::OnSaveFile(DWORD size, void *params)
{
	LPTSTR filePathName;
	VERIFY_MESSAGE_SIZE(size, sizeof(LPTSTR));
	filePathName =(LPTSTR)params;
	return LoadSaveFile(filePathName, false);
}

DWORD CLoadSaveManager::OnUnloadFile(DWORD size, void *params)
{
	LOADFILEEXTPARAMS *lfep;
	VERIFY_MESSAGE_SIZE(size, sizeof(LOADFILEEXTPARAMS));
	lfep =(LOADFILEEXTPARAMS *)params;
	return UnloadFile(lfep->fileName);
}

DWORD CLoadSaveManager::OnRegisterLoadSave(DWORD size, void *params)
{
	LSRC_MESSAGE *lsrcMess;

	VERIFY_MESSAGE_SIZE(size, sizeof(LSRC_MESSAGE));
	lsrcMess = (LSRC_MESSAGE *)params;

	IHashString *compName = lsrcMess->m_ComponentName;
	// save off filter strings
	m_LoadSaveMap[compName->GetUniqueID()] = &lsrcMess->m_FilterStrings;

	// iterator to find the correct place to insert
	FILTERSTRINGSLIST::iterator filterStringIterator = m_FilterStrings.begin();
	
	// search for the place to insert
	while( filterStringIterator != m_FilterStrings.end() &&
		( _tcsicmp( ( *filterStringIterator )->m_LoadFilter , *(&lsrcMess->m_FilterStrings.m_LoadFilter) ) < 0 ) )
	{
		filterStringIterator++;
	}

	// insert the string in the list
	m_FilterStrings.insert(filterStringIterator , &lsrcMess->m_FilterStrings);
	//	m_FilterStrings.push_back(&lsrcMess->m_FilterStrings);

	return MSG_HANDLED_PROCEED;
}

DWORD CLoadSaveManager::OnGetFilterStrings(DWORD size, void *params)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(FILTERSTRINGSLIST **));
	*((FILTERSTRINGSLIST **)params) = &m_FilterStrings;

	return MSG_HANDLED_STOP;
}

IHashString *CLoadSaveManager::FindExtensionMatch(LPCTSTR ext, BOOL isLoad)
{
	LOADSAVEMAP::iterator lsrcIter;
	size_t tLen;
	LPCTSTR tStr;
	LPCTSTR testExt;			// the current tested extension is structure
	size_t extLen;

	// grab extenstion length
	extLen = _tcslen(ext);
	for (lsrcIter = m_LoadSaveMap.begin(); lsrcIter != m_LoadSaveMap.end();
			lsrcIter++)
	{
		FILTERSTRINGS *filterStrings;

		filterStrings = (FILTERSTRINGS *)lsrcIter->second;

		if (isLoad)
		{
			tStr = filterStrings->m_LoadFilter;
		}
		else
		{
			tStr = filterStrings->m_SaveFilter;
		}

		if (tStr != NULL)
		{
			while ((*tStr != '\0') && (*(tStr+1) != '\0'))
			{
				// get length of open/import filters
				tLen = _tcslen(tStr)+1;
				// bump past description strings
				tStr += tLen;
				testExt = _tcschr(tStr, '.');
				// search until end of string
				while (testExt != NULL)
				{
					// compare extension input with current seen extension
					if (!_tcsnicmp(testExt, ext, extLen))
					{
						DWORD HashID;
						const TCHAR *hashStr;
						HashID = lsrcIter->first;
						hashStr = m_ToolBox->GetHashString(HashID);
						if (hashStr != NULL)
						{
							m_hsExtComponentType = hashStr;
							return &m_hsExtComponentType;
						}
						else
						{
							return NULL;
						}
					}
					testExt = _tcschr(testExt+1, '.');
				}
				
				tStr += _tcslen(tStr)+1;
			}
		}

	}

	return NULL;
}

DWORD CLoadSaveManager::OnGetFileVersion(DWORD size, void *param)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(IHashString));
	IHashString *outVer = (IHashString*)param;
	*outVer = m_hszFileVersion.GetString();
	return MSG_HANDLED_STOP;
}

DWORD CLoadSaveManager::OnSetFileVersion(DWORD size, void *param)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(IHashString));
	IHashString *inVer = (IHashString*)param;
	m_hszFileVersion = inVer->GetString();
	return MSG_HANDLED_STOP;
}