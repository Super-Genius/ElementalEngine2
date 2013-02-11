#include "Shaders\HLSLconstants.h"

struct VSIn
{
	float3 Position		: POSITION;
	float2 TexCoord		: TEXCOORD0;
};

struct VSOut
{
	float4 HPosition	: POSITION;
	float2 TexCoord		: TEXCOORD0;
};

VSOut main(	VSIn In )
{
	VSOut Out;

	float4 pos = float4(In.Position, 1.0);
	Out.TexCoord = In.TexCoord;
	Out.HPosition = mul(worldToScreen, pos);

	return Out;
}
