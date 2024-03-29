#include "Shaders\HLSLconstants.h"
#include "Shaders\VSH_CommonFunctions.h"

struct VS_INPUT
{
	float4 Pos				: POSITION;
	float2 TexCoord			: TEXCOORD0;
	float4 Color			: COLOR0;
};

struct VS_OUTPUT
{
	float4 Pos					: POSITION;
	float2 TexCoord0			: TEXCOORD0;
	float4 Color				: COLOR0;
};

VS_OUTPUT main(VS_INPUT i)
{
	VS_OUTPUT o;
	o.Pos = mul(worldToScreen, i.Pos );
	o.TexCoord0 = i.TexCoord;
	o.Color = i.Color;
	return o;
}