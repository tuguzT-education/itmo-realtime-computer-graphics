#pragma pack_matrix(row_major)

#include "transform.hlsl"

cbuffer VSConstantBuffer : register(b0)
{
    Transform transform;
    float2 tile_count;
    float time;
}

struct VS_Input
{
    float3 position : POSITION0;
    float3 normal : NORMAL0;
    float4 color : COLOR0;
    float2 texture_coordinate : TEXCOORD0;
};

struct VS_Output
{
    float4 position : SV_Position;
    float3 normal : NORMAL0;
    float4 color : COLOR0;
    float2 texture_coordinate : TEXCOORD0;
    float3 world_position : TEXCOORD1;
    float3 world_view_position : TEXCOORD2;
};

VS_Output VSMain(VS_Input input)
{
    VS_Output output = (VS_Output)0;

    // input.position.xy += 0.25f * cos(2.0f * time);
    // input.position.z *= (sin(time) + 1.0f) * 0.5f;

    // input.texture_coordinate.x += time * 0.1f;
    // input.texture_coordinate.y += sin(input.texture_coordinate.x * 5.0f + time) * 0.4f;

    output.position = mul(float4(input.position, 1.0f), WorldViewProjection(transform));
    output.normal = normalize(mul(float4(input.normal, 0.0f), transform.world).xyz);
    output.color = input.color;
    output.texture_coordinate = input.texture_coordinate * tile_count;
    output.world_position = mul(float4(input.position, 1.0f), transform.world).xyz;
    output.world_view_position = mul(float4(input.position, 1.0f), mul(transform.world, transform.view)).xyz;

    return output;
}
