struct Input
{
    float4 position : SV_Position;
    float3 normal : NORMAL;
    float2 uv : TEXCOORD;
    float4 worldPos : WPOS;
    float3 cameraPosition : CPOS;
    float4 lightSpacePos : TEXCOORD1;
};

cbuffer AmbientLightBuffer : register(b1)
{
    float4 ambientColor;                    // Ambient light color                  16
}

cbuffer PointLightBuffer : register(b2)
{
    float4 pointLightPos;                   // Point light position (world space)   16
    float4 pointLightColor;                 // Point light color                    16
    float pointLightIntensity;              //                                      4
    float pointLightRange;                  // --------> Not in use yet             4
    float2 padding1;
}

cbuffer DirectionalLightBuffer : register(b3)
{
    float3 directionalLightDirection;           // Point light position (world space)   12
    float4 directionalLightColor;               // Point light color                    16
    float directionalLightIntensity;            //                                      4
}

cbuffer MaterialBuffer : register(b4)
{
    float4 specularColor; // Specular light color
    float specularIntensity; // Specular intensity (Ks)
    float shininess; // Shininess (higher = sharper highlight)
}

Texture2D texture0 : register(t0); // Bind the texture to slot t0
SamplerState samplerState : register(s0); // Bind the sampler to slot s0

Texture2D shadowMap : register(t1);
SamplerComparisonState shadowSampler : register(s1);

float ComputeShadow(float4 lightSpacePos)
{
    // Transform to NDC space [-1,1] -> [0,1]
    float3 ndcSpace = lightSpacePos.xyz / lightSpacePos.w;
    float3 shadowMapUV = float3(ndcSpace.x * 0.5f + 0.5, ndcSpace.y * -0.5f + 0.5f, ndcSpace.z * 0.5f + 0.5f);

    shadowMapUV = saturate(shadowMapUV);

    // Compare with depth in shadow map
    float shadow = shadowMap.SampleCmpLevelZero(shadowSampler, shadowMapUV.xy, shadowMapUV.z - 0.01f); // Add small bias
    return shadow;
}

float4 main(Input input) : SV_TARGET
{
    // Texture Color
    float4 texColor = texture0.Sample(samplerState, input.uv);
    
    // Ambient lightning
    float4 ambient = texColor * ambientColor;
    
    // Diffuse Lighting
    float3 lightDistance = pointLightPos.xyz - input.worldPos.xyz;
    float3 lightDirection = normalize(lightDistance);
    
    float attenuation = 1 / pow(length(lightDistance), 2);
    //float attenuation = (lightDistance < pointLightRange) ? (1 / pow(length(lightDistance), 2)) : 0;

    float diffuseFactor = max(dot(input.normal.xyz, lightDirection), 0) * attenuation;
    
    float4 pointDiffuse = texColor * pointLightColor * diffuseFactor * pointLightIntensity;
    
     // Directional Light Diffuse
    float3 directionalLightDir = normalize(-directionalLightDirection.xyz); // Invert if light is "pointing at" surface
    float NdotL_dir = max(dot(input.normal, directionalLightDir),0.0f);
    float4 directionalDiffuse = texColor * directionalLightColor * NdotL_dir * directionalLightIntensity;

    // Combine Diffuse
    float4 diffuse = pointDiffuse;
    
    // Specular Lighting
    float3 viewDir = normalize(input.cameraPosition.xyz - input.worldPos.xyz);
    float3 reflectDir = reflect(-1 * lightDirection, input.normal.xyz);
    
    float cosDelta = max(dot(viewDir, reflectDir), 0);
    
    float specFactor = pow(cosDelta, shininess) * specularIntensity * attenuation;
    float4 specular = specularColor * specFactor;

    // Shadows
    float shadowFactor = ComputeShadow(input.lightSpacePos); // from vertex shader
    float4 shadowedDirectionalDiffuse = directionalDiffuse * shadowFactor;
 
    float depth = shadowMap.Sample(samplerState, input.uv).r;
    
    //return float4(input.lightSpacePos.xyz / input.lightSpacePos.w, 1);
    //return float4(shadowFactor.xxx, 1.0f);
    return (ambient + diffuse) + specular + shadowedDirectionalDiffuse;
}