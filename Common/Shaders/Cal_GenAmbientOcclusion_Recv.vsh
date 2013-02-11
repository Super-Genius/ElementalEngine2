#include "Shaders\HLSLconstants.h"

struct appdata  {
	float3 Position: POSITION;
	float3 Normal: NORMAL;
    float2 TexCoord:TEXCOORD0;     
    float4 Tangent: TANGENT;
    float2 TexCoordRTT:TEXCOORD1;
};

struct vpconn  {
	float4 HPosition: POSITION;
	float2 TexCoord:		TEXCOORD0;
	float3 Normal:			TEXCOORD1;
	float4 ShadowCoord:		TEXCOORD2;
	float3 Tangent:			TEXCOORD3;
	float3 Binormal:		TEXCOORD4;
	float3 PosOut:		COLOR0;
	float4 Fog:			COLOR1;
	float3 EyeVector	: TEXCOORD7;
};
	
uniform float3 EyePos	: register(c17);
uniform float4 FogColor	: register(c18);

vpconn main(appdata IN)
{
	float fogStart = 3500;
	vpconn OUT;	
	
	float3 normal;
	normal = IN.Normal;
	
	float3 tangent;
	tangent = IN.Tangent.xyz;
	
	normal = normalize(normal);
	tangent = normalize(tangent);
	
	float4 tempPos2;
	
	tempPos2.xyz = IN.Position.xyz;
	tempPos2.w = 1.0;
	//it is in lightspace now
	OUT.PosOut = mul( modelToWorld, tempPos2);
	//float3 lightspaceP = mul( worldToLight, float4( OUT.PosOut.x,OUT.PosOut.y,OUT.PosOut.z, 1) );
	float3 lightspaceP = mul( worldToLight, float4( 0,0,0,1) );
	
	//normalize by w, should already be normalized
	OUT.ShadowCoord.xyz = lightspaceP.xyz;
	OUT.ShadowCoord.w = zclip.y;
	
	OUT.HPosition = mul( worldToScreen, tempPos2 );	
	OUT.TexCoord.xy = IN.TexCoord.xy;
	//translate normal, tangent and binormal to worldspace from model space		
	float3x3 modelToWorldTrans = transpose((float3x3)modelToWorld);
	OUT.Normal   = normalize(mul(modelToWorldTrans,IN.Normal));
	OUT.Normal.x = -OUT.Normal.x;
	OUT.Tangent  = normalize(mul(modelToWorldTrans,IN.Tangent.xyz));
	OUT.Binormal = normalize( cross(OUT.Tangent.xyz, OUT.Normal.xyz) );

	OUT.EyeVector = EyePos - OUT.PosOut;

	float fogratio = (OUT.HPosition.z  - fogStart)/ FogColor.w;
	fogratio = clamp( fogratio, 0, 1 );
	OUT.Fog.rgb = FogColor.rgb;
	OUT.Fog.w = fogratio;

	OUT.HPosition.xy = IN.TexCoordRTT.xy * 2.0 - 1.0;

	return OUT;
}