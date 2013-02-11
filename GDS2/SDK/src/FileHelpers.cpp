///============================================================================
/// \file		FileHelpers.cpp
/// \brief		implementation for common interactions with FS.
/// \date		09-14-2007
/// \author		Andrey Ivanov
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

#include "FileHelpers.h"

namespace FileHelpers
{

/// \brief	Finds file extension in the passed path.
/// \param	szFilePath - path to the file
/// \return	pointer to the found extension or NULL if extension was not found
LPCTSTR GetExtension(LPCTSTR szFilePath)
{
	LPCTSTR szExt = _tcsrchr(szFilePath, _T('.'));
	if (szExt != NULL)
	{
		// skip the dot
		++szExt;
		// dot may be in folder name, but file part may not contain extension
		size_t nSlash = _tcscspn(szExt, _T("\\/"));
		if (szExt[nSlash] != _T('\0'))
		{
			return NULL;
		}
	}
	return szExt;
}

/// \brief	Builds filename from the passed path.
/// \param	szFilePath - path to the file
/// \return	string with filename and extension
StdString GetName(LPCTSTR szFilePath)
{
	ASSERT(szFilePath != NULL);

	TCHAR drive[_MAX_DRIVE];
	TCHAR dir[_MAX_DIR];
	TCHAR fname[_MAX_FNAME];
	TCHAR ext[_MAX_EXT];

	_tsplitpath(szFilePath, drive, dir, fname, ext);

	StdString szRes = fname;
	szRes += ext;
	return szRes;
}

/// \brief	Builds filename from the passed path.
/// \param	szFilePath - path to the file
/// \return	string with filename without extension
StdString GetNameWithoutExtension(LPCTSTR szFilePath)
{
	ASSERT(szFilePath != NULL);

	TCHAR drive[_MAX_DRIVE];
	TCHAR dir[_MAX_DIR];
	TCHAR fname[_MAX_FNAME];
	TCHAR ext[_MAX_EXT];

	_tsplitpath(szFilePath, drive, dir, fname, ext);

	return fname;
}

} //namespace FileHelpers
