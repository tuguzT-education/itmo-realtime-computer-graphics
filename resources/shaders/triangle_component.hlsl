#include "transform.hlsl"
#include "vertex.hlsl"

cbuffer VSConstantBuffer : register(b0)
{
    Transform transform;
    float2 tile_count;
}

VertexPositionNormalColorTexture VSMain(VertexPositionNormalColorTexture input)
{
	VertexPositionNormalColorTexture output = (VertexPositionNormalColorTexture)0;

	output.position = mul(WorldViewProjection(transform), float4(input.position.xyz, 1.0f));
	output.normal = mul(transform.world, input.normal);
	output.color = input.color;
	output.texture_coordinate = input.texture_coordinate * tile_count;

	return output;
}

Texture2D DiffuseMap : register(t0);
SamplerState Sampler : register(s0);

cbuffer PSConstantBuffer : register(b0)
{
    bool has_texture;
    float3 view_position;
}

float4 PSMain(VertexPositionNormalColorTexture input) : SV_Target
{
	float4 color = has_texture ? DiffuseMap.Sample(Sampler, input.texture_coordinate) : float4(1.0f, 1.0f, 1.0f, 1.0f);
	color *= input.color;
	return color;
}
