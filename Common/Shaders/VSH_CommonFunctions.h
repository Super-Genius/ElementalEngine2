#include "Shaders\HLSLconstants.h"

float CalcFog( in float PosZ, in float FogStart, in float FogScale )
{
	float fogratio = (PosZ - FogStart) / FogScale;
	fogratio = clamp( fogratio, 0, 1 );
	return fogratio;
}

float CalcZ( in float PosZ, in float ZMin, in float ZMax )
{
	float zratio = (PosZ - ZMin) / ZMax;
	zratio = clamp( zratio, 0, 1 );
	return zratio;
}

float3 SkinPosition( in float4 Position, in float4 Indices, in float4 Weight )
{
	float3 SkinnedPosition = 0;
	SkinnedPosition  = mul(boneMatrix[Indices.x], Position.xyzw) * Weight.x;
	SkinnedPosition += mul(boneMatrix[Indices.y], Position.xyzw) * Weight.y;
	SkinnedPosition += mul(boneMatrix[Indices.z], Position.xyzw) * Weight.z;
	SkinnedPosition += mul(boneMatrix[Indices.w], Position.xyzw) * Weight.w;
	return SkinnedPosition;
}

float3 SkinNormal( in float3 Normal, in float4 Indices, in float4 Weight )
{
	float3 SkinnedNormal = 0;
	SkinnedNormal  = mul(boneMatrix[Indices.x], Normal.xyz) * Weight.x;
	SkinnedNormal += mul(boneMatrix[Indices.y], Normal.xyz) * Weight.y;
	SkinnedNormal += mul(boneMatrix[Indices.z], Normal.xyz) * Weight.z;
	SkinnedNormal += mul(boneMatrix[Indices.w], Normal.xyz) * Weight.w;
	return SkinnedNormal;
}

float4 SkinTangent( in float4 Tangent, in float4 Indices, in float4 Weight )
{
	float4 SkinnedTangent = 0;
	SkinnedTangent.xyz  = mul(boneMatrix[Indices.x], Tangent.xyz) * Weight.x;
	SkinnedTangent.xyz += mul(boneMatrix[Indices.y], Tangent.xyz) * Weight.y;
	SkinnedTangent.xyz += mul(boneMatrix[Indices.z], Tangent.xyz) * Weight.z;
	SkinnedTangent.xyz += mul(boneMatrix[Indices.w], Tangent.xyz) * Weight.w;
	SkinnedTangent.w = Tangent.w;
	return SkinnedTangent;
}

float3 CalcBinormal( in float3 Tangent, in float3 Normal, in float Direction )
{
	return normalize(( cross(Tangent.xyz, Normal) )) * Direction;
}

float3x3 CalcTBNMatrix( in float3 Tangent, in float3 Normal )
{
	float3x3 modelToWorldTrans = (float3x3)modelToWorld;
	
	float3 N = normalize( mul( modelToWorldTrans, Normal ) );
	float3 T = normalize( mul( modelToWorldTrans, Tangent ) );
	float3 B = CalcBinormal( T, N, 1.0 );
	float3x3 TBN = float3x3( T, B, N );
	return TBN;
}

// Skinned TBN has to factor Binormal Flip (w component of the Tangent vector)
float3x3 CalcTBNMatrix( in float4 Tangent, in float3 Normal )
{
	float3x3 modelToWorldTrans = (float3x3)modelToWorld;
	
	float3 N = normalize( mul( modelToWorldTrans, Normal ) );
	float3 T = normalize( mul( modelToWorldTrans, Tangent.xyz ) );
	float3 B = CalcBinormal( T, N, -Tangent.w );
	float3x3 TBN = float3x3( T, B, N );
	return TBN;
}

float3 CalcSpecular( in float3 EyePos, in float3 WorldPos, in float3 LightDir, in float3x3 TBN )
{
	float3 V = normalize( EyePos.xyz - WorldPos.xyz );
	float3 L = LightDir;
	float3 H = normalize(L + V);
	return mul(TBN, H);
}

// Used for Phong Specular Reflection
float3 CalcHVector( in float3 EyePos, in float3 WorldPos, in float3 LightDir, in float3x3 TBN )
{
	float3 V = normalize( EyePos.xyz - WorldPos.xyz );
	float3 L = LightDir;
	float3 H = normalize(L + V);
	return mul(TBN, H);
}

// Used for Blinn Specular Reflection
float3 CalcViewVector( in float3 EyePos, in float3 WorldPos, in float3x3 TBN )
{
	float3 ViewVecWorld = normalize( EyePos - WorldPos );
	return mul(TBN, ViewVecWorld);
}

#if (ACTIVELIGHTS > 0)
void CalcLights( in float3 WorldPos, in float3x3 TBN, out float3 LightDir[ACTIVELIGHTS], out float4 TangentLight[ACTIVELIGHTS] )
{
	for( int light = 0; light < ACTIVELIGHTS; light++ )
	{
		// w is (-1) if light is directional or (>0) if light is omni
		LightDir[light] = VertexLights[ light ].xyz;
		TangentLight[light].w = 1;
		if( VertexLights[ light ].w > 0 )
		{
			LightDir[light] -= WorldPos.xyz;
			float distance = length( LightDir[ light ] ) + 1.f;//make sure it is not zero
			TangentLight[light].w = max( 0, min( 1.f - distance/VertexLights[light].w, 1 ) );
		}
		LightDir[light] = normalize( LightDir[light] );
		TangentLight[light].xyz = mul( TBN, LightDir[light] );
	}
}
#endif

void CalcShadowCasterCoords( in float4 ModelPos, out float4 HPosition, out float4 ShadowMapColor )
{
	//it is in lightspace now
	float4 lightspaceP = mul( modelToWorld, ModelPos ) - float4(shadowcoord[0].xyz, 0.0);
	float lenP = length( lightspaceP );
	float alphaout = (lenP/zclip.y); //alpha test for culling away other hemisphere
	lightspaceP /= lenP;
	lightspaceP.y += zclip.z; // hemisphere half

	HPosition.x = lightspaceP.x / lightspaceP.y;
	HPosition.y = lightspaceP.z / lightspaceP.y;
	HPosition.z =(lenP  - zclip.x)/(zclip.y-zclip.x); //(len - znear)/ (zfar- znear);
	HPosition.w = 1.0;

	ShadowMapColor = float4( HPosition.zzz, alphaout );
}

float4 CalcShadowReceiverCoords( in int coord )
{
	return float4( -shadowcoord[coord].xyz, shadowcoord[coord].w );
}
