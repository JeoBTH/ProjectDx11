struct Input
{
    float3 pos : POSITION;
};

struct Output
{
    float4 pos : SV_Position;
};

cbuffer ShadowViewProjection : register(b3)
{
    float4x4 lightViewProj;
};

Output main(Input input)
{
    Output output;
    output.pos = mul(float4(input.pos, 1.0f), lightViewProj);
    return output;
}