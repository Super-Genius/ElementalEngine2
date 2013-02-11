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
#include ".\particletoolbox.h"

CParticleToolBox::CParticleToolBox(void)
{
}

CParticleToolBox::~CParticleToolBox(void)
{
}


void CParticleToolBox::ExtractFileName(TCHAR * extractedfilename, const TCHAR * filepath)
{
	TCHAR drive[_MAX_DRIVE];
	TCHAR dir[_MAX_DIR];
	TCHAR fname[_MAX_FNAME];
	TCHAR ext[_MAX_EXT];

	_tsplitpath( filepath, drive, dir, fname, ext );

	StdString szOutput;
	szOutput += fname;
	szOutput += ext;

	_tcscpy(extractedfilename, szOutput);
}
void CParticleToolBox::BuildParticleDirectory(TCHAR * czDirectoryPath, const TCHAR * czEmitterPath)
{
	TCHAR czEmitterPath_Drive[_MAX_DRIVE];
	TCHAR czEmitterPath_Dir[_MAX_DIR];
	TCHAR czEmitterPath_Fname[_MAX_FNAME];

	TCHAR czTempPath[_MAX_PATH];

	_tsplitpath(czEmitterPath, czEmitterPath_Drive, czEmitterPath_Dir, czEmitterPath_Fname, NULL);

	_tmakepath(czTempPath, czEmitterPath_Drive, czEmitterPath_Dir, NULL, NULL);

	StdString szNewDirectoryPath(czTempPath);
	szNewDirectoryPath += czEmitterPath_Fname;

	_tcscpy(czDirectoryPath, szNewDirectoryPath);

}

void CParticleToolBox::BuildParticleBrushPath(TCHAR * czNewBrushPath, const TCHAR * czEmitterPath, const TCHAR * czOldBrushPath)
{
	TCHAR czOldBrushPath_Drive[_MAX_DRIVE];
	TCHAR czOldBrushPath_Dir[_MAX_DIR];
	TCHAR czOldBrushPath_Fname[_MAX_FNAME];
	TCHAR czOldBrushPath_Ext[_MAX_EXT];

	_tsplitpath(czOldBrushPath, czOldBrushPath_Drive, czOldBrushPath_Dir, czOldBrushPath_Fname, czOldBrushPath_Ext);

	TCHAR * lpcFormat = NULL;
	TCHAR * lpcBeginningOfBrushName = NULL;

	// this is to remove preceding _'s. 
	for (int i = 0; i < _MAX_PATH; i++)
	{
		lpcFormat = _tcsstr(czOldBrushPath_Fname, _T("_"));
		if (lpcFormat)
			lpcBeginningOfBrushName = lpcFormat;
		else
			break;
	}
	if (lpcBeginningOfBrushName)
		lpcBeginningOfBrushName += 1;
	else
		lpcBeginningOfBrushName = czOldBrushPath_Fname;

	TCHAR czEmitterPath_Drive[_MAX_DRIVE];
	TCHAR czEmitterPath_Dir[_MAX_DIR];
	TCHAR czEmitterPath_Fname[_MAX_FNAME];
	TCHAR czEmitterPath_Ext[_MAX_EXT];

	_tsplitpath(czEmitterPath, czEmitterPath_Drive, czEmitterPath_Dir, czEmitterPath_Fname, czEmitterPath_Ext);

	StdString szNewBrushFilename;
	szNewBrushFilename += czEmitterPath_Fname;
	szNewBrushFilename += _T('\\');
	szNewBrushFilename += lpcBeginningOfBrushName;

	_tmakepath(czNewBrushPath, czEmitterPath_Drive, czEmitterPath_Dir, szNewBrushFilename, czOldBrushPath_Ext);
}
