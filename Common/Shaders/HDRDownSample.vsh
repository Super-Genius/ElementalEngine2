#include "Shaders\HLSLconstants.h"

struct VS_INPUT
{
	float4  Pos					: POSITION;
	float2  TexCoord			: TEXCOORD0;
};

struct VS_OUTPUT
{
	float4  Pos					: POSITION;
	float2  TexCoord0			: TEXCOORD0;
};

VS_OUTPUT main(VS_INPUT i)
{
	VS_OUTPUT o;
	o.Pos = mul(worldToScreen, i.Pos );
	//o.Pos = clamp( o.Pos, -.3, .3 );
	o.TexCoord0 = i.TexCoord;
	return o;
}