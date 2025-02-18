struct Input
{
    float4 position : SV_Position;
    float3 normal : NORMAL;
    float2 uv : TEXCOORD;
    float4 worldPos : WPOS;
    float3 cameraPosition : CPOS;
};

cbuffer LightBuffer : register(b1)
{
    float4 ambientColor; // Ambient light color                  16
    
    float4 pointLightPos; // Point light position (world space)   16
    float4 pointLightColor; // Point light color                    16
    float pointLightIntensity; //                                      4
    float pointLightRange; // --------> Not in use yet             4
    float2 padding; // padding                              8
    
    float4 specularColor; // Specular light color
    float specularIntensity; // Specular intensity (Ks)
    float shininess; // Shininess (higher = sharper highlight)
}

Texture2D texture0 : register(t0); // Bind the texture to slot t0
SamplerState samplerState : register(s0); // Bind the sampler to slot s0

float4 main(Input input) : SV_TARGET
{
    // Texture Color
    float4 texColor = texture0.Sample(samplerState, input.uv);
    
    // Ambient lightning
    float4 ambient = texColor * ambientColor;
    
    // Diffuse Lighting
    float3 lightDistance = pointLightPos.xyz - input.worldPos.xyz;
    float3 lightDirection = -1 * normalize(lightDistance);
    
    float attenuation = 1 / pow(length(lightDistance), 2);
    //float attenuation = (lightDistance < pointLightRange) ? (1 / pow(length(lightDistance), 2)) : 0;

    float diffuseFactor = max(dot(input.normal.xyz, lightDirection), 0) * attenuation;
    
    float4 diffuse = texColor * pointLightColor * diffuseFactor * pointLightIntensity;
    
    // Specular Lighting
    float3 viewDir = normalize(input.cameraPosition.xyz - input.worldPos.xyz);
    float3 reflectDir = reflect(lightDirection, input.normal.xyz);
    
    float cosDelta = max(dot(viewDir, reflectDir), 0);
    
    float specFactor = pow(cosDelta, shininess) * specularIntensity * attenuation;
    float4 specular = specularColor * specFactor;

    return (ambient + diffuse) + specular;
}