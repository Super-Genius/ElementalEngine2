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
#include <tchar.h>
#include <math.h>

#ifdef _DEBUG
// This define must occur before any headers are included.
//#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#define DEBUG_NEW new(_NORMAL_BLOCK, THIS_FILE, __LINE__)
#endif


#ifdef WIN32 // all this should be moved to a GDS Plugin...
#include <d3d9.h> //used for atlas generation
#include <d3dx9.h> //used for atlas generation
#include <d3dx9mesh.h>
#endif

#include "ElementalEngine.hpp"
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
#include <set>
#include "IMeshObject.h"
#include "IController.h"
#include "IIndexBuffer.h"
#include "ILightObject.h"
#include "IVertexBufferObject.h"
#include "math.h"
#include "Ray.h"
#include "AABB.h"
#include "..\..\include\lightmap\lightmapdefines.h"
#include "..\..\include\Cal3dModels\Cal3dModelsDefine.h"
#include "..\..\include\Shape\ShapeDefines.h"
#include "CMap.h"
#include "PhotonMap.h"
#include "kdtree.h"
#include "MeshParameterization.h"
#include "LightmapGenerator.h"
#include "LightMapObject.h"
#include "LightMapObjectManager.h"

// TODO: reference additional headers your program requires here
