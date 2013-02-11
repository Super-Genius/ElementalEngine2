///============================================================================
/// \file		OpenALErrorString.h
/// \brief		Simply converts ALenum error code into a TCHAR * string
/// \date		03/01/07
/// \author		Nick Tourte
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

#ifndef _OPENAL_ERROR_STRING_H_
#define _OPENAL_ERROR_STRING_H_

/// Converts al error code into english string
/// \param errCode = relevant error code
/// \return string translation of code
const TCHAR* GetALErrorString(ALenum errCode);

#endif //_OPENAL_ERROR_STRING_H_