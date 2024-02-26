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

struct TransformData
{
    float3 offset;
};

cbuffer Transform : register(b0)
{
    TransformData transform;
}

PS_IN VSMain(VS_IN input)
{
	PS_IN output = (PS_IN)0;

	output.position = float4(input.position + transform.offset, 1.0f);
	output.color = input.color;

	return output;
}

float4 PSMain(PS_IN input) : SV_Target
{
	float4 color = input.color;
	return color;
}
