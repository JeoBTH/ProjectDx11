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
sampler shadowSampler : register(s1);

float ComputeShadow(float4 lightSpacePos)
{    
    // Transform to NDC space [-1,1] -> [0,1]
    float3 ndcSpace = lightSpacePos.xyz / lightSpacePos.w;

    // Convert NDC [-1,1] to texture coords [0,1]
    ndcSpace.xy = ndcSpace.xy * 0.5f + 0.5f;
    // Y-flip: remove this line unless your shadow map is upside-down
    ndcSpace.y = 1.0f - ndcSpace.y;
    
    float3 texCoords = ndcSpace;

    // Check to see if outside shadow map bounds
    if (texCoords.x < 0.0f || texCoords.x > 1.0f ||
        texCoords.y < 0.0f || texCoords.y > 1.0f)
    {
        return 1.0f;
    }

    float currentDepth = texCoords.z;

    // Sample closest depth from shadow map
    float closestDepth = shadowMap.Sample(samplerState, texCoords.xy).r;

    // Add depth bias to avoid shadow acne
    float bias = 0.001f;

    // Return shadow factor
    return (currentDepth - bias) <= closestDepth ? 1.0f : 0.0f;
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
    
    return ambient + (diffuse + specular + shadowedDirectionalDiffuse) * shadowFactor;
}