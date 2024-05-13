struct VertexPositionColorTexture
{
    float4 position : SV_Position;
	float4 color : COLOR;
	float2 texture_coordinate : TEXCOORD;
};

struct VertexPositionColor
{
    float4 position : SV_Position;
    float4 color : COLOR;
};
