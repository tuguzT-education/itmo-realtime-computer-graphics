struct VertexPositionNormalColorTexture
{
    float4 position : SV_Position;
    float3 normal : NORMAL;
	float4 color : COLOR;
	float2 texture_coordinate : TEXCOORD0;
};

struct VertexPositionColor
{
    float4 position : SV_Position;
    float4 color : COLOR;
};
