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

#ifndef _OBJECTCUBMAP_STDAFX_H_
#define _OBJECTCUBMAP_STDAFX_H_

#include "Platform.h"

//between scenevisibility and renderpriorities
#define OBJECTCUBEMAPPRIORITY		0x05EC


#include "ElementalEngine.hpp"
#include "EE-SDK.h"
#include "mathutils.h"
#include "EEDefines.h"
#include "CHashString.hpp"
#include "Register.h"
#include "BaseMessages.h"
#include "CObjectTemplate.hpp"
#include "IRenderObject.h"
#include "ShaderConstants.h"
#include "Plane.h"
#include "Vec3.h"
#include "EulerAngle.h"
#include "matrix3x3.h"
#include "Register.h"
#include "BaseMessages.h"
#include <math.h>
typedef set<DWORD> DWORDSET;
#include "CManager.h"
#include "PerformanceProfiler.h"

#include <vector>
using namespace std;

#include "ObjectCubeMapDefines.h"
#include "CubeMapVisitor.h"
#include "ObjectCubeMap.h"
#include "ObjectCubeMapManager.h"
#include "ObjectCubeGenerator.h"

#endif		// #ifndef _OBJECTCUBMAP_STDAFX_H_
