///============================================================================
/// \file	FilePath.cpp
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
#include "FilePath.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace ElementalEngine
{

StdString ExtractDrive( const TCHAR *filepath )
{
	TCHAR drive[MAX_PATH];
	
	// this allows greater than a single letter drive
	const TCHAR *drivesplitdilm = _T(":\\");
	_tcscpy( drive, filepath );
	TCHAR *drivesplit = _tcsstr( drive, drivesplitdilm );
	if (drivesplit)
	{
		*drivesplit = '\0';
		_tcscat( drive, drivesplitdilm );		
	}
	else
		drive[0] = '\0';

	StdString szOutput = drive;
	return szOutput;
}

StdString ExtractFileName( const TCHAR *filepath )
{
	TCHAR drive[_MAX_DRIVE];
	TCHAR dir[_MAX_DIR];
	TCHAR fname[_MAX_FNAME];
	TCHAR ext[_MAX_EXT];

	_tsplitpath( filepath, drive, dir, fname, ext );

	StdString szOutput;
	szOutput += fname;
	szOutput += ext;

	return szOutput;
}

StdString SetPathDrive( const TCHAR *filepath, const TCHAR *newdrive )
{
	TCHAR drive[MAX_PATH];
	TCHAR dir[MAX_PATH];
	TCHAR fname[MAX_PATH];
	TCHAR ext[MAX_PATH];

	// this allows greater than a single letter drive
	const TCHAR *drivesplitdilm = _T(":\\");
	_tcscpy( drive, filepath );

	// look for :\ or not there then look for :/
	TCHAR *drivesplit = _tcsstr( drive, drivesplitdilm );
	if (!drivesplit)
	{
		drivesplitdilm = _T(":/");
		drivesplit = _tcsstr( drive, drivesplitdilm );
	}

	if (drivesplit)
	{
		*drivesplit = '\0';
		_tcscat( drive, drivesplitdilm );

		filepath = filepath+_tcslen(drive);
	}
	else
		drive[0] = '\0';

	_tsplitpath( filepath, NULL, dir, fname, ext );

	StdString szOutput;
	szOutput += newdrive;
	szOutput += dir;
	szOutput += fname;
	szOutput += ext;

	return szOutput;
}

bool CheckFileExists( const TCHAR *pFilePath )
{
	CHECKFILEEXISTS cfe;
	cfe.pFilePath = pFilePath;
	static DWORD msgHash_CheckFileExists = CHashString(_T("CheckFileExists")).GetUniqueID();
	if (EngineGetToolBox()->SendMessage(msgHash_CheckFileExists, sizeof(cfe), &cfe) == MSG_HANDLED)
	{
		return cfe.bExists != FALSE;
	}
	return false;
}

}
