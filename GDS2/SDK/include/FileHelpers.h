///============================================================================
/// \file		FileHelpers.h
/// \brief		Provides helper functions for common interactions with FS.
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

#ifndef _GDS_FS_HELPERS_H_
#define _GDS_FS_HELPERS_H_

#include "StdString.h"

namespace FileHelpers
{
/// \brief	Finds file extension in the passed path.
/// \param	szFilePath - path to the file
/// \return	pointer to the found extension or NULL if extension was not found
LPCTSTR GetExtension(LPCTSTR szFilePath);

/// \brief	Builds filename from the passed path.
/// \param	szFilePath - path to the file
/// \return	string with filename and extension
StdString GetName(LPCTSTR szFilePath);

/// \brief	Builds filename from the passed path.
/// \param	szFilePath - path to the file
/// \return	string with filename without extension
StdString GetNameWithoutExtension(LPCTSTR szFilePath);

};

#endif //_GDS_FS_HELPERS_H_