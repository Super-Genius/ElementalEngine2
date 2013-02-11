///============================================================================
/// \file	ShaderConstants.h
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

// D3DConstantDefs.h

// Input stream definitions
#define V_POS		v0
#define V_NORM		v1
#define V_SxT		V_NORM
#define V_DIFF		V_NORM
#define V_SPEC		v2
#define V_TEX		v3
#define V_TEX_2		v4
#define V_TEX_3		v5
#define V_TEX_4		v6
#define V_BINORM	v7
#define V_S			V_BINORM
#define V_TANGENT	v8
#define V_T			V_TANGENT
#define V_BONE_IDX	v9
#define V_BONE_WGHT	v10
#define V_POS_2		v11
#define V_POS_3		v12
#define V_COL		v13

#define I_POS		0
#define I_NORM		1
#define I_DIFF		I_NORM
#define I_SPEC		2
#define I_TEX		3
#define I_TEX_2		4
#define I_TEX_3		5
#define I_TEX_4		6
#define I_BINORM	7
#define I_TAN		8
#define I_BONE_IDX	9
#define I_BONE_WGHT	10
#define I_POS_2		11
#define I_POS_3		12
#define I_COL		13

// Vertex Shader constant definitions
#define MAXBONESPERMESH 29
#define MAXSHADOWMAPS 2

// Transform definitions
#define C_WORLD_TO_SCREEN			0
#define C_MODEL_TO_WORLD			4
#define C_WORLD_TO_LIGHT			8
#define C_ZCLIP						15
#define C_BONE_START				20
#define C_CUSTOM_CONSTANTS_START	20

// Light Definitions
#define MAX_COMPLEX_LIGHTS			4
#define MAX_SIMPLE_LIGHTS			2
#define MAX_LIGHTS					(MAX_COMPLEX_LIGHTS+MAX_SIMPLE_LIGHTS)

#define C_HDR_AVERAGE_LUMINOSITY	4
#define C_LIGHT_START				5
#define C_SHADOWCOORD0				13
#define C_SHADOWCOORD1				14
#define C_LIGHT_AMBIENT				17

#define C_VERTEX_LIGHT_START		107

// Miscellaneous definitions
#define C_CONST_1			14					// This should ALWAYS be = { 0.0f, 0.25f, 0.5f, 1.0f }
#define C_FOG_INFO			15					// { start, end, unused, unused }

#define C_VAL_ONE			c[C_CONST_1].wwww
#define C_VAL_HALF			c[C_CONST_1].zzzz
#define C_VAL_ZERO			c[C_CONST_1].xxxx

// Pixel Shader constant definitions
#define C_PIX_WHITE			0
#define C_PIX_GRAY			1
#define C_PIX_BLACK			2
#define C_PIX_AMBIENT		3
#define C_PIX_DIFFUSE		4
#define C_PIX_SPECULAR		5
#define C_PIX_HFOG_COLOR	6
#define C_PIX_SHADOW_ID		7
#define C_PIX_UTIL			6

// Functions

// Stores normalized 3 vector in _dst, and distance in _scratch
#define VS_NORMALIZE( _dst )	dp3 _dst.w, _dst, _dst	\
								rsq _dst.w, _dst.w		\
								mul _dst, _dst, _dst.w

// Will add lighting contribution to register specified in _dst
#define VS_DIRECTIONAL_LIGHT( _dst, _normal, _lightdir, _lightinfo1, _lightinfo2, _scratch )

// Will add lighting contribution to register specified in _dst
#define VS_POINT_LIGHT( _dst, _normal, _lightinfo1, _lightinfo2, _scratch1, _scratch2, _scratch3 )