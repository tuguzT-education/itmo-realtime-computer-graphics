#include "transform.hlsl"
#include "vertex.hlsl"

cbuffer VSConstantBuffer : register(b0)
{
    Transform transform;
}

VertexPositionColor VSMain(VertexPositionColor input)
{
    VertexPositionColor output = (VertexPositionColor)0;

    output.position = mul(WorldViewProjection(transform), float4(input.position.xyz, 1.0f));
    output.color = input.color;

    return output;
}

float4 PSMain(VertexPositionColor input) : SV_Target
{
    float4 color = input.color;
    return color;
}
