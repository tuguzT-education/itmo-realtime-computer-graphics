#pragma pack_matrix(row_major)

#include "transform.hlsl"

cbuffer VSConstantBuffer : register(b0)
{
    Transform transform;
    float2 tile_count;
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
    float3 world_position : POSITION0;
};

VS_Output VSMain(VS_Input input)
{
    VS_Output output = (VS_Output)0;

    output.world_position = mul(float4(input.position, 1.0f), transform.world).xyz;

    return output;
}

cbuffer ShadowMapConstantBuffer : register(b0)
{
    float4x4 shadow_map_view_projections[SHADOW_MAP_CASCADE_COUNT];
    float4 shadow_map_debug_colors[SHADOW_MAP_CASCADE_COUNT];
    float4 shadow_map_distances[((SHADOW_MAP_CASCADE_COUNT - 1) / 4) + 1];
};

typedef VS_Output GS_Input;

struct GS_Output
{
    float4 position : SV_Position;
    uint index : SV_RenderTargetArrayIndex;
};

[instance(SHADOW_MAP_CASCADE_COUNT)]
[maxvertexcount(3)]
void GSMain(triangle GS_Input inputs[3], in uint id : SV_GSInstanceID, inout TriangleStream<GS_Output> stream)
{
    [unroll]
    for (int i = 0; i < 3; ++i)
    {
        GS_Output output = (GS_Output) 0;
        output.position = mul(float4(inputs[i].world_position.xyz, 1.0f), shadow_map_view_projections[id]);
        output.index = id;

        stream.Append(output);
    }
}
