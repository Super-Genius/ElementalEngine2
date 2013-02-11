///============================================================================
/// \file		Win32EnvironmentManager.cpp
/// \brief		Implementation of Win32 Environment
/// \date		06-27-2008
/// \author		Zsolt Matyas
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

#include "StdAfx.h"
// ShlObj.h is used by the windows directory functions to retrieve the
// common program files directory.
#include "shlobj.h"

// Singleton component registration
REGISTER_COMPONENT_SINGLETON(CWin32EnvironmentManager);

// register our message handler
REGISTER_MESSAGE_HANDLER(GetEnvironmentDefaultResourcePath, OnGetDefaultResourcePath, CWin32EnvironmentManager);
REGISTER_MESSAGE_HANDLER(EnvironmentFindFile, OnWin32FindFile, CWin32EnvironmentManager);
REGISTER_MESSAGE_HANDLER(EnvironmentCloseFileSearch, OnCloseFileSearch, CWin32EnvironmentManager);

CWin32EnvironmentManager::CWin32EnvironmentManager() : 
	CManager(_T("CWin32EnvironmentManager"), 0, 0 )
{
	// new path variable
	TCHAR strPath[_MAX_PATH];
	
	// get the Common Files Path
	SHGetSpecialFolderPath( 0, strPath, CSIDL_PROGRAM_FILES_COMMON, FALSE );

	// create the stdString variable
	StdString szResourcePath( strPath );

	// Concatenate our specific directory
	szResourcePath += "\\Elemental Engine II";

	// store the result in the member variable 
	m_strDefaultResourcePath = szResourcePath;
}

CWin32EnvironmentManager::~CWin32EnvironmentManager()
{
	DeInit();
}

void CWin32EnvironmentManager::Update( DWORD tickCount )
{
	// we dont need to update anything now
}

IComponent *CWin32EnvironmentManager::Create(int nArgs, va_list argptr)
{
	return SINGLETONINSTANCE( CWin32EnvironmentManager );
}

DWORD CWin32EnvironmentManager::PumpHandler(DWORD size, void *data, IHashString *name, IHashString *compType, MSGFUNCTION msgFunction)
{
	// only pump messages for certain types
	static CHashString cwin32EnvironmentTypeName(_T("CWin32EnvironmentManager"));
	if (compType)
	{
		if (*compType == cwin32EnvironmentTypeName)
			return CManager::PumpHandler(size,data,name,compType,msgFunction);
	}
	return MSG_NOT_HANDLED;
}

DWORD CWin32EnvironmentManager::OnGetDefaultResourcePath(DWORD size, void *params)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(StdString));
	
	// set the return value
	StdString &returnParam = *(StdString*)params;

	// return our member variable
	returnParam = m_strDefaultResourcePath;

	return MSG_HANDLED_PROCEED;
}

DWORD CWin32EnvironmentManager::OnWin32FindFile(DWORD size, void *param)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(FINDFILE));
	FINDFILE &cfe = *(FINDFILE*)param;

	if ( cfe.pInternalData == NULL )
	{
	 	return Win32FindFirstFile( &cfe );
	}
	else
	{
		return Win32FindNextFile ( &cfe );
	}

}

DWORD CWin32EnvironmentManager::Win32FindFirstFile(FINDFILE *cfe)
{
	//TODO: Check relative paths...
	// if it is a relative path, perhaps we whould convert it into a full path...

	BOOL invalidparam = false;

	TCHAR *filePathArray = new TCHAR[MAX_PATH]();
	TCHAR *fileNameArray = new TCHAR[_MAX_FNAME]();

	*filePathArray = '\0';
	*fileNameArray = '\0';

	//Check for NULL and empty strings
	if ( ( cfe->pFileName == NULL ) |  // FileName is NULL (at least it should be '*'
		( _tcslen( cfe->pFileName ) == 0 ) )// Filename length 0 is illegal 
	{
		invalidparam = TRUE;
		cfe->Result = FINDFILE_ERROR_INVALID_FILENAME;
	}
		
	//check length if pFilePath + pFileName exceeds MAX_PATH 
	int pLength = 0; 
	if ( cfe->pFilePath != NULL )
	{
		pLength = (int) _tcslen( (cfe->pFilePath) ); 
	}
	pLength += (int) _tcslen(  cfe->pFileName );
	
	if ( pLength > MAX_PATH ) // path is too long
	{
		cfe->Result = FINDFILE_ERROR_PATH_LENGTH;
		invalidparam = TRUE;
	}

	if (invalidparam) return MSG_HANDLED_STOP;

	// TODO: MORE CLEVER PATH LEGNTH calculation is possible...

	//TODO: Check trailing '\' in filename (it is an illegal parameter
	// for findFirstFile() 

	unsigned int pathLength = 0;

	// construct our full path
	if( cfe->pFilePath != NULL )
	{
		// get the length of the path
		pathLength = (int) strlen( cfe->pFilePath );

		// copy the path
		_tcscpy( filePathArray, cfe->pFilePath );

		// append the '\' to delimit file if necessary
		if( &cfe->pFilePath[pathLength-1] == "\\" )
		{
			_tcscat( filePathArray, "\\" );
		}
	}

	// concatenate filename
	_tcscat( filePathArray, cfe->pFileName );
 
	// call win32 search
	WIN32_FIND_DATA FileData;
	HANDLE hSearch = FindFirstFile( filePathArray, &FileData); 

	// FindFirstFile Failed
	if (hSearch == INVALID_HANDLE_VALUE) 
	{ 
		// check if failed because there are no matching files
		if ( GetLastError() == ERROR_FILE_NOT_FOUND )
		{
			cfe->Result = FINDFILE_NOTFOUND;
		}
		else
		{
			// any other error
			cfe->Result = FINDFILE_ERROR_UNKNOWN;
		}
		return MSG_HANDLED_PROCEED;
	} 
	else
	{
		//TODO: If we match '.' or '..' we should skip them. In this case
		// findNextFile call is required.

		_tcscpy(fileNameArray, FileData.cFileName);

		cfe->bDirectory = ( FileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY );
		cfe->bExists = TRUE;
		cfe->Result = FINDFILE_OK;
		cfe->bReadOnly = ( FileData.dwFileAttributes & FILE_ATTRIBUTE_READONLY );
		TCHAR rdrive[ _MAX_DRIVE ];
		TCHAR rpath[ MAX_PATH ];
		_splitpath( filePathArray, rdrive, rpath, NULL, NULL );
		
		*filePathArray = '\0';
		_tcscpy( (TCHAR *) filePathArray , rdrive );
		_tcscat( (TCHAR *) filePathArray , rpath );
		
		// set the return values to our allocated memory containers
		cfe->pFileName = fileNameArray;
		cfe->pFilePath = filePathArray;
		cfe->pInternalData = hSearch;
	}

	if (cfe->bExists)
		return MSG_HANDLED_STOP;
	else
		return MSG_HANDLED_PROCEED;
}

DWORD CWin32EnvironmentManager::Win32FindNextFile(FINDFILE* cfe)
{
	HANDLE handle = (HANDLE)cfe->pInternalData;

	// check the incoming handle
	if( handle == INVALID_HANDLE_VALUE )
	{
		// check if failed because there are no matching files
		if ( GetLastError() == ERROR_FILE_NOT_FOUND )
		{
			cfe->Result = FINDFILE_NOTFOUND;
		}
		else
		{
			// any other error
			cfe->Result = FINDFILE_ERROR_UNKNOWN;
		}
		return MSG_HANDLED_PROCEED;
	}

	// proceed search
	WIN32_FIND_DATA FileData;
	BOOL searchResult = FindNextFile( handle, &FileData );

	if( searchResult ) 
	{
		_tcscpy(cfe->pFileName, FileData.cFileName);

		cfe->bDirectory = ( FileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY );
		cfe->bExists = TRUE;
		cfe->Result = FINDFILE_OK;
		cfe->bReadOnly = ( FileData.dwFileAttributes & FILE_ATTRIBUTE_READONLY );

		// TODO: The path is not updated. We return the one we got as incomming parameter.
		// To get the path of the current file we need to use the handle...

//		TCHAR rdrive[ _MAX_DRIVE ];
//		TCHAR rpath[ MAX_PATH ];
//		_splitpath( filePathArray, rdrive, rpath, NULL, NULL );
		
	
//		*(cfe->filePath_array) = '\0';
//		_tcscpy( (cfe->pFilePath) , rdrive );
//		_tcscat( (cfe->pFilePath) , rpath );
		
		// set the return values to our allocated memory containers
		cfe->pInternalData = handle;
	}
	else
	{
		cfe->bExists = FALSE;
		cfe->Result = FINDFILE_NOTFOUND;

		// if we did not found the file, no other plugin will find it. 
		// let's force finishing the search by stopping the message loop:
		return MSG_HANDLED_STOP;
	}

	//perhaps a do-while loop here to skip '.' and '..' entries.
	
	return MSG_HANDLED_PROCEED;
}

DWORD CWin32EnvironmentManager::OnCloseFileSearch(DWORD size, void *param)
{
	// get our necessary variables
	VERIFY_MESSAGE_SIZE(size, sizeof(FINDFILE));
	FINDFILE *cfe = (FINDFILE*)param;
	HANDLE handle = (HANDLE)cfe->pInternalData;
	
	// Close the search handle and free our allocations
	FindClose( handle );
	free(cfe->pFileName);
	free(cfe->pFilePath);

	return MSG_HANDLED_PROCEED;
}