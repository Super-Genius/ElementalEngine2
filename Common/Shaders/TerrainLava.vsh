#include "Shaders\HLSLconstants.h"
#include "Shaders\VSH_CommonFunctions.h"

struct VS_INPUT
{
	float4 Position		: POSITION;
	float3 Normal		: NORMAL;
	float3 Tangent		: TANGENT;
	float2 TexCoord0	: TEXCOORD0; // mask
	float2 TexCoord1	: TEXCOORD1; // tile
	float2 TexCoord2	: TEXCOORD2; // lightmap
};

struct VS_OUTPUT
{
	float4  Pos					: POSITION;
	float2  TexCoord0			: TEXCOORD0;
	float2  TexCoord1			: TEXCOORD1;
	float3  TexCoord2			: TEXCOORD2;
#if (ACTIVELIGHTS > 0)
	float4  TangentLight[ACTIVELIGHTS]		: TEXCOORD3;
	float3	ViewVector			: TEXCOORD7;
#endif
	float4 Fog					: COLOR0;
};

uniform float3 EyePos			: register(c17);
uniform float4 FogDistance		: register(c18);
uniform float2 ZMinMax			: register(c19);
uniform float4 SinCosTanTime	: register(c20);
// x=OscSpeed y=OscOffset z=OscScale w=AccumRate
uniform float4 Texture0_X		: register(c21); 
uniform float4 Texture0_Y		: register(c22); 
uniform float4 Texture1_X		: register(c23); 
uniform float4 Texture1_Y		: register(c24); 
uniform float4 TextureMask_X	: register(c25); 
uniform float4 TextureMask_Y	: register(c26); 
uniform float4 WaterInfo		: register(c27); // x=transparency depth, y=water level
uniform float4 WaterConstants	: register(c28); // x=shore tc bend amount
// x=OscSpeed y=OscOffset z=OscScale w=AccumRate

VS_OUTPUT main(VS_INPUT IN)
{
	VS_OUTPUT OUT;

	// x=OscScale y=OscOffset z=OscSpeed w=AccumRate
	//Debug Info
	/*
	Texture0_X = float4( 0.15, 0.0, 0.15, -0.005 );
	Texture0_Y = float4( 0.15, 0.0, 0.15, -0.005 );
	Texture1_X = float4( 0.15, 0.0, 0.15, -0.005 );
	Texture1_Y = float4( 0.15, 0.0, 0.15, -0.005 );
	TextureMask_X = float4( 0.05, 0.0, 0.05, 0.0 );
	TextureMask_Y = float4( 0.05, 0.0, 0.05, 0.0 );
	*/

	// Calculate Screen Position
	OUT.Pos = mul(worldToScreen, float4(IN.Position.x, 0.0, IN.Position.z, 1.0) );

#if (ACTIVELIGHTS > 0)
	// Calculate the TBN
	float3x3 TBN = CalcTBNMatrix( IN.Tangent, IN.Normal );

	// Calculate World Position
	float4 tempPos;
	tempPos.xyz = IN.Position;
	tempPos.w = 1.0;
	tempPos = mul(modelToWorld, tempPos );

	// Calculate Lights into Tangent Space
	float3 lightdir[ACTIVELIGHTS];
	CalcLights( tempPos, TBN, lightdir, OUT.TangentLight );

	OUT.ViewVector = CalcViewVector( EyePos.xyz, tempPos.xyz, TBN );
#endif

	// (water level - terrain level) / transparency depth
	float fWaterTransparency = saturate( (WaterInfo.y - IN.Position.y) / WaterInfo.x );

	float2 coord = IN.TexCoord0 * 2;
//	float2 tcNormal = IN.Normal.xz * 0.1 * (1.0-fWaterTransparency);
	float2 tcNormal = IN.Normal.xz * (1.0-fWaterTransparency) * (WaterConstants.x);

	OUT.TexCoord0.x = coord.x + sin(SinCosTanTime.w * Texture0_X.x + Texture0_X.y) * Texture0_X.z;
	OUT.TexCoord0.x += SinCosTanTime.w * Texture0_X.w;
	OUT.TexCoord0.y = coord.y + sin(SinCosTanTime.w * Texture1_Y.x + Texture1_Y.y) * Texture1_Y.z;
	OUT.TexCoord0.y += SinCosTanTime.w * Texture0_Y.w;
	
	OUT.TexCoord1.x = coord.x + sin(SinCosTanTime.w * Texture1_X.x + Texture1_X.y) * Texture1_X.z;
	OUT.TexCoord1.x += SinCosTanTime.w * Texture1_X.w;
	OUT.TexCoord1.y = coord.y + sin(SinCosTanTime.w * Texture1_Y.x + Texture1_Y.y) * Texture1_Y.z;
	OUT.TexCoord1.y += SinCosTanTime.w * Texture1_Y.w;
	
	OUT.TexCoord2.x = coord.x + sin(SinCosTanTime.w * TextureMask_X.x + TextureMask_X.y) * TextureMask_X.z;
	OUT.TexCoord2.x += SinCosTanTime.w * TextureMask_X.w;
	OUT.TexCoord2.y = coord.y + sin(SinCosTanTime.w * TextureMask_Y.x + TextureMask_Y.y) * TextureMask_Y.z;
	OUT.TexCoord2.y += SinCosTanTime.w * TextureMask_Y.w;

	OUT.TexCoord2.xy -= tcNormal;
	OUT.TexCoord2.z = saturate(fWaterTransparency + 0.0); // darkening near edges of shore

	OUT.Fog.w = CalcFog( OUT.Pos.z, FogDistance.x, FogDistance.y );
	OUT.Fog.yz = 0;
	OUT.Fog.x = CalcZ( OUT.Pos.z, ZMinMax.x, ZMinMax.y );

	return OUT;
}