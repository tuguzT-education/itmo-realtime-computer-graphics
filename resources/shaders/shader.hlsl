struct VS_IN
{
	float4 pos : POSITION0;
	float4 col : COLOR0;
};

struct PS_IN
{
	float4 pos : SV_POSITION;
 	float4 col : COLOR;
};

struct ConstantData
{
    float4 offset;
};

cbuffer ConstantBuffer : register(b0)
{
    ConstantData ConstData;
}

PS_IN VSMain(VS_IN input, uint vId : SV_VertexID)
{
	PS_IN output = (PS_IN)0;
	
	output.pos = float4(input.pos.xyz + ConstData.offset.xyz, 1.0f);
	output.col = input.col;
	
	return output;
}

float4 PSMain( PS_IN input ) : SV_Target
{
	float4 col = input.col;
	return col;
}
