#include "Shaders\HLSLconstants.h"
#include "Shaders\VSH_CommonFunctions.h"

struct appdata  {
	float3 Position: POSITION;
	float4 Weight:BLENDWEIGHT;
	float4 Indices:BLENDINDICES;
	float3 Normal: NORMAL;
    float2 TexCoord:TEXCOORD0;     
    float4 Tangent: TANGENT;
};

struct vpconn  {
	float4 HPosition: POSITION;
	float2 TexCoord: TEXCOORD0;
#if (ACTIVELIGHTS > 0)
	float4 TangentLight[ACTIVELIGHTS]	: TEXCOORD1;
#endif
	float4 PosOutDepth		: COLOR0;
	float4 ViewVectorFog	: TEXCOORD5;
#if (SHADOWMAPS > 0)
	float4 ShadowCoord[SHADOWMAPS]	: TEXCOORD6;
#endif
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
	float3 pos = SkinPosition( float4(IN.Position.xyz, 1.0), IN.Indices, IN.Weight );

	//Normals
	float3 normal = SkinNormal( IN.Normal, IN.Indices, IN.Weight );
		
	//Tangents
	float4 tangent = SkinTangent( IN.Tangent, IN.Indices, IN.Weight );
		
#if (DETAILLIGHTS > 0 )
	OUT.Normal = normal;
#endif
	
	//TBN Matrix
	float3x3 TBN = CalcTBNMatrix( tangent, normal  );

	float3 posWorld = mul(modelToWorld, float4(pos,1.0)).xyz;
	OUT.PosOutDepth.xyz = pos;//pixel lights are calculated in model space for efficiency(light pos are pre-transformed by CPU to modelspace per model)

	OUT.HPosition = mul( worldToScreen, float4(pos,1.0));
	OUT.TexCoord.xy = IN.TexCoord.xy;

#if (ACTIVELIGHTS > 0)
	//Lights into Tangent Space
	float3 lightdir[ACTIVELIGHTS];
	CalcLights( posWorld, TBN, lightdir, OUT.TangentLight );
#endif

	// calculate specular for first lighton
	OUT.ViewVectorFog.xyz = CalcViewVector( EyePos.xyz, posWorld.xyz, TBN );
	OUT.ViewVectorFog.w = CalcFog( OUT.HPosition.z, FogDistance.x, FogDistance.y );
	OUT.PosOutDepth.w = CalcZ( OUT.HPosition.z, ZMinMax.x, ZMinMax.y );
	
	// Shadow Coord
#if (SHADOWMAPS > 0)
	for( int shadowmap = 0; shadowmap < SHADOWMAPS; shadowmap++ )
	{
		OUT.ShadowCoord[shadowmap] = float4(posWorld, 0.0) + CalcShadowReceiverCoords(shadowmap);
	}
#endif
	return OUT;
}
