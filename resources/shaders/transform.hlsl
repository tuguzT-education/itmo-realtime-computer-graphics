#pragma pack_matrix(row_major)

struct Transform
{
    float4x4 world;
    float4x4 view;
    float4x4 projection;
};

float4x4 WorldViewProjection(in Transform transform)
{
    return mul(mul(transform.world, transform.view), transform.projection);
}
