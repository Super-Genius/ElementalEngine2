#include "Shaders\HLSLconstants.h"

struct VSIn
{
	float3 Position		:	POSITION;
	float2 TexCoord0	:	TEXCOORD0;
	float4 Color		:	COLOR0;
};

struct VSOut
{
	float4 HPosition	: POSITION;
	float2 TexCoord0	: TEXCOORD0;
	float4 Color		:	COLOR0;
};

VSOut main(	VSIn In	)
{
	VSOut Out;

	Out.HPosition = mul(worldToScreen, float4(In.Position,1));
	Out.TexCoord0 = In.TexCoord0;
	Out.Color = In.Color;

	return Out;
}
