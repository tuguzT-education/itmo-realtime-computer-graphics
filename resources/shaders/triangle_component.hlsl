#include "transform.hlsl"
#include "vertex.hlsl"

cbuffer ConstantBuffer : register(b0)
{
    Transform transform;
    bool has_texture;
    float2 tile_count;
}

VertexPositionColorTexture VSMain(VertexPositionColorTexture input)
{
	VertexPositionColorTexture output = (VertexPositionColorTexture)0;

	output.position = mul(WorldViewProjection(transform), float4(input.position.xyz, 1.0f));
	output.color = input.color;
	output.texture_coordinate = input.texture_coordinate * tile_count;

	return output;
}

Texture2D DiffuseMap : register(t0);
SamplerState Sampler : register(s0);

float4 PSMain(VertexPositionColorTexture input) : SV_Target
{
	float4 color = has_texture ? DiffuseMap.Sample(Sampler, input.texture_coordinate) : float4(1.0f, 1.0f, 1.0f, 1.0f);
	color *= input.color;
	return color;
}
