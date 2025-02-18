struct Input
{
    float3 pos : POSITION;
    float3 normal : NORMAL;
    float2 uv : TEXCOORD;
};

struct Output
{
    float4 position : SV_Position;
    float3 normal : NORMAL;
    float2 uv : TEXCOORD;
    float4 worldPos : WPOS;
    float3 cameraPosition : CPOS;
};

cbuffer TransformBuffer : register(b0)
{
    float4x4 worldMatrix;
};

cbuffer CameraBuffer : register(b2)
{
    float4x4 viewProjectionMatrix;
    float3 cameraPosition;
};

Output main(Input input)
{
    Output output;
    
    float4 worldposition = mul(float4(input.pos, 1.0f), worldMatrix);
    output.worldPos = worldposition;
    output.position = mul(worldposition, viewProjectionMatrix);
    output.normal = normalize(mul(input.normal, (float3x3) worldMatrix));
    output.uv = input.uv; // Pass UV to the pixel shader
    output.cameraPosition = cameraPosition.xyz;
    
    return output;
}