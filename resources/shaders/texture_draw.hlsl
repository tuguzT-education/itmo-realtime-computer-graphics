#pragma pack_matrix(row_major)

cbuffer ConstBuf : register(b0) {
    float4x4 WorldViewProj;
}

struct VS_IN
{
    float3 pos : POSITION;
    float2 tex : TEXCOORD0;
};

struct PS_IN
{
    float4 pos : SV_POSITION;
    float2 tex : TEXCOORD0;
};

Texture2D		DiffuseMap		: register(t0);
SamplerState	Sampler			: register(s0);


PS_IN VSMain( VS_IN input )
{
    PS_IN output = (PS_IN)0;

    output.pos = mul(float4(input.pos, 1.0f), WorldViewProj);
    output.tex = input.tex;

    return output;
}

float4 PSMain( PS_IN input ) : SV_Target
{
    float4 color = DiffuseMap.SampleLevel(Sampler, input.tex, 0);

    return float4(color.r, color.r, color.r, 1.0f);
}
