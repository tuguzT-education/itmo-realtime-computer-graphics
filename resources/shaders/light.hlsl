struct AmbientLight
{
    float4 color;
};

struct DirectionalLight
{
    float3 direction;
    float4 color;
};

struct Attenuation
{
    float4 const_factor;
    float4 linear_factor;
    float4 quad_factor;
};

struct PointLight
{
    float3 position;
    float4 color;
    Attenuation attenuation;
};

struct SpotLight {
    float3 position;
    float3 direction;
    float4 color;
    Attenuation attenuation;
    float inner_cone_angle;
    float outer_cone_angle;
};
