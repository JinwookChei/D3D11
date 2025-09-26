struct Input
{
    float2 Pos : POSITION;
    float2 UV : TEXCOORD;
};

struct PS_INPUT
{
    float4 Pos : SV_Position;
    float2 UV : TEXCOORD;
};

cbuffer ConstantBuffer : register(b0)
{
    float2 Offset;
    float2 Scale;
}

PS_INPUT main(Input input)
{
    PS_INPUT outputValue = (PS_INPUT) 0;
    float2 scaledPosition = input.Pos * Scale + Offset;
    outputValue.Pos = float4(scaledPosition, 0.0f, 1.0f);
    outputValue.UV = input.UV;
    
    return outputValue;
}