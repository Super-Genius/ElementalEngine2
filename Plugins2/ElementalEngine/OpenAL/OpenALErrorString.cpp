///============================================================================
/// \file		OpenALErrorString.cpp
/// \brief		Simply converts ALenum error code into a TCHAR * string
/// \date		03/01/07
/// \author		Nick Tourte
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

const TCHAR* GetALErrorString(ALenum errCode)
{
	switch(errCode)
	{
		case AL_NO_ERROR:
		{
			return _T("No error");
		}
		case AL_INVALID_NAME:
		{
			return _T("Bad ID passed in");
		}
		case AL_INVALID_ENUM:
		{
			return _T("Invalid enum passed in");
		}
		case AL_INVALID_VALUE:
		{
			return _T("Invalid value passed in");
		}
		case AL_INVALID_OPERATION:
		{
			return _T("Invalid operation request");
		}
		case AL_OUT_OF_MEMORY:
		{
			return _T("OpenAL is out of memory");
		}
		default:
		{
			return _T("Mysterious and otherwise undocumented error");
		}
	}
}