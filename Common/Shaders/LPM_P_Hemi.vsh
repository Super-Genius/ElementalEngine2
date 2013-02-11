#include "Shaders\HLSLconstants.h"
#include "Shaders\VSH_CommonFunctions.h"

struct appdata  {
	float3 Position: POSITION;
};

struct vpconn  {
	float4 HPosition: POSITION;
	float4 Color: COLOR0;
};

vpconn main(appdata IN)
{
	vpconn OUT;	
	
	CalcShadowCasterCoords( float4(IN.Position.xyz,1.0), OUT.HPosition, OUT.Color );

	return OUT;
}