///==========================================================================
/// \file	stdafx.h
/// \brief	include file for standard system include files,
///			or project specific include files that are used frequently, but
///			are changed infrequently
/// \date	9/22/2006
/// \author	Kyle Swaim
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
///==========================================================================

#pragma once

#define NOMINMAX
#include "Platform.h"

#include <assert.h>
#include <tchar.h>
#include <math.h>

#ifdef _DEBUG
// This define must occur before any headers are included.
//#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#define DEBUG_NEW new(_NORMAL_BLOCK, THIS_FILE, __LINE__)
#endif

// Elemental Engine Header Files
#include "ElementalEngine.hpp"
#include "EE-SDK.h"
#include "CObjectTemplate.hpp"
#include "EEDefines.h"
#include "CHashString.hpp"
#include "Register.h"
#include "BaseMessages.h"
#include "IPhysObject.h"
#include "Vec3.h"
#include "Matrix3x3.h"
#include "Matrix4x4.h"
#include "Quaternion.h"
#include "MathUtils.h"

#include "..\..\include\Cal3dModels\Cal3dModelsDefine.h"

// PhysX includes
#include "NxPhysics.h"
#include "NxCooking.h"
#include "NxUserContactReport.h"

// PhysX Tools Header Files
#include "MemoryStream.h"
#include "NXU_Helper.h"

#include "Physics/NovodexStructs.h"

// Plugins Header Files
#include "toluaxx.h"
#include "Physics/PhysicsDefines.h"
#include "Terrain/TerrainDefines.h"
#include "NodeSystem/NodeSystemStructs.h"
#include "QHStateMachine/QHStateMachineDefines.h"
#include "Shape/ShapeDefines.h"

// Physics Plugin Header Files
#include "NovodexMathAdapter.h"
#include "CLuaRayCollisionList.h"

#include "IPhysicsJoint.h"
#include "CPhysicsJoint.h"

#include "IPhysicsActor.h"
#include "CPhysicsActor.h"

#include "IPhysicsCloth.h"
#include "CPhysicsCloth.h"

#include "IPhysicsObject.h"
#include "CPhysicsObject.h"

#include "PhysicsEvents.h"

#include "CHeightfieldPhysicsObject.h"
#include "CTriggerPhysicsObject.h"
#include "CBoundaryPhysicsObject.h"
#include "CPickingPhysicsObject.h"
#include "CEECharacterPhysicsObject.h"
#include "IPhysicsManager.h"
#include "CPhysicsManager.h"
#include "CRaycastReport.h"
#include "CTriggerReport.h"
#include "CContactReport.h"
#include "CPhysicsReport.h"
#include "UserAllocator.h"
#include "CPhysicsObjectCreateCallback.h"
#include "CCoordinateToolPhysicsObject.h"