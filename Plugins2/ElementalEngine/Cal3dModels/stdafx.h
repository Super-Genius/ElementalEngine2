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

#include "Platform.h"

#include <assert.h>

#include "ElementalEngine.hpp"
#include "EE-SDK.h"
#include "mathutils.h"
#include "EEDefines.h"
#include "CHashString.hpp"
#include "Register.h"
#include "BaseMessages.h"
#include "CObjectTemplate.hpp"
#include "IRenderObject.h"
#include "Vec3.h"
//#include "IKeyFrame.h"
#include "toluaxx.h"
#include "Matrix4x4.h"
#include "matrix3x3.h"
#include "Quaternion.h"
#include <list>
#include <vector>
#include <stack>
#include "IMeshObject.h"
#include "IController.h"
#include "IIndexBuffer.h"
#include "ILightObject.h"
#include "IVertexBufferObject.h"
#include "Ray.h"
#include "AABB.h"
#include "ShaderConstants.h"
#include "..\..\include\Cal3dModels\Cal3dModelsDefine.h"
#include "..\..\include\Lightmap\LightmapDefines.h"
#include "..\..\include\QHStateMachine\QHStateMachineDefines.h"

#ifndef CAL_16BIT_INDICES
#define CAL_16BIT_INDICES //make sure we use 16 bit indices!!!
#endif
#include "cal3d/cal3d.h"
#include "cal3d/animcallback.h"

#include "Cal3DDataStream.h"
#include "Cal3dCallback.h"
#include "Cal3DRenderObject.h"
#include "Cal3DCoreModelCache.h"
#include "Cal3DCoreAnimationCache.h"
#include "Cal3DCoreSkeletonCache.h"
#include "Cal3DCoreMeshCache.h"
#include "Cal3DModelManager.h"

// TODO: reference additional headers your program requires here
