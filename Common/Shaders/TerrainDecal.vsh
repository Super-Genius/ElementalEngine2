#include "Shaders\HLSLconstants.h"
#include "Shaders\VSH_CommonFunctions.h"

struct VSIn
{
	float4 Position		: POSITION;
	float3 Normal		: NORMAL;
	float2 TexCoord0	: TEXCOORD0; // mask
	float2 TexCoord1	: TEXCOORD1; // tile
	float2 TexCoord2	: TEXCOORD2; // lightmap
};

struct VSOut
{
	float4 HPosition	: POSITION;
	float2 TexCoord0	: TEXCOORD0; // decal
	float4 Color		: COLOR0;
	float4 Fog			: COLOR1;
};

uniform float2 ZMinMax : register(c19);
uniform float4 Color		: register(c20);
uniform float4 TexTransform	: register(c21);
uniform float4x4 TexTransformMat	: register(c22);

VSOut main(	VSIn In )
{
	VSOut Out;

	Out.HPosition = mul(worldToScreen, In.Position);
	float4 tc = float4(In.TexCoord0.xy, 0.0, 1.0);
	
	float tcsize = TexTransform.z / TexTransform.w;
	tc.xy = tc.xy + TexTransform.xy;
	tc = mul(TexTransformMat, tc );
	tc.xy = tc.xy - TexTransform.xy;
	tc.xy = tc.xy * tcsize + 0.5;
	tc.xy = tc.xy + TexTransform.xy * tcsize;
	Out.TexCoord0 = tc.xy;
	Out.Color = Color;
	
	Out.Fog = float4(0,0,0,0);
	Out.Fog.x = CalcZ( Out.HPosition.z, ZMinMax.x, ZMinMax.y );
	return Out;
}
