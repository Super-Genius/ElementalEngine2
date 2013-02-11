#include "Shaders\HLSLconstants.h"

struct VSIn
{
	float3 Position		: POSITION;
	float3 TexCoord0	: TEXCOORD0;
};

struct VSOut
{
	float4 HPosition	: POSITION;
	float3 TexCoord0	: TEXCOORD0;
};

uniform float4 EyePos	: register(c22);
uniform float4 ScaleZOffset : register(c23); // x(scale), y(zoffset)

VSOut main(	VSIn In )
{
	VSOut Out;

	// ScaleZOffset.x is an arbitrary value just to keep it from hitting the near clip plane
	float3 pos = (In.Position * ScaleZOffset.x) + EyePos.xyz;
	// zoffset is [0,1] so needs to be scaled to represent moving the eye from center of the box to the top
	pos.z -= ScaleZOffset.x * ScaleZOffset.y;
	float3 tc = In.TexCoord0;
//	tc.xyz = tc.xzy;
	Out.TexCoord0 = tc;
	Out.HPosition = mul(worldToScreen, float4(pos,1.0));

	return Out;
}
