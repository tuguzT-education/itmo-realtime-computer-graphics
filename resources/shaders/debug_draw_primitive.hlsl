#include "transform.hlsl"
#include "vertex.hlsl"

cbuffer VSConstantBuffer : register(b0)
{
    Transform transform;
}

struct VS_Input
{
    float3 position : POSITION;
    float4 color : COLOR;
};

struct VS_Output
{
    float4 position : SV_Position;
    float4 color : COLOR;
};

VS_Output VSMain(VS_Input input)
{
    VS_Output output;

    output.position = mul(WorldViewProjection(transform), float4(input.position, 1.0f));
    output.color = input.color;

    return output;
}

typedef VS_Output PS_Input;

float4 PSMain(PS_Input input) : SV_Target
{
    float4 color = input.color;
    return color;
}
