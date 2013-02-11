#include "Shaders\HLSLconstants.h"
#include "Shaders\VSH_CommonFunctions.h"

struct appdata  
{
	float3 Position			: POSITION;
	float3 Normal			: NORMAL;
    float4 Tangent			: TANGENT; 
    float2 TexCoord			: TEXCOORD0;   
    float2 TexCoord1		: TEXCOORD1;   
};

struct vpconn  
{
	float4 HPosition		: POSITION;
	float2 TexCoord			: TEXCOORD0;
#if (ACTIVELIGHTS > 0)
	float4 TangentLight[ACTIVELIGHTS]	: TEXCOORD1;
	float3 SpecularH		: TEXCOORD5;
#endif
	float3 ViewVector		: TEXCOORD6;
	float4 Fog				: COLOR1;
};
	
uniform float3 EyePos	: register(c17);
uniform float4 FogDistance	: register(c18);
uniform float2 ZMinMax		: register(c19);

vpconn main(appdata IN)
{
	vpconn OUT;	
	
	float4 pos;
	pos.xyz= IN.Position.xyz;
	pos.w = 1.0;

	float4 tempPos;
	tempPos = mul(modelToWorld,pos);
	OUT.TexCoord.xy = IN.TexCoord.xy;
	OUT.HPosition = mul(worldToScreen, pos);

	//TBN Matrix
	float3x3 TBN = CalcTBNMatrix( IN.Tangent, IN.Normal );

	// View Vec into Tangent Space
	OUT.ViewVector = CalcViewVector( EyePos.xyz, tempPos.xyz, TBN );

#if (ACTIVELIGHTS > 0)
	//Lights into Tangent Space
	float3 lightdir[ACTIVELIGHTS];
	CalcLights( tempPos, TBN, lightdir, OUT.TangentLight );

	// calculate specular for first light
	OUT.SpecularH.xyz = CalcHVector( EyePos.xyz, tempPos.xyz, lightdir[0].xyz, TBN );
#endif

	// Calculate Vertex Fog
	OUT.Fog.w = CalcFog( OUT.HPosition.z, FogDistance.x, FogDistance.y );
	OUT.Fog.yz = 0;
	OUT.Fog.x = CalcZ( OUT.HPosition.z, ZMinMax.x, ZMinMax.y );
	
	return OUT;
}