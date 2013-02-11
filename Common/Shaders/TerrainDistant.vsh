#include "Shaders\HLSLconstants.h"
#include "Shaders\VSH_CommonFunctions.h"

struct VSIn
{
	float4 Position		: POSITION;
	float3 Normal		: NORMAL;
	float3 Tangent		: TANGENT;
	float2 TexCoord0	: TEXCOORD0; // mask
	float2 TexCoord1	: TEXCOORD1; // tile
	float2 TexCoord2	: TEXCOORD2; // lightmap
};

struct VSOut
{
	float4 HPosition		: POSITION;
	float4 PosOutDepth		: COLOR0;
};

uniform float2 ZMinMax		: register(c19);

VSOut main(	VSIn IN )
{
	VSOut OUT;

	OUT.HPosition = mul( worldToScreen, float4(IN.Position.xyz,1.0));
	OUT.PosOutDepth.xyz = IN.Position.xyz;
	OUT.PosOutDepth.w = CalcZ( OUT.HPosition.z, ZMinMax.x, ZMinMax.y );

	return OUT;
}
