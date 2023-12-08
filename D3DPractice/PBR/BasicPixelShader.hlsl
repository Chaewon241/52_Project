#include "Shared.fxh"

//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 main(PS_INPUT input) : SV_Target
{
    float4 finalColor = 0;

    float3 Normal = normalize(input.Norm);
    
    // �븻���� �ִ���
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

    // ����ŧ������ �ִ���
    if (UseSpecularMap)
    {
        SpecularColor *= txSpecular.Sample(samLinear, input.Tex).r;
    }
    
    float Opacity = 1.0f;
    
    // ���Ľ�Ƽ���� �ִ���
    if (UseOpacityMap)
    {
        Opacity = txOpacity.Sample(samLinear, input.Tex).a;
    }
    
    // ���� final color�� �ؽ�ó ������ ���Ͽ� �����մϴ�.
    finalColor = AmbientColor + DiffuseColor + SpecularColor;

    return float4(finalColor.rgb, Opacity);
}