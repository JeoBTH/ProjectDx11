struct Input
{
    float3 pos : POSITION;
};

struct Output
{
    float4 pos : SV_Position;
};

cbuffer TransformBuffer : register(b0)
{
    float4x4 worldMatrix;
};

cbuffer ShadowViewProjection : register(b3)
{
    float4x4 lightViewProj;
};

Output main(Input input)
{
    Output output;
    float4 worldposition = mul(float4(input.pos, 1.0f), worldMatrix);
    output.pos = mul(worldposition, lightViewProj);
    return output;
}