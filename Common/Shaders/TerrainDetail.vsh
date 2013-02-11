#include "Shaders\HLSLconstants.h"
#include "Shaders\VSH_CommonFunctions.h"

struct VSIn
{
	float4 Position		: POSITION;
	float3 Normal		: NORMAL;
	float3 Tangent		: TANGENT;
	float2 TexCoord0	: TEXCOORD0; // mask
	float2 TexCoord1	: TEXCOORD1; // tile
	float2 TexCoord2	: TEXCOORD2; // lightmap
};

struct VSOut
{
	float4 HPosition		: POSITION;
	float4 MaskTileTexCoord	: TEXCOORD0; // mask(xy) tile(zw)
#if (ACTIVELIGHTS > 0)
	float4 TangentLight[ACTIVELIGHTS]	: TEXCOORD1;
#if (SHADOWMAPS > 0)
	float4 ShadowCoord[SHADOWMAPS]	: TEXCOORD5;
#endif
#endif
	float4 ViewVectorFog	: COLOR0;
	float4 PosOutDepth		: COLOR1;
#if (DETAILLIGHTS > 0 )
	float3 Normal			:	NORMAL;
#endif
};

uniform float4 FogDistance		: register(c18);
uniform float2 ZMinMax		: register(c19);
uniform float4 EyePos		: register(c22);
uniform float4 Params		: register(c23); // x=loddist

VSOut main(	VSIn IN )
{
	VSOut OUT;

	//Position
	float3 pos = IN.Position.xyz;

	//TBN Matrix
	float3x3 TBN = CalcTBNMatrix( IN.Tangent, IN.Normal );

	float3 posWorld = mul(modelToWorld, float4(pos,1.0)).xyz;

	OUT.HPosition = mul( worldToScreen, float4(pos,1.0));

	OUT.MaskTileTexCoord.xy = IN.TexCoord0.xy;
	OUT.MaskTileTexCoord.zw = IN.TexCoord1.xy;

#if (ACTIVELIGHTS > 0)
	//Lights into Tangent Space
	float3 lightdir[ACTIVELIGHTS];
	CalcLights( posWorld, TBN, lightdir, OUT.TangentLight );

#if (SHADOWMAPS > 0)
	// Shadow Coord
	for( int shadowmap = 0; shadowmap < SHADOWMAPS; shadowmap++ )
	{
		OUT.ShadowCoord[shadowmap] = float4(posWorld, 0.0) + CalcShadowReceiverCoords(shadowmap);
	}
#endif //SHADOWMAPS
#endif //ACTIVELIGHTS

	float3 V = EyePos.xyz - posWorld.xyz;
	float lenV = length( V );
	float SpecularFade = pow(saturate(lenV / Params.x), 3);

	OUT.ViewVectorFog.xyz = CalcViewVector( EyePos.xyz, posWorld.xyz, TBN );
	OUT.ViewVectorFog.w = CalcFog( OUT.HPosition.z, FogDistance.x, FogDistance.y );

	OUT.PosOutDepth.xyz = pos;
	OUT.PosOutDepth.w = CalcZ( OUT.HPosition.z, ZMinMax.x, ZMinMax.y );

#if (DETAILLIGHTS > 0 )
	OUT.Normal = IN.Normal;
#endif
	return OUT;
}
