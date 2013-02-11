#include "Shaders\HLSLconstants.h"

struct appdata  {
	float3 Position: POSITION;
	float3 Normal: NORMAL;
    float2 TexCoord:TEXCOORD0;   
    float2 Time: TEXCOORD1; 
};

struct vpconn  {
	float4 HPosition:	POSITION;
	float2 TexCoord:	TEXCOORD0;
	float4  Color:COLOR0;
};
	
uniform float4 Time	: register(c19);

vpconn main(appdata IN)
{
	vpconn OUT;	
	
	float4 pos;
	pos.xyz= IN.Position.xyz;
	pos.w = 1.0;
	
	OUT.TexCoord.xy = IN.TexCoord.xy;
	OUT.HPosition = mul(worldToScreen, pos);
	//calculate alpha fade:
	float timeDifference = Time.w - IN.Time.x;
	float timeRatio = 1.f - timeDifference / IN.Time.y;
	timeRatio = clamp( timeRatio, 0.f , 1.f );
	OUT.Color = float4( 1.f, 1.f, 1.f, timeRatio );	
	return OUT;
}