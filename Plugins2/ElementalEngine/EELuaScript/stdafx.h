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

#include <assert.h>

#include "ElementalEngine.hpp"
#include "EE-SDK.h"
#include "EEDefines.h"
#include "CHashString.hpp"
#include "EEMemory.h"
#include "Register.h"
#include "BaseMessages.h"
#include "EELuaScript\EELuaScriptDefines.h"
#include "PerformanceProfiler.h"
#include "IVariableList.hpp"
#include "LuaDefines.h"
#include "ILuaArchive.h"
#include "ILuaScript.h"

#include "Vec3.h"
#include "Vec4.h"
#include "Matrix3x3.h"
#include "Matrix4x4.h"
#include "Quaternion.h"
#include "EulerAngle.h"
#include "MathUtils.h"

#include "IDatabase.h"
#include "DatabaseDefines.hpp"
#include "toluaxx.h"

#include "LuaVec3.h"
#include "LuaVec4.h"
#include "LuaMatrix3x3.h"
#include "LuaMatrix4x4.h"
#include "LuaQuaternion.h"
#include "LuaEulerAngle.h"
#include "LuaMath.h"

#include "LuaArchive.h"
#include "LuaToolBox.h"
#include "LuaHashString.h"
#include "LuaVariableList.h"
#include "LuaDatabase.h"
#include "LuaTimer.h"

#include "CManager.h"
#include "LuaManager.h"
#include "LuaScriptVar.h"
#include "LuaScriptVarList.h"
#include "LuaScript.h"
#include "LuaDWORDSet.h"
#include "LuaHashStringList.h"

#include "Random.h"
#include "LuaRandUtil.h"

#include "VisitNodeDWORD.h"
#include "VisitNodeName.h"
#include "LuaVisitor.h"
#include "LuaStackCheck.h"

/// Equivalent to AI Priority
#define LUAPRIORITY			0x0200
