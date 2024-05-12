struct Transform
{
    float4x4 world;
    float4x4 view;
    float4x4 projection;
};

cbuffer ConstantBuffer : register(b0)
{
    Transform transform;
    bool has_texture;
    float2 tile_count;
}

struct VS_Input
{
	float3 position : SV_Position;
	float4 color : COLOR;
	float2 texcoord : TEXCOORD;
};

struct VS_Output
{
    float4 position : SV_Position;
    float4 color : COLOR;
    float2 texcoord : TEXCOORD;
};

VS_Output VSMain(VS_Input input)
{
	VS_Output output = (VS_Output)0;

    float4x4 wvp = mul(mul(transform.projection, transform.view), transform.world);
	output.position = mul(wvp, float4(input.position, 1.0f));
	output.color = input.color;
	output.texcoord = input.texcoord * tile_count;

	return output;
}

Texture2D DiffuseMap : register(t0);
SamplerState Sampler : register(s0);

typedef VS_Output PS_Input;

float4 PSMain(PS_Input input) : SV_Target
{
	float4 color = has_texture ? DiffuseMap.Sample(Sampler, input.texcoord) : float4(1.0f, 1.0f, 1.0f, 1.0f);
	color *= input.color;
	return color;
}
