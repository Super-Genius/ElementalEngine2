#include "Shaders\HLSLconstants.h"
#include "Shaders\VSH_CommonFunctions.h"

struct VS_INPUT
{
	float4  Pos					: POSITION;
	float2  TexCoord0			: TEXCOORD0;
};

struct VS_OUTPUT
{
	float4 Pos				: POSITION;
	float4 TexCoords[4]		: TEXCOORD0;
	float4 TexCoordCenter	: COLOR1;
	float4 Weight			: COLOR0;
};

float GaussianDistribution( float x, float y, float rho )
{
	const float two_rho_sqrd = 2.0 * rho * rho;
	float g = rsqrt( PI * two_rho_sqrd );
	g *= exp( -(x*x + y*y) / (two_rho_sqrd) );
	return g;
}

void CalcSampleOffsets_Gaussian5x5( in float2 textureSize, out float2 vTexCoordOffsets[15], out float4 vColorWeights[15], in float fDeviation, in float fMultiplier )
{
    float weight = fMultiplier * GaussianDistribution( 0.0, 0.0, fDeviation );
    vColorWeights[0] = float4( weight, weight, weight, 1.0 );
    vTexCoordOffsets[0] = float2(0.0, 0.0);

    for( int i=1; i < 8; i++ )
    {
	    // Fill the first half
        weight = fMultiplier * GaussianDistribution( i, 0.0, fDeviation ); // get the Gaussian intensity for this offset
        vTexCoordOffsets[i] = float2(i, i);
        vColorWeights[i] = float4( weight, weight, weight, 1.0 );

	    // Mirror to the second half
        vTexCoordOffsets[i+7] = -vTexCoordOffsets[i];
        vColorWeights[i+7] = vColorWeights[i];
    }
}

void CalcSampleOffsets_Gaussian3x3( out float2 vTexCoordOffsets[8], out float4 vColorWeights[8], in float fBlurFactor, in float fMultiplier )
{
	const float fPixelCenter = 0.5;
	const float4 offsets = float4( -2.5, -0.5, 1.5, 3.5 );
	
	const float fX = fPixelCenter * fBlurFactor;
	const float fY = fPixelCenter * fBlurFactor;
	vTexCoordOffsets[0] = float2(fX, offsets.x * fBlurFactor);
	vTexCoordOffsets[1] = float2(fX, offsets.y * fBlurFactor);
	vTexCoordOffsets[2] = float2(fX, offsets.z * fBlurFactor);
	vTexCoordOffsets[3] = float2(fX, offsets.w * fBlurFactor);
	vTexCoordOffsets[4] = float2(offsets.x * fBlurFactor, fY);
	vTexCoordOffsets[5] = float2(offsets.y * fBlurFactor, fY);
	vTexCoordOffsets[6] = float2(offsets.z * fBlurFactor, fY);
	vTexCoordOffsets[7] = float2(offsets.w * fBlurFactor, fY);

	for (int i=0; i < 8; i++)
	{
		vColorWeights[i] = float4(1.0,1.0,1.0,1.0) * fMultiplier;
	}
}

void CalcSampleOffsets_Median3x3( in float2 textureSize, out float2 vTexCoordOffsets[8], out float4 vColorWeights[8], in float fMultiplier )
{
/*    float weight = fMultiplier * GaussianDistribution( 0.0, 0.0, fDeviation );
    vColorWeights[0] = float4( weight, weight, weight, 1.0 );
    vTexCoordOffsets[0] = float2(0.0, 0.0);

    for( int i=1; i < 8; i++ )
    {
	    // Fill the first half
        weight = fMultiplier * GaussianDistribution( i, 0.0, fDeviation ); // get the Gaussian intensity for this offset
        vTexCoordOffsets[i] = float2(i, i);
        vColorWeights[i] = float4( weight, weight, weight, 1.0 );

	    // Mirror to the second half
        vTexCoordOffsets[i+7] = -vTexCoordOffsets[i];
        vColorWeights[i+7] = vColorWeights[i];
    }
*/
}

void CalcSampleOffsets_Mean3x3( in float2 textureSize, out float2 vTexCoordOffsets[8], out float4 vColorWeights[8], in float fMultiplier )
{
	const float fMeanWeight = 1.0 / 8.0;
    float weight = fMultiplier * fMeanWeight;
    vTexCoordOffsets[0] = float2(0.0, 0.0);
    vTexCoordOffsets[1] = float2(-1.0, -1.0);
    vTexCoordOffsets[2] = float2(0.0, -1.0);
    vTexCoordOffsets[3] = float2(1.0, -1.0);
    vTexCoordOffsets[4] = float2(1.0, 0.0);
    vTexCoordOffsets[5] = float2(1.0, 1.0);
    vTexCoordOffsets[6] = float2(0.0, 1.0);
    vTexCoordOffsets[7] = float2(-1.0, 1.0);

    for( int i=0; i < 8; i++ )
    {
        vColorWeights[i] = float4( weight, weight, weight, 1.0 );
    }
}

VS_OUTPUT main(VS_INPUT i)
{
	VS_OUTPUT o;
	o.Pos = mul(worldToScreen, i.Pos);
	o.TexCoordCenter = float4(i.TexCoord0.xy, 0.0, 0.0);

	// Gaussian 5x5
/*
	float2 vTexCoordOffsets[15];
	float4 vColorWeights[15];
	CalcSampleOffsets_Gaussian5x5( 0.0, vTexCoordOffsets, vColorWeights, 3.0, 2.0 );
	o.TexCoords[0] = float4( vTexCoordOffsets[0].x, 0.0, vColorWeights[0].x, 0.0 );
	for (int tc=1; tc < 8; tc++)
	{
		o.TexCoords[tc] = float4( vTexCoordOffsets[tc].x, vTexCoordOffsets[tc+7].x, vColorWeights[tc].x, vColorWeights[tc+7].x );
	}
*/
	// Mean 3x3
/*	float2 vTexCoordOffsets[8];
	float4 vColorWeights[8];
	CalcSampleOffsets_Mean3x3( 0.0, vTexCoordOffsets, vColorWeights, 2.0 );
	for (int tc=0; tc < 4; tc++)
	{
		o.TexCoords[tc] = float4( 
			vTexCoordOffsets[tc].x, vTexCoordOffsets[tc].y, 
			vTexCoordOffsets[tc+4].x, vTexCoordOffsets[tc+4].y
		);
	}
*/
	// Gaussian 3x3
	float2 vTexCoordOffsets[8];
	float4 vColorWeights[8];
	CalcSampleOffsets_Gaussian3x3( vTexCoordOffsets, vColorWeights, 2.0, 0.4 );
	for (int tc=0; tc < 4; tc++)
	{
		o.TexCoords[tc] = float4( 
			vTexCoordOffsets[tc].x, vTexCoordOffsets[tc].y, 
			vTexCoordOffsets[tc+4].x, vTexCoordOffsets[tc+4].y
		);
	}
	o.Weight = vColorWeights[0];
	return o;
}