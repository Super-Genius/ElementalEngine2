#include "Shaders\HLSLconstants.h"

struct PASSTHRU_VERTEX
{
    float4 Position   : POSITION;
    float2 TexCoords  : TEXCOORD0;
};

PASSTHRU_VERTEX main( float2 vPosition  : POSITION, 
                                     float2 vTexCoords : TEXCOORD0 )
{
    PASSTHRU_VERTEX Output;
    Output.Position  = mul(worldToScreen, float4( vPosition.x, vPosition.y, 0.0f, 1.0f ));
    Output.TexCoords = vTexCoords;
    return Output;
}