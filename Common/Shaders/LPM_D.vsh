#include "Shaders\HLSLconstants.h"

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
	float3 Normal:		TEXCOORD1;
	float4 PosOut:		COLOR0;
	float3 Tangent:		TEXCOORD3;
	float3 Binormal:	TEXCOORD4;
};
	

vpconn main(appdata IN)
{
	vpconn OUT;	
	
	float3 pos= 0;
	float3 normal= 0;
	float3 tangent = 0;
	
	float4 i;
	
	i = IN.Indices;
	
	/*pos.xyz  = mul(boneMatrix[i.x], IN.Position) * IN.Weight.x;
	pos.xyz += mul(boneMatrix[i.y], IN.Position) * IN.Weight.y;
	pos.xyz += mul(boneMatrix[i.z], IN.Position) * IN.Weight.z;
	pos.w   = 1.0;//+= mul(boneMatrix[i.w], IN.Position) * IN.Weight.w;
	*/
	float4 tempPos;
	tempPos.xyz = IN.Position.xyz;
	tempPos.w = 1.0;
	
	//pos.x = dot(boneMatrix[i.x][0].xyzw, tempPos.xyzw) * IN.Weight.x;
	//pos.y = dot(boneMatrix[i.x][1].xyzw, tempPos.xyzw) * IN.Weight.x;
	//pos.z = dot(boneMatrix[i.x][2].xyzw, tempPos.xyzw) * IN.Weight.x;
	//pos.x += dot(boneMatrix[i.y][0].xyzw, tempPos.xyzw) * IN.Weight.y;
	//pos.y += dot(boneMatrix[i.y][1].xyzw, tempPos.xyzw) * IN.Weight.y;
	//pos.z += dot(boneMatrix[i.y][2].xyzw, tempPos.xyzw) * IN.Weight.y;
	//pos.x += dot(boneMatrix[i.z][0].xyzw, tempPos.xyzw) * IN.Weight.z;
	//pos.y += dot(boneMatrix[i.z][1].xyzw, tempPos.xyzw) * IN.Weight.z;
	//pos.z += dot(boneMatrix[i.z][2].xyzw, tempPos.xyzw) * IN.Weight.z;
	pos  = mul(boneMatrix[i.x], tempPos.xyzw) * IN.Weight.x;
	pos += mul(boneMatrix[i.y], tempPos.xyzw) * IN.Weight.y;
	pos += mul(boneMatrix[i.z], tempPos.xyzw) * IN.Weight.z;
	//pos += mul(boneMatrix[i.w], tempPos) * IN.Weight.w;
	
	normal = mul(boneMatrix[i.x], IN.Normal)* IN.Weight.x;
	normal += mul(boneMatrix[i.y], IN.Normal)* IN.Weight.y;
	normal += mul(boneMatrix[i.z], IN.Normal)* IN.Weight.z;
	
	tangent = mul(boneMatrix[i.x], IN.Tangent.xyz)* IN.Weight.x;
	tangent += mul(boneMatrix[i.y], IN.Tangent.xyz)* IN.Weight.y;
	tangent += mul(boneMatrix[i.z], IN.Tangent.xyz)* IN.Weight.z;
	
	normal = normalize(normal);
	tangent = normalize(-tangent);
	
	float4 tempPos2;
	
	tempPos2.xyz = pos.xyz;
	tempPos2.w = 1.0;
	
	OUT.HPosition = mul( worldToScreen, tempPos2);
	
	OUT.PosOut = mul(modelToWorld,tempPos2);
	OUT.TexCoord.xy = IN.TexCoord.xy;
	//translate normal, tangent and binormal to worldspace from model space	
	OUT.Normal = mul(modelToWorld,normal);
	OUT.Tangent =  mul(modelToWorld,tangent);
	OUT.Binormal = cross(  OUT.Normal.xyz, OUT.Tangent.xyz );
	
	return OUT;
}