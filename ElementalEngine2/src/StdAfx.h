///============================================================================
/// \file	StdAfx.h
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

#define NOMINMAX
#include "Platform.h"

typedef float FLOAT;

#include "ElementalEngine.hpp"

// local non-interface headers
#include "EE-SDK.h"
#include "Singleton.h"
#include "CHashString.hpp"
#include "Register.h"
#include "BaseMessages.h"
#include "EEDefines.h"

#include "PerformanceProfiler.h"
#include "Config.h"
#include "CShadermanager.h"
#include "CEffect.h"
//Temp until we figure out how we want to handle these objects:
#include "CObjectSceneSettings.h"
#include "ShaderConstants.h"

#include "CTransform.h"

#include "CRenderManager.h"
#include "CRenderObject.h"
#include "CController.h"
#include "CGeometry.h"
#include "CController.h"
#include "CLightManager.h"
#include "CKeyFrame.h"
#include "CParticleType.h"
#include "CParticleManager.h"
#include "CParticleRenderObject.h"
#include "CParticleLineRenderObject.h"
#include "CParticleRibbonRenderObject.h"
#include "CParticleEmitter.h"
#include "CParticleEmitterObject.h"
#include "CParticleEmitterType.h"
#include "CParticleLineType.h"
#include "CParticleRibbonType.h"
#include "CParticleBatchRenderObject.h"
#include "CMaterialOverride.h"
#include "Vec3.h"
#include "CTimer.h"

