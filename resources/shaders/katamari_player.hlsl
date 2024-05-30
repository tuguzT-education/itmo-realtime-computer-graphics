#include "transform.hlsl"

cbuffer VSConstantBuffer : register(b0)
{
    Transform transform;
    float4x4 directional_light_view;
    float4x4 directional_light_projection;
    float2 tile_count;
    float time;
}

struct VS_Input
{
    float3 position : POSITION;
    float3 normal : NORMAL;
    float4 color : COLOR;
    float2 texture_coordinate : TEXCOORD0;
};

struct VS_Output
{
    float4 position : SV_Position;
    float3 normal : NORMAL;
    float4 color : COLOR;
    float2 texture_coordinate : TEXCOORD0;
    float3 world_position : TEXCOORD1;
    float4 directional_light_position : TEXCOORD2;
};

VS_Output VSMain(VS_Input input)
{
    VS_Output output = (VS_Output)0;

    // input.position.xy += 0.25f * cos(2.0f * time);
    // input.position.z *= (sin(time) + 1.0f) * 0.5f;

    // input.texture_coordinate.x += time * 0.1f;
    // input.texture_coordinate.y += sin(input.texture_coordinate.x * 5.0f + time) * 0.4f;

    output.position = mul(WorldViewProjection(transform), float4(input.position, 1.0f));
    output.normal = normalize(mul(transform.world, float4(input.normal, 0.0f)).xyz);
    output.color = input.color;
    output.texture_coordinate = input.texture_coordinate * tile_count;
    output.world_position = mul(transform.world, float4(input.position, 1.0f)).xyz;
    output.directional_light_position = mul(mul(directional_light_projection, directional_light_view),
                                            float4(output.world_position.xyz, 1.0f));

    return output;
}
