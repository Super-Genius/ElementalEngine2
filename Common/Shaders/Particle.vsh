#include "Shaders\HLSLconstants.h"
#include "Shaders\VSH_CommonFunctions.h"

struct VSIn
{
	float3 Position		:	POSITION;
	float3 PosOffset	:	NORMAL;
	float2 TexCoord0	:	TEXCOORD0;
	float4 Life			:	TANGENT; // life, midlife, maxlife, rotation
};

struct VSOut
{
	float4 HPosition			: POSITION;
	float2 TexCoord0			: TEXCOORD0;
	float4 Color				: COLOR0;
};

uniform float2 ZMinMax : register(c19);

uniform float4x4 ViewMatrix : register(c20);
uniform float4 ParticleWeightSpeedSizeSpinKey0 : register(c24);
uniform float4 ParticleWeightSpeedSizeSpinKey1 : register(c25);
uniform float4 ParticleWeightSpeedSizeSpinKey2 : register(c26);
uniform float4 ParticleColorKey0 : register(c27);
uniform float4 ParticleColorKey1 : register(c28);
uniform float4 ParticleColorKey2 : register(c29);

VSOut main(	VSIn In	)
{
	VSOut Out;

	float lifestep = step( In.Life.x, In.Life.y );
	float weight0 = (In.Life.x / In.Life.y);
	float weight1 = ((In.Life.x - In.Life.y) / (In.Life.z - In.Life.y));
	
	float4 WeightSpeedSizeSpin = 
		lerp( ParticleWeightSpeedSizeSpinKey0, ParticleWeightSpeedSizeSpinKey1, weight0 ) * lifestep + 
		lerp( ParticleWeightSpeedSizeSpinKey1, ParticleWeightSpeedSizeSpinKey2, weight1 ) * (1.0 - lifestep);

	float4 Color = 
		lerp( ParticleColorKey0, ParticleColorKey1, weight0 ) * lifestep + 
		lerp( ParticleColorKey1, ParticleColorKey2, weight1 ) * (1.0 - lifestep);

	Out.Color = Color;
	Out.TexCoord0 = In.TexCoord0;

	float life = In.Life.x / In.Life.z;
	float rotation = In.Life.w + WeightSpeedSizeSpin.w * life;
	float s,c;
	sincos(rotation, s, c);
	float3x3 rotationMatrix = float3x3( c, -s, 0.0, s, c, 0.0, 0.0, 0.0, 1.0 );
	float3 posOffset = mul( rotationMatrix, In.PosOffset * WeightSpeedSizeSpin.z );
	float3x3 invViewMatrix = transpose((float3x3)ViewMatrix);

	float4 pos = float4( In.Position + mul( invViewMatrix, posOffset ), 1.0 );
	Out.HPosition = mul(worldToScreen, pos);

	return Out;
}
