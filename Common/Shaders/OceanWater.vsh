#include "Shaders\HLSLconstants.h"
#include "Shaders\VSH_CommonFunctions.h"

struct VSIn
{
	float3 Position		:	POSITION;
//	float3 Normal		:	NORMAL;
//	float2 TexCoord		:	TEXCOORD0;
//	float3 Tangent		:	TANGENT;
};

struct VSOut
{
	float4 HPosition	: POSITION;
	float4 INP			: TEXCOORD0;
	float4 INP1			: TEXCOORD1;
	float4 EyeVector	: TEXCOORD2;
	float2 TexGens		: TEXCOORD3;
	float4 INP2			: TEXCOORD4;
//	float3 EyeVector	: TEXCOORD5;
//	float4 Col0		: COLOR0;
//	float4 Col1		: COLOR1;
};

uniform float4 FogDistance	: register(c18);
uniform float4 EyePos	: register(c22);
uniform float4 PosKs	: register(c23); // rPosScale, rDetailHeightCutoff, rDetailHeightCutoffMin
uniform float4 ClipKs	: register(c24); // 
uniform float4 DMParameters	: register(c25); // texscale, ?, ?, 1/texscale
uniform float4 MipKs	: register(c26); // min mip dist, max mip dist, mip count

float tex2D_bilinear4x(uniform sampler2D tex, half4 t, float2 Scales)
{
	float size  = Scales.x;
	float scale = Scales.y;
	float4 tAB0 = tex2Dlod(tex, t);
	float2 f = frac(t.xy*size);
	float2 tAB  = lerp(tAB0.xz, tAB0.yw, f.x);
	return lerp(tAB.x, tAB.y, f.y);
}

VSOut main(	VSIn In, 
	uniform sampler2D HMap0 : register(s0),
	uniform sampler2D HMap1 : register(s1)
	)
{
	VSOut Out;

	const float rDetailHeightCutoff = PosKs.y;
	const float rDetailHeightCutoffMin = PosKs.z;

	float4 pos;
	pos.xz = In.Position.xy * pow(In.Position.z, 4) * PosKs.x;
	pos.y = 0.0;
	pos.w = 1.0;

	float4 ClipPos = mul(worldToScreen, pos);
	Out.HPosition = ClipPos;
	Out.INP = float4(0.0,0.0,1.0,0.0);
	Out.EyeVector.xyz = float3(1.0,0.0,0.0);
	Out.INP1 = float4(0.0,0.0,0.0,0.0);
	Out.INP2 = float4(0.0,0.0,0.0,0.0);
	Out.TexGens.xy = float2(0.0,0.0);
	float3 b0 = abs(ClipPos.xyz) < (ClipPos.www*ClipKs.x+ClipKs.y);
	if (all(b0))
	{
		float4 t;
		t.xy = pos.xz * DMParameters.x;

		float MipDist2 = dot(pos.xz, pos.xz);
		t.z = (MipDist2-MipKs.x)*MipKs.w;
		t.z = clamp(t.z, 0, MipKs.z); // mipmap of the hmap texture

		const half2 CS = half2(cos(3.4*10/180),sin(3.4*10/180));
		Out.TexGens.xy = half2(t.x*CS.x+t.y*CS.y, -t.x*CS.y+t.y*CS.x);

		Out.INP.xy = t.xy;
		Out.INP.z  = DMParameters.w/(DMParameters.z*DMParameters.y);
		Out.INP.w = (1.0-0.25)*1.5;

		const float fMagicA = 15*3.14/180; // 15deg in rad
		const float2 CS2 = float2(sin(fMagicA), cos(fMagicA));
		float2 tn = t.xy * float2(3.33*2.1, 3.37*5.7);
		Out.INP1.xy = t.xy*5.1;
		Out.INP1.zw = float2(tn.x*CS2.y+tn.y*CS2.x, -tn.x*CS2.x+tn.y*CS2.y);

		Out.INP2 = float4(Out.INP1.xy*13.1, -8, -8);

		float vDisp = tex2Dlod( HMap0, float4(t.xyzz) ).a;
//		float TexSize = DMParameters.y*pow(2,-floor(t.z));
//		float2 Scales = float2(TexSize, 1.0/TexSize);
//		float vDisp = tex2D_bilinear4x( HMap0, t.xyzz, Scales );

		if (In.Position.z < rDetailHeightCutoff)
		{
			float R = saturate((In.Position.z-rDetailHeightCutoffMin) * (1.0/(rDetailHeightCutoff-rDetailHeightCutoffMin)));
			float S = (1.0/5.1)*(1-R);
			vDisp += tex2Dlod( HMap1, float4(t.xy,0.0,0.0) ).a * S;
		}
		pos.y = (vDisp - 0.5) * DMParameters.z;
		pos.w = 1.0;
	}
	Out.HPosition = mul(worldToScreen, pos);

	float4 worldPos = mul(modelToWorld, pos);
	Out.EyeVector.xyz = worldPos.xyz - EyePos.xyz;
		
	Out.EyeVector.w = CalcFog( Out.HPosition.z, FogDistance.x, FogDistance.y );

	return Out;
}
