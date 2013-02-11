#include "Shaders\HLSLconstants.h"
		
struct VS_INPUT
{
	float4  Pos					: POSITION;
	float3  Normal				: NORMAL;
	float2  TexCoord0			: TEXCOORD0;
};

struct VS_OUTPUT
{
	float4  Pos					: POSITION;
	float2  TexCoord0			: TEXCOORD0;
	float2  TexCoord1			: TEXCOORD1;
	float2  FogSin				: COLOR;	
};

uniform float3 EyePos		: register(c18);
uniform float4 UVDrift		: register(c19);
uniform float4 Timer		: register(c20);
uniform float4 CloudFade	: register(c21);

VS_OUTPUT main(VS_INPUT i)
{
	VS_OUTPUT o;
	o.Pos = mul(worldToScreen, i.Pos );
	o.TexCoord0 = i.TexCoord0.xy * 4.0; // cloud mask
	o.TexCoord1 = i.TexCoord0.xy * 128.0; // noise mask
	
	o.TexCoord0.x = o.TexCoord0.x + (Timer.w * UVDrift.x);
	o.TexCoord0.y = o.TexCoord0.y + (Timer.w * UVDrift.y);
	
	o.TexCoord1.x = o.TexCoord1.x + (Timer.w * UVDrift.y);
	o.TexCoord1.y = o.TexCoord1.y + (Timer.w * UVDrift.x);

	float3 posWorld = mul(modelToWorld,i.Pos).xyz;
	float3 eyeVector =  EyePos - posWorld;
	
	float dist = clamp(length( eyeVector ), 0, CloudFade.y);
	
	if (dist < CloudFade.x)
	{
		float fogratio = dist / CloudFade.x;
		o.FogSin.r = lerp( CloudFade.z, 1, fogratio );		
	}
	else
	{
		float fogratio = (dist - CloudFade.x) / (CloudFade.y - CloudFade.x);
		o.FogSin.r = 1.0 - fogratio;		
	}
	o.FogSin.g = Timer.r;
	return o;
}