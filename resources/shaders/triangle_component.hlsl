struct VS_IN
{
	float3 position : POSITION0;
	float4 color : COLOR0;
};

struct PS_IN
{
	float4 position : SV_POSITION;
 	float4 color : COLOR;
};

struct Transform
{
    float4x4 wvp_matrix;
};

cbuffer TransformBuffer : register(b0)
{
    Transform transform;
}

PS_IN VSMain(VS_IN input)
{
	PS_IN output = (PS_IN)0;

	output.position = mul(transform.wvp_matrix, float4(input.position, 1.0f));
	output.color = input.color;

	return output;
}

float4 PSMain(PS_IN input) : SV_Target
{
	float4 color = input.color;
	return color;
}
