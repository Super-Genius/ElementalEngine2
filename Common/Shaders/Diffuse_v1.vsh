#include "Shaders\HLSLconstants.h"

float4 c_TextureScale : register (c20);

struct VS_INPUT
{
	float4  Pos					: POSITION;
	float2  TexCoord			: TEXCOORD0;
	float4  Color				: COLOR0;
};

struct VS_OUTPUT
{
	float4  Pos					: POSITION;
	float2  TexCoord0			: TEXCOORD0;
	float4  TexCoord1			: TEXCOORD1;
};
	
VS_OUTPUT main(VS_INPUT i)
{
	VS_OUTPUT o;
	o.Pos = mul(worldToScreen, i.Pos );
	o.TexCoord0 = i.TexCoord;
	o.TexCoord1 = i.Color;
	return o;
}