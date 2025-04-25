struct Input
{
    float4 position : SV_Position;
    float3 normal : NORMAL;
    float2 uv : TEXCOORD;
    float4 worldPos : WPOS;
    float3 cameraPosition : CPOS;
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
    float4 directionalLightPos;             // Point light position (world space)   16
    float4 directionalLightColor;           // Point light color                    16
    float directionalLightIntensity;        //                                      4
    float3 padding2;
}

cbuffer MaterialBuffer : register(b4)
{
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
    float3 lightDirection = normalize(lightDistance);
    
    float attenuation = 1 / pow(length(lightDistance), 2);
    //float attenuation = (lightDistance < pointLightRange) ? (1 / pow(length(lightDistance), 2)) : 0;

    float diffuseFactor = max(dot(input.normal.xyz, lightDirection), 0) * attenuation;
    
    float4 pointDiffuse = texColor * pointLightColor * diffuseFactor * pointLightIntensity;
    
     // Directional Light Diffuse (treat pos as direction)
    float3 directionalDir = normalize(-directionalLightPos.xyz); // Negate if you want light *towards* surface
    float NdotL_dir = max(dot(input.normal, directionalDir), 0.0f);
    float4 directionalDiffuse = texColor * directionalLightColor * directionalLightIntensity * NdotL_dir;

    // Combine Diffuse
    float4 diffuse = pointDiffuse + directionalDiffuse;
    
    // Specular Lighting
    float3 viewDir = normalize(input.cameraPosition.xyz - input.worldPos.xyz);
    float3 reflectDir = reflect(-1 * lightDirection, input.normal.xyz);
    
    float cosDelta = max(dot(viewDir, reflectDir), 0);
    
    float specFactor = pow(cosDelta, shininess) * specularIntensity * attenuation;
    float4 specular = specularColor * specFactor;

    return (ambient + diffuse) + specular;
    //return ambient;
    //return float4(input.uv, 0.0f, 1.0f); // visualize UVs
    //return texture0.Sample(samplerState, input.uv);
}