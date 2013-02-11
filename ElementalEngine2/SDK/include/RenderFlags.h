///============================================================================
/// \file	IRenderFlags
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

#ifndef RENDERFLAGS_H
#define RENDERFLAGS_H

/// SHADER_RENDER_FLAG
/// Defines a SHADER_RENDERER_FLAG as a DWORD
typedef DWORD SHADER_RENDER_FLAG;

/// defines for SHADER_RENDER_FLAG
#define BASE_FLAG						0x0001
#define ALPHA							BASE_FLAG 
#define ALPHATEST						BASE_FLAG << 1
#define	LIGHT_RECEIVE_HEMI				BASE_FLAG << 2
#define	LIGHT_RECEIVE_OMNI				BASE_FLAG << 3
#define	LIGHT_RECEIVE_DIR				BASE_FLAG << 4
#define	SHADOW_RECEIVE_HEMI				BASE_FLAG << 5
#define	SHADOW_RECEIVE_OMNI				BASE_FLAG << 6
#define	SHADOW_RECEIVE_DIR				BASE_FLAG << 7
#define	SHADOW_CAST_HEMI				BASE_FLAG << 8
#define	SHADOW_CAST_OMNI				BASE_FLAG << 9
#define	SHADOW_CAST_DIR					BASE_FLAG << 10
#define	LIGHTMAP_RECEIVE				BASE_FLAG << 11
#define	LIGHTMAP_CAST					BASE_FLAG << 12
#define	SPHERICAL_HARMONICS				BASE_FLAG << 13
#define	AMBIENT_OCCLUSION				BASE_FLAG << 14
#define	HIGH_DYNAMIC_RANGE				BASE_FLAG << 15
#define	BONES_0							BASE_FLAG << 16
#define	BONES_1							BASE_FLAG << 17
#define	BONES_2							BASE_FLAG << 18
#define	BONES_3							BASE_FLAG << 19
#define	BONES_4							BASE_FLAG << 20
#define	FOG								BASE_FLAG << 21
#define	SHADOW_CAST						BASE_FLAG << 22
#define	SHADOW_RECEIVE					BASE_FLAG << 23
#define AMBIENT_OCCLUSION_CAST			BASE_FLAG << 24
#define AMBIENT_OCCLUSION_RECEIVE		BASE_FLAG << 25
#define SHADER_RF_LIGHT					BASE_FLAG << 26
#define SHADER_RF_GLOW					BASE_FLAG << 27
#define SHADER_RENDER_FLAG_MAX			28
//#define	RESERVED					BASE_FLAG << 28
//#define	RESERVED					BASE_FLAG << 29
//#define	RESERVED					BASE_FLAG << 30
//#define	RESERVED					BASE_FLAG << 31

/// ENUMSAMPLERSTATE
/// enumeration of the possible sampler state values
/// if this changes be sure to change the lookup arrays in all renderer plugins
enum ENUMSAMPLERSTATE
{
	SAMPLERSTATE_MINFILTER,
	SAMPLERSTATE_MAGFILTER,
	SAMPLERSTATE_MIPFILTER,
	SAMPLERSTATE_U,
	SAMPLERSTATE_V,
	SAMPLERSTATE_COUNT
};

/// ENUMTEXTURESTAGESTATE
/// enumeration of the possible texture stage types
/// NOTE: if this changes be sure to change the lookup arrays in all renderer plugins
enum ENUMTEXTURESTAGESTATE
{
	TEXTURESTAGESTATE_COLOROP,
	TEXTURESTAGESTATE_COLORARG1,
	TEXTURESTAGESTATE_COLORARG2,
	TEXTURESTAGESTATE_ALPHAOP,
	TEXTURESTAGESTATE_ALPHAARG1,
	TEXTURESTAGESTATE_ALPHAARG2,
	TEXTURESTAGESTATE_COUNT
};

/// TEXTURESTAGEPARAM
/// enumeration of texture stage params
/// NOTE: if this changes be sure to change the lookup arrays in all renderer plugins
enum TEXTURESTAGEPARAM
{
	TEXTURESTAGEOP_DISABLE,
	TEXTURESTAGEOP_MODULATE,
	TEXTURESTAGEOP_SELECTARG1,
	TEXTURESTAGEOP_SELECTARG2,
	TEXTURESTAGEARG_TEXTURE,
	TEXTURESTAGEARG_DIFFUSE,
	TEXTURESTAGE_TEXF_NONE,
	TEXTURESTAGE_TEXF_POINT,
	TEXTURESTAGE_TEXF_LINEAR,
	TEXTURESTAGE_TADDRESS_CLAMP,
	TEXTURESTAGE_TADDRESS_WRAP,
	TEXTURESTAGE_TADDRESS_MIRROR,
	TEXTURESTAGEPARAM_COUNT
};

/// ENUMRENDERSTATE
/// enumeration of render state flages
/// NOTE: if this changes be sure to change the lookup arrays in all renderer plugins
enum ENUMRENDERSTATE
{
	RENDERSTATE_ALPHATESTENABLE,
	RENDERSTATE_ALPHABLENDENABLE,
	RENDERSTATE_ALPHAREF,
	RENDERSTATE_ZWRITEENABLE,
	RENDERSTATE_ZENABLE,
	RENDERSTATE_CULLMODE,
	RENDERSTATE_FILLMODE,
	RENDERSTATE_POINTSIZE,
	RENDERSTATE_DEPTHBIAS,
	RENDERSTATE_SLOPESCALEDEPTHBIAS,
	RENDERSTATE_SRCBLEND,
	RENDERSTATE_DESTBLEND,
	RENDERSTATE_SCISSORTESTENABLE,
	RENDERSTATE_COLORWRITEENABLE,
	RENDERSTATE_COUNT
};

/// ENUMRENDERSTATEPARAM
/// enumeration of render state values params
/// NOTE: if this changes be sure to change the lookup arrays in all renderer plugins
enum ENUMRENDERSTATEPARAM
{
	RENDERSTATEPARAM_FALSE,
	RENDERSTATEPARAM_TRUE,
	RENDERSTATEPARAM_CULLNONE,
	RENDERSTATEPARAM_CULLCW,
	RENDERSTATEPARAM_CULLCCW,
	RENDERSTATEPARAM_CULLREGULAR = RENDERSTATEPARAM_CULLCCW, // cull regular synonymous with ccw
	RENDERSTATEPARAM_WIREFRAME,
	RENDERSTATEPARAM_SOLID,
	RENDERSTATEPARAM_ZTRUE,
	RENDERSTATEPARAM_ZFALSE,
	RENDERSTATEPARAM_SRCALPHA,
	RENDERSTATEPARAM_INVSRCALPHA,
	RENDERSTATEPARAM_ONE,
	RENDERSTATEPARAM_COUNT
};

#endif