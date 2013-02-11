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

#pragma once

#include "Platform.h"
#include "ElementalEngine.hpp"
#include "EE-SDK.h"
#include "EEDefines.h"
#include "LoadSaveMessages.hpp"
#include "CHashString.hpp"
#include "Register.h"
#include "BaseMessages.h"
#include "DNSMDefines.h"
#include "Vec3.h"
#include "Cal3DUtil.h"
#include "../../include/Cal3dModels/Cal3dModelsDefine.h"

/// Map of DWORD File Names to DWORD IObjects
typedef map<DWORD, DWORD> FILETOOBJECTMAP;