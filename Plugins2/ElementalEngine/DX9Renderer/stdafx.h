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

#if (defined _DEBUG)
#define D3D_DEBUG_INFO
#endif
#include <d3d9.h>
#include <d3dx9.h>
#include <stack>
#include "rgbe.h"
#include "ElementalEngine.hpp"
#include "EE-SDK.h"
#include "CRenderObject.h"
#include "mathutils.h"
#include "EEDefines.h"
#include "CHashString.hpp"
#include "Register.h"
#include "BaseMessages.h"
#include "CObjectTemplate.hpp"
#include "IRenderObject.h"
#include "Config.h"
#include "ShaderConstants.h"
#include "Plane.h"
#include "Vec3.h"
#include "Matrix4x4.h"
#include "matrix3x3.h"
#include "Vec3.h"
#include "Vec4.h"
#include "toluaxx.h"
#include "Quaternion.h"
#include "IMoviePlayer.h"
#include "CDX9VertexBufferObject.h"
#include "CDX9ShaderCommon.h"
#include "CDX9PixelShader.h"
#include "CDX9VertexShader.h"
#include "CDX9Font.h"
#include "CDX9FontImplementation.h"
#include "CDX9Renderer.h"
#include "CDX9RenderContext.h"
#include "DX9Platform.h"
//#include "..\..\include\DX9Renderer\DX9RendererDefine.h"

// TODO: reference additional headers your program requires here
