#include "Shaders\HLSLconstants.h"
#include "Shaders\VSH_CommonFunctions.h"

struct appdata  {
	float3 Position: POSITION;
	float3 Normal: NORMAL;
    float2 TexCoord:TEXCOORD0;     
    float2 TexCoord1:TEXCOORD1;   
    float4 Tangent: TANGENT;
};

struct vpconn  {
	float4 HPosition:	POSITION;
	float2 TexCoord:	TEXCOORD0;
#if (ACTIVELIGHTS > 0)
	float4 TangentLight[ACTIVELIGHTS]	: TEXCOORD1;
#endif
	float4 PosOutDepth		: COLOR0;
	float4 ViewVectorFog	: TEXCOORD5;
	float4 ShadowCoord[MAXSHADOWMAPS]	: TEXCOORD6;
#if (DETAILLIGHTS > 0 )
	float3 Normal			: NORMAL;
#endif
};

uniform float3 EyePos	: register(c17);
uniform float4 FogDistance	: register(c18);
uniform float2 ZMinMax : register(c19);

vpconn main(appdata IN)
{
	vpconn OUT;	

	//Position
	float3 pos = IN.Position.xyz;

	//TBN Matrix
	float3x3 TBN = CalcTBNMatrix( IN.Tangent, IN.Normal );
	
	OUT.PosOutDepth.xyz = pos;//pixel lights are calculated in model space for efficiency(light pos are pre-transformed by CPU to modelspace per model)
	float3 posWorld = mul(modelToWorld, float4(pos,1.0)).xyz;

	OUT.HPosition = mul( worldToScreen, float4(pos,1.0));
	OUT.TexCoord.xy = IN.TexCoord.xy;

#if (ACTIVELIGHTS > 0)
	//Lights into Tangent Space
	float3 lightdir[ACTIVELIGHTS];
	CalcLights( posWorld, TBN, lightdir, OUT.TangentLight );

#endif

	// Shadow Coord
	OUT.ShadowCoord[0] = float4(posWorld, 0.0) + CalcShadowReceiverCoords(0);
	OUT.ShadowCoord[1] = float4(posWorld, 0.0) + CalcShadowReceiverCoords(1);

	OUT.ViewVectorFog.xyz = CalcViewVector( EyePos.xyz, posWorld.xyz, TBN );
	OUT.ViewVectorFog.w = CalcFog( OUT.HPosition.z, FogDistance.x, FogDistance.y );
	OUT.PosOutDepth.w = CalcZ( OUT.HPosition.z, ZMinMax.x, ZMinMax.y );

#if (DETAILLIGHTS > 0 )
	OUT.Normal = IN.Normal;
#endif

	return OUT;
}
