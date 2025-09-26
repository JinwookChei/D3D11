Texture2D mergeTexture : register(t0);
SamplerState samplers : register(s0);

struct PS_INPUT
{
    float4 Pos : SV_Position;
    float2 UV : TEXCOORD;
};

float4 main(PS_INPUT input) : SV_TARGET
{
    float4 color = mergeTexture.Sample(samplers, input.UV);

    color.a = saturate(color.a);

    return color;
}