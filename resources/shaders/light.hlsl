struct Light
{
    float4 ambient;
    float4 diffuse;
    float4 specular;
};

struct DirectionalLight : Light
{
    float3 direction;
};

struct Attenuation
{
    float4 const_factor;
    float4 linear_factor;
    float4 quad_factor;
};

struct PointLight : Light
{
    float3 position;
    Attenuation attenuation;
};

struct SpotLight : Light {
    float3 direction;
    float3 position;
    Attenuation attenuation;
    float inner_cone_angle;
    float outer_cone_angle;
};
