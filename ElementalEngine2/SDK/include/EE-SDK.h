///==========================================================================
/// \file	EE-SDK.h
/// \brief	Primary include file for the EE SDK library.
/// \date	08/03/2005
/// \author	Brian Bazyk
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
///==========================================================================

#ifndef EE_SDK_H__
#define EE_SDK_H__

#include "CPoolAllocator.h"
#include <float.h>
#include <math.h>
#include "CObjectTemplate.hpp"
#include "Vec3.h"
#include "Vec4.h"
#include "Quaternion.h"
#include "Matrix3x3.h"
#include "Matrix4x4.h"
#include "MathUtils.h"
#include "Ray.h"
#include "AABB.h"
#include "Plane.h"
#include "EulerAngle.h"
#include "Random.h"
#include "Frustum.h"
#include "IHashString.hpp"
#include "CManagerListener.h"
#include "CManager.h"
#include "ProxyObject.h"
#include "ISceneObject.h"
#include "CSceneObject.h"
#include "ExceptionHandler.h"
#include "FilePath.h"
#include "IMeshObject.h"
#include "CMeshDescriptors.h"

#ifdef WIN32
#ifdef EE_SDK_EXPORTS
#else
#endif
#endif

#endif