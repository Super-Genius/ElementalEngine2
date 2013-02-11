#include "Shaders\HLSLconstants.h"
#include "Shaders\VSH_CommonFunctions.h"

struct appdata  {
	float3 Position: POSITION;
	float4 Weight:BLENDWEIGHT;
	float4 Indices:BLENDINDICES;
};

struct vpconn  {
	float4 HPosition: POSITION;
	float4 Color: COLOR0;
};
	

vpconn main(appdata IN)
{
	vpconn OUT;	

	float3 modelPos = SkinPosition( float4(IN.Position.xyz, 1.0), IN.Indices, IN.Weight );
	CalcShadowCasterCoords( float4(modelPos,1.0), OUT.HPosition, OUT.Color );

	return OUT;
}