#include "Shaders\ShaderConstants.h"
// HLSL version of vertex shader constants
//#include "SDK/ShaderConstants.h //this is for matching constants to the ones used by the engine. This HLSL constant
//header should match what the shaderconstants.h defines for its matrices, but the shaderconstants.h is for asm shader defines

#ifndef __HLSL_CONSTANTS__
#define __HLSL_CONSTANTS__

// #define constant defaults
#ifndef ACTIVELIGHTS
#define ACTIVELIGHTS 0
#endif

#ifndef DETAILLIGHTS
#define DETAILLIGHTS 0
#endif

#ifndef SHADOWMAPS
#define SHADOWMAPS 0
#endif

#define PI 3.1415926535897932384626433832795

//VERTEX SHADER
row_major float4x4 worldToScreen : register (c0);
row_major float4x4 modelToWorld : register (c4);
row_major float4x4 worldToLight : register (c8);
float4 shadowcoord[MAXSHADOWMAPS] : register(c13);
float4 zclip : register(c15);

row_major float3x4 boneMatrix[MAXBONESPERMESH]: register (c20);

float4 VertexLights[MAX_LIGHTS]		: register(c107);

//PIXEL SHADER
float4 hdrAverageLuminosity : register(c4);
float4 lightData[MAX_LIGHTS*2] : register(c5);
float4 lightAmbient : register(c17);

#endif //__HLSL_CONSTANTS__