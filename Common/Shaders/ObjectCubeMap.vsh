#include "Shaders\HLSLconstants.h"

struct appdata  {
	float3  Pos             : POSITION;
	float3  TexCoord        : TEXCOORD0;
};

struct VS_OUTPUT  {
	float4  Pos			: POSITION;
	float3  TexCoord0	: TEXCOORD0;
};
	

VS_OUTPUT main(appdata IN)
{
	VS_OUTPUT o;
	o.Pos.xyz = IN.Pos;
	o.Pos.w = 1.0;
	o.Pos.xyzw = mul( worldToScreen, o.Pos.xyzw );
	o.TexCoord0 = IN.TexCoord;
	return o;
}