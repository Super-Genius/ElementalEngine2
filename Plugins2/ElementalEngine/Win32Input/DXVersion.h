///============================================================================
/// \file	DXVersion.h
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

#ifndef DXVERSION_H__
#define DXVERSION_H__

#define DIRECTINPUT_VERSION 0x800

const int ERR_NOERROR        =     0;   // Everything is okay
const int ERR_ERROR          =    -1;   // Generic error. Something went wrong.
const int ERR_DINPUTERROR    = -2120;   // Generic DirectInput error
const int ERR_NODINPUT       = -2121;   // DirectInput could not be found (or initialized)
const int ERR_WRONGDINPUTVER = -2122;   // Wrong DirectInput version (too old)
const int ERR_NODINPUTDEVICE = -2123;   // No (such) DirectInput device was found (or initialized)

#endif