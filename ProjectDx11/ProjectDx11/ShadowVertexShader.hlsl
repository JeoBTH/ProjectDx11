struct VSInput
{
    float3 pos : POSITION;
};

struct VSOutput
{
    float4 pos : SV_Position;
};

cbuffer ShadowViewProjection : register(b3)
{
    float4x4 lightViewProj;
};

VSOutput main(VSInput input)
{
    VSOutput output;
    output.pos = mul(float4(input.pos, 1.0f), lightViewProj);
    return output;
}