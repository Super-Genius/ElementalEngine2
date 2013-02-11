///============================================================================
/// \file		stdafx.h
/// \brief		precompiled header
/// \date		10/05/2007
/// \author		Kyle Swaim
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

#ifndef COORDINATETOOL_STDAFX_H
#define COORDINATETOOL_STDAFX_H

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

// Modify the following defines if you have to target a platform prior to the ones specified below.
// Refer to MSDN for the latest info on corresponding values for different platforms.
#ifndef WINVER				// Allow use of features specific to Windows 95 and Windows NT 4 or later.
#define WINVER 0x0400		// Change this to the appropriate value to target Windows 98 and Windows 2000 or later.
#endif

#include <afxwin.h>         // MFC core and standard components

// Elemental Engine Header Files:
#include "ElementalEngine.hpp"
#include "CObjectTemplate.hpp"
#include "CManager.h"
#include "Register.h"
#include "BaseMessages.h"
#include "IActionHandler.h"
#include "EEDefines.h"
#include "EE-SDK.h"
#include "Messages.h"

// Object Editor Header Files:
#include "ObjectEditor/ObjectEditorDefines.h"

#endif // #ifndef COORDINATETOOL_STDAFX_H
