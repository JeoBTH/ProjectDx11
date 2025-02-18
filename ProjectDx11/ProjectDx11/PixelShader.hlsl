struct Input
{
    float4 position : SV_Position;
    float3 normal : NORMAL;
    float2 uv : TEXCOORD;
    float4 worldPos : WPOS;
    float3 cameraPosition : CPOS;
};

Texture2D texture0 : register(t0); // Bind the texture to slot t0
SamplerState samplerState : register(s0); // Bind the sampler to slot s0

float4 main(Input input) : SV_TARGET
{
    // Texture Color
    float4 texColor = texture0.Sample(samplerState, input.uv);
    

    return texColor;
}