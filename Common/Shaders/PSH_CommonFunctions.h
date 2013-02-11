#include "Shaders\HLSLconstants.h"

struct PS_MRT_OUTPUT
{
	float4 Color0: COLOR0;
	float4 Color1: COLOR1;
};

PS_MRT_OUTPUT	MakeMRTOutput( in float4 color )
{
	PS_MRT_OUTPUT Out;
	Out.Color0 = color;
	Out.Color1 = float4(  0.f, 0.f, 0.f, 0.f );
	return Out;
}

PS_MRT_OUTPUT	MakeMRTOutput( in float4 color, in float depth )
{
	PS_MRT_OUTPUT Out;
	Out.Color0 = color;
	Out.Color1 = float4( depth, depth, depth, 1.f );
	return Out;
}

float3 UnpackNormal( in float4 NormalMapValue )
{
	NormalMapValue.xyz = NormalMapValue.xyz * 2 - 1;
	NormalMapValue.z = sqrt( 1 - NormalMapValue.x*NormalMapValue.x - NormalMapValue.y*NormalMapValue.y );
	return NormalMapValue.xyz;
}

#if (ACTIVELIGHTS > 0)
void NormalizeLights( inout float4 TangentLights[ACTIVELIGHTS] )
{
	float3 TempLight;

	for( int light = 0; light < ACTIVELIGHTS; light++ )
	{
		TempLight = normalize( TangentLights[light].xyz );
		TangentLights[light].xyz = TempLight;
	}
}
#endif

float CalcSpecular( in float3 Normal, in float3 HalfAngle, in float Pow, in float SpecAmount, in float Attenuation )
{
	return Attenuation * SpecAmount * pow(saturate(dot(Normal, normalize(HalfAngle))), Pow);
}

float3 CalcColoredSpecular( in float3 Normal, in float3 HalfAngle, in float Pow, in float SpecAmount, in float Attenuation )
{
	return ( lightData[0].rgb * CalcSpecular( Normal, HalfAngle, Pow, SpecAmount, Attenuation ) );
}

float CalcSpecularBlinn( in float3 Normal, in float3 HalfAngle, in float Pow, in float SpecAmount )
{
	return SpecAmount * pow(saturate(dot(Normal, normalize(HalfAngle))), Pow);
}

float3 CalcPixelLights( in float3 Position, in float3 Normal, in float4 lightsData[MAX_LIGHTS*2] )
{
	float3 normal = normalize( Normal );
	float3 intensity = float3(0,0,0);
	int numLights = ( ACTIVELIGHTS + DETAILLIGHTS ) * 2;
	for( int light = ACTIVELIGHTS*2; light < numLights ; light += 2 )
	{
		//Light Dir should come in modelspace( already multiplied by the inverse world matrix )
		float4 curLight = lightsData[ light ].xyzw;
		float3 LightDir = ( curLight.xyz - Position.xyz);
		float distance = length( LightDir ) + 1.f;//make sure it is not 0
		float atten = saturate( 1.f - (distance/curLight.w) );
		LightDir /= distance;
		float NdotL = atten*max( dot(Normal,LightDir ), 0 );
		intensity +=  NdotL*lightsData[light + 1].rgb;
	}
	return intensity;
}


float CalcSpecularPhong( in float3 Normal, in float3 ViewVec, in float3 LightVec, in float Pow, in float SpecAmount )
{
	return SpecAmount * pow(saturate(dot(reflect(-ViewVec, Normal), LightVec)), Pow);
}

#if (ACTIVELIGHTS > 0)
float3 CalcMultiLightColoredSpecular( in float3 Normal, in float3 ViewVec, in float Pow, in float SpecAmount, in float4 TangentLights[ACTIVELIGHTS] )
{
	float3 SpecColor = float3(0.0,0.0,0.0);

	float3 VrefN = reflect(-ViewVec, Normal);

	for( int light = 0; light < ACTIVELIGHTS; light++ )
	{
		//if( lightData[light*2+1].w > 0.0 )
		//{
			//SpecColor += ( lightData[light*2+1].rgb * CalcSpecularPhong( Normal, ViewVec, TangentLights[light].xyz, Pow, TangentLights[light].w) ) * lightData[light*2+1].w;
			//SpecColor += ( lightData[light*2+1].rgb * CalcSpecularPhong( Normal, ViewVec, TangentLights[light].xyz, Pow, SpecAmount) );
			SpecColor += ( lightData[light*2+1].rgb * pow(saturate(dot(VrefN, TangentLights[light].xyz) ), Pow ) ) * TangentLights[light].w;
		//}
	}
	SpecColor.rgb *= SpecAmount;
	return SpecColor.rgb;
}
#endif

#if (ACTIVELIGHTS > 0)
float3 CalcLights( in float3 Normal, in float4 TangentLights[ACTIVELIGHTS] )
{
	float3 intensity = float3(0,0,0);
	for( int light = 0; light < ACTIVELIGHTS; light++ )
	{
		float NdotL = max( 0.0, (dot(Normal, TangentLights[light].xyz)) );
		intensity += TangentLights[light].w * NdotL * lightData[light*2].rgb;
	}
	return intensity;
}
#endif

#if (ACTIVELIGHTS > 0)
float3 CalcDebugLights( in float3 Normal, in float4 TangentLights[ACTIVELIGHTS] )
{
	float3 intensity = float3(0,0,0);
	for( int light = 0; light < ACTIVELIGHTS; light++ )
	{
		if (TangentLights[light].w > 0.0)
			intensity.rg += 0.25;
	}
	return intensity;
}
#endif

#if (ACTIVELIGHTS > 0)
float CalcShadowTerm( in float4 ShadowCoord, sampler2D ShadowMap )
{
	{
		//Percentage closer filtering
		// Generate the 9 texture co-ordinates for a 3x3 PCF kernel
		float2 vTexCoords[9];

		half fShadowTerm = 1.0;	
		// Generate the tecture coordinates for the specified depth-map size
		// 4 3 5
		// 1 0 2
		// 7 6 8
		//Shadow coord has to be correct per pixel or else we get distorted and
		//offset shadows   
		float3 lightspace = ShadowCoord.xyz;
		float lenP =  length( lightspace.xyz );
		float depth = lenP / ShadowCoord.w; //alpha test for culling away other hemisphere

		depth -=  0.001;

		float2 scCoord;
		float4 sclodCoord;
		float2 shadow[2];
		float4 lodShadow[2];
		float2 sample;
		if( depth < 1.0 )
		{
			float sampleCoord = step( 0, lightspace.y );
			float hemisphere = (sampleCoord - 0.5) * 2.0;
			lightspace.xyz /= lenP;

			scCoord.x = lightspace.x / (lightspace.y + hemisphere);
			scCoord.y = -lightspace.z / (lightspace.y + hemisphere);
			scCoord.xy = float2(0.5,0.5) + (0.5 * scCoord.xy);

			// The second method of texcoord generation creates higher res but lumpier shadows and requires
			// a larger sampling resolution (because they are not diagonally adjacent texel samples)
			// It's all a tradeoff of shadow lumpiness and texelation.
			// The lerp causes texel lookup to change resolution over distance. This gives us softer shadows
			// up close (wider sampling) and 'sharper' in the distance (tighter sampling).
			// Since the resolution lowers by distance, the tigher sampling reduces the texelated effect.
//			float fTexelSize = lerp( 1.0 / 1024.0, 1.0 / 2048.0, depth );
//			vTexCoords[0] = scCoord.xy + float2( -fTexelSize, -fTexelSize );
//			vTexCoords[1] = scCoord.xy + float2(  fTexelSize, -fTexelSize );
//			vTexCoords[2] = scCoord.xy + float2( -fTexelSize,  fTexelSize );
//			vTexCoords[3] = scCoord.xy + float2(  fTexelSize,  fTexelSize );
			float fTexelSize = lerp( 1.0 / 512.0, 1.0 / 1024.0, depth );
			vTexCoords[0] = scCoord.xy + float2( fTexelSize, 0.0 );
			vTexCoords[1] = scCoord.xy + float2( -fTexelSize,  0.0 );
			vTexCoords[2] = scCoord.xy + float2(  0.0, fTexelSize );
			vTexCoords[3] = scCoord.xy + float2(  0.0,  -fTexelSize );

			float2 A[4];
			float4 coord = float4( scCoord.x, scCoord.y, 0, 3 );
			float2 shadowSample = tex2D( ShadowMap, scCoord.xy ).rg;	
			float2 shadowCompare = tex2Dlod( ShadowMap, coord ).rg;
			shadow[0].x = shadowSample.r;
			shadow[0].y = shadowCompare.r;
			shadow[1].x = shadowSample.g;
			shadow[1].y = shadowCompare.g;
			sample = lerp( shadow[0], shadow[1], sampleCoord );
			if( abs( sample.x  - sample.y) > .003 )
			{
				A[0] = tex2D( ShadowMap, vTexCoords[ 0 ] ).rg;
				A[1] = tex2D( ShadowMap, vTexCoords[ 1 ] ).rg;
				A[2] = tex2D( ShadowMap, vTexCoords[ 2 ] ).rg;
				A[3] = tex2D( ShadowMap, vTexCoords[ 3 ] ).rg;
				lodShadow[0].x = A[0].x;
				lodShadow[0].y = A[1].x;
				lodShadow[0].z = A[2].x;
				lodShadow[0].w = A[3].x;
				lodShadow[1].x = A[0].y;
				lodShadow[1].y = A[1].y;
				lodShadow[1].z = A[2].y;
				lodShadow[1].w = A[3].y;
				float4 totalLod = lerp( lodShadow[0], lodShadow[1], sampleCoord );
				totalLod = float4( depth, depth, depth, depth ) - totalLod;
				//if depth is greater 
				totalLod = step( 0, totalLod );
				float multiplier = totalLod.x + totalLod.y + totalLod.z + totalLod.w;
				fShadowTerm = 1.0 - multiplier*.25;
			}
			else
			{
				fShadowTerm = step( 0, sample.x - depth );
			}
		}
		//This allows us to fade the shadow as it extends away from the light
		half newfShadowTerm = fShadowTerm + (( 1 - fShadowTerm )*depth );
		return saturate( newfShadowTerm );
//		return saturate( fShadowTerm );
	}
}
#endif
