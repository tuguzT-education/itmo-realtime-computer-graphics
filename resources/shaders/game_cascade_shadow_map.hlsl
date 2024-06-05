cbuffer GSShadowMapConstantBuffer : register(b0)
{
    float4x4 shadow_map_view_projections[SHADOW_MAP_CASCADE_COUNT];
    float4 shadow_map_distances[((SHADOW_MAP_CASCADE_COUNT - 1) / 4) + 1];
};

struct GS_Input
{
    float4 position : POSITION0;
};

struct GS_Output
{
    float4 position : SV_Position;
    uint index : SV_RenderTargetArrayIndex;
};

[instance(SHADOW_MAP_CASCADE_COUNT)]
[maxvertexcount(3)]
void GSMain(triangle GS_Input inputs[3], in uint id : SV_GSInstanceID, inout TriangleStream<GS_Output> stream)
{
    [unroll]
    for (int i = 0; i < 3; ++i)
    {
        GS_Output output = (GS_Output) 0;
        output.position = mul(shadow_map_view_projections[id], float4(inputs[i].position.xyz, 1.0f));
        output.index = id;

        stream.Append(output);
    }
}
