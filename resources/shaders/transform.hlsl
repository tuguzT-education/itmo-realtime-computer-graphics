struct Transform
{
    float4x4 world;
    float4x4 view;
    float4x4 projection;
};

float4x4 WorldViewProjection(Transform transform)
{
    return mul(mul(transform.projection, transform.view), transform.world);
}
