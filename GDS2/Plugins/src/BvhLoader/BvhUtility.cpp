#include"StdAfx.h"
#include"BvhUtility.h"
HANDLE g_hFileLock = NULL;
const TCHAR g_szFileLock[] = _T("BvhLoader FileLock");

//-----------------------------------------------------------------------------
HANDLE OpenFile( TCHAR *szFileName )
{
	// open the file, keep trying until it's not INVALID_HANDLE_VALUE
	// There has to be a better way than this.  I mean, I want to be able
	// to block the thread until the file is available again.  I don't know
	// if there's any API for that...
	HANDLE hFile = INVALID_HANDLE_VALUE;
	while( hFile == INVALID_HANDLE_VALUE )
	{
		// we only try to open the file if we own the hFileLock mutex.
		g_hFileLock = CreateMutex( NULL, FALSE, g_szFileLock );
	
		ASSERT( WaitForSingleObject( g_hFileLock, INFINITE ) == WAIT_OBJECT_0 );

		hFile = CreateFile(
			szFileName,
			GENERIC_READ,
			FILE_SHARE_READ,
			NULL,
			OPEN_EXISTING,
			FILE_ATTRIBUTE_NORMAL,
			NULL );
		Sleep( 0 );

		ReleaseMutex( g_hFileLock );
	}

	return hFile;
}