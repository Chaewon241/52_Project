#include "Shared.fxh"

//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 main(PS_INPUT input) : SV_Target
{
    float4 finalColor = 0;

    float3 Normal = normalize(input.Norm);
    
    // 노말맵이 있는지
    if (UseNormalMap)
    {
        float3 Tangent = normalize(input.Tangent);
        float3 BiTangent = cross(Normal, Tangent);
        float3 NormalTangentSpace = txNormal.Sample(samLinear, input.Tex).rgb * 2.0f - 1.0f;
        float3x3 WorldTransform = float3x3(Tangent, BiTangent, Normal);
        Normal = mul(NormalTangentSpace, WorldTransform);
        Normal = normalize(Normal);
    }
    
    // ambient
    float4 AmbientColor = cbLightAmbient * cbMaterialAmbient * cbLightColor;
    
    // diffuse
    float3 NormalizedLightDir = normalize(cbLightDir.xyz);
    float4 PreDiffuse = saturate(max(0.0f, dot(-NormalizedLightDir, Normal)));
    float4 DiffuseColor = cbLightDiffuse * cbMaterialDiffuse * PreDiffuse;
    
    // blinn phong
    float3 ViewVecotr = normalize(cbCameraPosition - input.PixelPos.xyz);
    float3 HalfVector = normalize(-NormalizedLightDir + ViewVecotr);
    float fHDotN = max(0.0f, dot(HalfVector, Normal));
    float4 SpecularColor = pow(fHDotN, cbMaterialSpecularPower) * cbMaterialSpecular * cbLightSpecular;

    // 스펙큘러맵이 있는지
    if (UseSpecularMap)
    {
        SpecularColor *= txSpecular.Sample(samLinear, input.Tex).r;
    }
    
    float Opacity = 1.0f;
    
    // 오파시티맵이 있는지
    if (UseOpacityMap)
    {
        Opacity = txOpacity.Sample(samLinear, input.Tex).a;
    }
    
    // 기존 final color와 텍스처 색상을 곱하여 결합합니다.
    finalColor = AmbientColor + DiffuseColor + SpecularColor;

    return float4(finalColor.rgb, Opacity);
}