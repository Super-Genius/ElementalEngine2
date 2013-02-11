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

#include <StdAfx.h>

#include "SaveAsHelper.h"

static TCHAR SaveFileName[_MAX_PATH];

static OPENFILENAME Ofn = {
	sizeof(OPENFILENAME),
	0,
	NULL,
	NULL,
	NULL,
	0,
	0,
	SaveFileName,
	_MAX_PATH,
	NULL,
	0,
	NULL,
	NULL,
    OFN_EXPLORER | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
	0,
	0,
	NULL,
	NULL,
	NULL,
	NULL
};

// here we need to select only our file types to save as.
TCHAR *SaveFileAsByExtension(TCHAR *extensionInfo)
{
	TCHAR curExt[_MAX_EXT];
	TCHAR newExt[_MAX_EXT];
	TCHAR newDrive[_MAX_DRIVE];
	TCHAR newPath[_MAX_PATH];
	TCHAR newName[_MAX_FNAME];
	TCHAR *oldFName;
	Ofn.lpstrFilter = extensionInfo;
	SaveFileName[0] = '\0';
	Ofn.lpstrTitle = "Save File As";
	Ofn.hwndOwner = NULL;
	if (!GetOpenFileName(&Ofn))
	{
		// cancel selected.
		return NULL;
	}

	// force extension, if it doesn't exist
	_tsplitpath(SaveFileName, newDrive, newPath, newName, newExt);
	// skip description string
	oldFName = extensionInfo + _tcslen(extensionInfo)+1;
	// find what extension we must have
	_tsplitpath(oldFName, NULL, NULL, NULL, curExt);
	// replace bad extensions	
	if (_tcsicmp(newExt, curExt))
	{
		sprintf(SaveFileName, _T("%s%s%s%s"), newDrive, newPath,
			newName, curExt);
	}

	return SaveFileName;
}
