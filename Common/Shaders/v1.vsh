#include "Shaders\HLSLconstants.h"

struct VS_INPUT
{
	float4  Pos             : POSITION;
	float2  TexCoord        : TEXCOORD;
};

struct VS_OUTPUT
{
	float4  Pos			: POSITION;
	float2  TexCoord0	: TEXCOORD0;
};
	
VS_OUTPUT main(VS_INPUT i)
{
	VS_OUTPUT o;
	o.Pos = mul(i.Pos, worldToScreen);
	o.TexCoord0 = i.TexCoord;
	return o;
}