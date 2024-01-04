Texture2D txDiffuse : register(t0);
Texture2D txNormal : register(t1);
Texture2D txSpecular : register(t2);
Texture2D txOpacity : register(t3);
SamplerState samLinear : register(s0);

//--------------------------------------------------------------------------------------
// Constant Buffer Variables
//--------------------------------------------------------------------------------------
cbuffer ConstantBuffer : register(b0)
{
    matrix World;
    matrix View;
    matrix Projection;
}

cbuffer LightConstantBuffer : register(b1)
{
    float4 cbLightDir;
    float4 cbLightColor;
    float4 cbLightAmbient;
    float4 cbLightDiffuse;
    float4 cbLightSpecular;
}

cbuffer MaterialConstantBuffer : register(b2)
{
    float4 cbMaterialAmbient;
    float4 cbMaterialDiffuse;
    float4 cbMaterialSpecular;
    float cbMaterialSpecularPower;
    float3 cbCameraPosition;
    bool UseOpacityMap;
    bool UseSpecularMap;
    bool UseNormalMap;
    float3 padding;
}

cbuffer MatrixPalette : register(b3)
{
    matrix MatrixPaletteArray[128];
}

//--------------------------------------------------------------------------------------
struct VS_INPUT
{
    float4 Pos : POSITION;
    float3 Norm : NORMAL;
    float2 Tex : TEXCOORD0;
    float3 Tangent : TANGENT;
    int4 BlendIndices : BLENDINDICES;
    float4 BlendWeights : BLENDWEIGHT;
};

struct PS_INPUT
{
    float4 Pos : SV_POSITION;
    float3 Norm : NORMAL;
    float2 Tex : TEXCOORD0;
    float3 PixelPos : POSITION;
    float3 Tangent : TANGENT;
};