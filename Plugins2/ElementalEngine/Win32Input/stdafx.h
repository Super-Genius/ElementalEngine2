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

#ifndef STDAFX_98CE7448_562D_4460_A592_9168D579DED4_H
#define STDAFX_98CE7448_562D_4460_A592_9168D579DED4_H

#include "Platform.h"

#include <assert.h>
#include <tchar.h>

#ifdef _DEBUG
// This define must occur before any headers are included.
//#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#define DEBUG_NEW new(_NORMAL_BLOCK, THIS_FILE, __LINE__)
#endif

#define DIRECTINPUT_VERSION 0x800

#include "ElementalEngine.hpp"
#include "EEDefines.h"
#include "CHashString.hpp"
#include "CObjectTemplate.hpp"
#include "Register.h"
#include "BaseMessages.h"
#include "CWin32JoystickHandler.h"
#include "CInput.h"

#endif	// #ifndef STDAFX_98CE7448_562D_4460_A592_9168D579DED4_H