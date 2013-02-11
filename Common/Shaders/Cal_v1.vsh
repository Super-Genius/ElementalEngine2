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
	float4 PosOutDepth		:	COLOR0;
	float4 ViewVectorFog	:	TEXCOORD5;
#if (DETAILLIGHTS > 0 )
	float3 Normal			:	NORMAL;
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

	OUT.PosOutDepth.xyz = pos;
	OUT.HPosition = mul( worldToScreen, float4(pos,1.0));
	OUT.TexCoord.xy = IN.TexCoord.xy;

	//Lights into Tangent Space
#if (ACTIVELIGHTS > 0)
	float3 lightdir[ACTIVELIGHTS];
	CalcLights( posWorld, TBN, lightdir, OUT.TangentLight );
#endif

	// calculate specular for first light
	//OUT.SpecularH.xyz = CalcSpecular( EyePos.xyz, posWorld.xyz, lightdir[0].xyz, TBN );
	OUT.ViewVectorFog.xyz = CalcViewVector( EyePos.xyz, posWorld.xyz, TBN );
	OUT.ViewVectorFog.w = CalcFog( OUT.HPosition.z, FogDistance.x, FogDistance.y );
	OUT.PosOutDepth.w = CalcZ( OUT.HPosition.z, ZMinMax.x, ZMinMax.y );
	
	return OUT;
}
