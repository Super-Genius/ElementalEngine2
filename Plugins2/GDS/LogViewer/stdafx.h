///============================================================================
/// \file		stdafx.h
/// \brief		precompiled header
/// \date		02/15/2008
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

#ifndef LOGVIEWER_STDAFX_H
#define LOGVIEWER_STDAFX_H

#if _MSC_VER >= 1400
#define _CRT_SECURE_NO_DEPRECATE 1
#endif

#ifdef _DEBUG
// This define must occur before any headers are included.
//#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#define DEBUG_NEW new(_NORMAL_BLOCK, THIS_FILE, __LINE__)
#endif

// Exclude rarely-used stuff from Windows headers
#define WIN32_LEAN_AND_MEAN

#if _MSC_VER > 1400
#define _CRT_SECURE_NO_DEPRECATE 1
#endif

// Windows Header Files:
#include <windows.h>

// Elemental Engine Header Files:
#include "ElementalEngine.hpp"
#include "CObjectTemplate.hpp"
#include "CManager.h"
#include "Register.h"
#include "BaseMessages.h"
#include "IActionHandler.h"
#include "EEDefines.h"
#include "EE-SDK.h"

// LogViewer Plugin Header Files:
#include "LogViewerComponent.h"

#endif // #ifndef LOGVIEWER_STDAFX_H



