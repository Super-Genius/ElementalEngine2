///============================================================================
/// \file		DLLVersion.h
/// \date		07-14-2005
/// \author		Kenneth Hurley
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

#ifndef _DLLVERSION_H_
#define _DLLVERSION_H_



#ifdef _DEBUG
// major version in upper 16 bits
// low word =  minor version, low bit debug =0, release=1
#define INTERFACEDLLVERSION 0x00006100
#else
#define INTERFACEDLLVERSION 0x00006101
#endif

#endif // #ifndef _DLLVERSION_H_

#ifdef DLLIMPORTING
extern "C" __declspec(dllimport) DWORD CDECL GetDLLVersion();
#else
extern "C" __declspec(dllexport) DWORD CDECL GetDLLVersion();
#endif


