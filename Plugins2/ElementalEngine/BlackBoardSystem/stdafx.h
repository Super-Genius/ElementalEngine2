///============================================================================
/// \file	stdafx.h
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

#pragma once

#include <assert.h>
#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>

#include "ElementalEngine.hpp"
#include "CHashString.hpp"
#include "Register.h"
#include "BaseMessages.h"
#include "BBKListener.h"
#include "BBKManager.h"
#include "KnowledgeBase.h"

/// Equivalent to AI Priority
#define LUAPRIORITY			0x0200

static DWORD TODWORD(int x) { return x; }