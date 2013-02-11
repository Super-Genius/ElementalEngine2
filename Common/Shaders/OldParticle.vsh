#include "Shaders\HLSLconstants.h"
#include "Shaders\VSH_CommonFunctions.h"

struct VSIn
{
	float3 Position		:	POSITION;
	float2 TexCoord0	:	TEXCOORD0;
	float3 PosOffset	:	NORMAL;
    float4 Rotation		:	TANGENT;
	float4 Color		:	COLOR0;
};

struct VSOut
{
	float4 HPosition	: POSITION;
	float2 TexCoord0	: TEXCOORD0;
	float4 Color		:	COLOR0;
//	float4  Depth		:	COLOR1;
//	float4  Center		:	TEXCOORD1;
//	float4  Pos			:	TEXCOORD2;	
};

uniform float4x4 ViewMatrix	: register(c20);
uniform float2 ZMinMax : register(c19);

VSOut main(	VSIn In	)
{
	VSOut Out;

	float s,c;
	sincos(In.Rotation.x, s, c);
	float3x3 mat = float3x3( c, -s, 0.0, s, c, 0.0, 0.0, 0.0, 1.0 );
	float3 posOffset = mul( mat, In.PosOffset );
	float3x3 mat2 = transpose((float3x3)ViewMatrix);
	posOffset = mul( mat2, posOffset );

	float4 pos = float4(In.Position + posOffset, 1);
	float radius =	length( posOffset );
	Out.HPosition = mul(worldToScreen, pos);
	Out.TexCoord0 = In.TexCoord0;
	Out.Color = In.Color;

/*
	// Soft Particles
	Out.Depth.x = (Out.HPosition.x/Out.HPosition.w)*.5f + .5f;//(Out.HPosition.x/1280.f) + .5f;
	Out.Depth.y =  -(Out.HPosition.y/Out.HPosition.w)*.5f + .5f;
	Out.Depth.z = CalcZ( Out.HPosition.z, ZMinMax.x, ZMinMax.y );
	//distance of square length based on diagonal:
	radius *= 0.707f; //1/sqrt(2)
	Out.Depth.w = CalcZ( radius, ZMinMax.x, ZMinMax.y );

	Out.Center.xyz = In.Position.xyz;
	Out.Center.w = ZMinMax.x;
	Out.Pos.xyz = In.Position + posOffset;
	Out.Pos.w = ZMinMax.y;
*/

	return Out;
}
