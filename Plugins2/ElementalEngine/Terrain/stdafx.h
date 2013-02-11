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

#ifndef _TERRAIN_STDAFX_H_
#define _TERRAIN_STDAFX_H_

#include "Platform.h"

#include <assert.h>

#include "ElementalEngine.hpp"
#include "EE-SDK.h"
#include "EEDefines.h"
#include "CHashString.hpp"
#include "Register.h"
#include "BaseMessages.h"
#include "CObjectTemplate.hpp"
#include "toluaxx.h"
#include "IRenderObject.h"
#include "lightmap/lightmapdefines.h"
#include "Cal3dModels/Cal3dModelsDefine.h"
#include "ShaderConstants.h"
#include "IRenderObject.h"
#include "ITextureObject.h"
#include "IMeshObject.h"
#include "IVertexBufferObject.h"
#include "IIndexBuffer.h"
#include "IEffect.h"
#include "Terrain/TerrainDefines.h"
#include "Physics/PhysicsDefines.h" // for raycasting

extern MathUtil g_Math;

#include "HeightmapObject.h"
#include "CTerrainConfig.h"
#include "CTerrainSector.h"
#include "CTerrainSectorRenderObject.h"
#include "CTerrainWaterRenderObject.h"
#include "CTerrainDecalRenderObject.h"
#include "CTerrainManager.h"

#endif	//#ifndef _TERRAIN_STDAFX_H_
