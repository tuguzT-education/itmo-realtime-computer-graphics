#include "transform.hlsl"

cbuffer VSConstantBuffer : register(b0)
{
    Transform transform;
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
};

VS_Output VSMain(VS_Input input)
{
	VS_Output output;

    input.position.xy += 0.25f * cos(2.0f * time);
	input.position.z *= sin(time);

	output.position = mul(WorldViewProjection(transform), float4(input.position, 1.0f));
	output.normal = mul(transform.world, float4(input.normal, 1.0f)).xyz;
	output.color = input.color;
	output.texture_coordinate = input.texture_coordinate * tile_count;
	output.world_position = mul(transform.world, float4(input.position, 1.0f)).xyz;

	return output;
}

Texture2D DiffuseMap : register(t0);
SamplerState Sampler : register(s0);

cbuffer PSConstantBuffer : register(b0)
{
    bool has_texture;
    float3 view_position;
}

typedef VS_Output PS_Input;

float4 PSMain(PS_Input input) : SV_Target
{
	float4 color = has_texture ? DiffuseMap.Sample(Sampler, input.texture_coordinate) : float4(1.0f, 1.0f, 1.0f, 1.0f);
	color *= input.color;
	return color;
}
