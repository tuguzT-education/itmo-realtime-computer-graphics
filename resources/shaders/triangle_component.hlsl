#include "transform.hlsl"
#include "material.hlsl"
#include "light.hlsl"

cbuffer VSConstantBuffer : register(b0)
{
    Transform transform;
    float2 tile_count;
}

struct VS_Input
{
    float3 position : POSITION;
    float3 normal : NORMAL;
    float4 color : COLOR;
    float2 texture_coordinate : TEXCOORD0;
};

struct VS_Output
{
    float4 position : SV_Position;
    float3 normal : NORMAL;
    float4 color : COLOR;
    float2 texture_coordinate : TEXCOORD0;
    float3 world_position : TEXCOORD1;
};

VS_Output VSMain(VS_Input input)
{
    VS_Output output = (VS_Output)0;

    output.position = mul(WorldViewProjection(transform), float4(input.position, 1.0f));
    output.normal = normalize(mul(transform.world, float4(input.normal, 0.0f)).xyz);
    output.color = input.color;
    output.texture_coordinate = input.texture_coordinate * tile_count;
    output.world_position = mul(transform.world, float4(input.position, 1.0f)).xyz;

    return output;
}

Texture2D DiffuseMap : register(t0);
SamplerState Sampler : register(s0);

cbuffer PSConstantBuffer : register(b0)
{
    bool has_texture;
    float3 view_position;
    Material material;
    DirectionalLight directional_light;
    PointLight point_light;
    SpotLight spot_light;
}

typedef VS_Output PS_Input;

float4 PhongLightning(in Light light, in Material material, float3 position, float3 normal, float3 to_light_direction)
{
    float3 to_view_direction = normalize(view_position - position);
    float3 reflect_direction = normalize(reflect(to_light_direction, normal));

    float4 ambient = light.ambient * material.ambient;
    float4 diffuse = max(dot(to_light_direction, normal), 0.0f) * light.diffuse * material.diffuse;
    float4 specular = pow(max(dot(-to_view_direction, reflect_direction), 0.0f), material.exponent)
                          * light.specular * material.specular;
    return ambient + diffuse + specular;
}

float4 PSMain(PS_Input input) : SV_Target
{
    float4 color = has_texture ? DiffuseMap.Sample(Sampler, input.texture_coordinate) : float4(1.0f, 1.0f, 1.0f, 1.0f);
    color *= input.color;

    Light d_light;
    d_light.ambient = directional_light.ambient;
    d_light.diffuse = directional_light.diffuse;
    d_light.specular = directional_light.specular;
    float4 d_color = PhongLightning(d_light, material, input.world_position, input.normal, -directional_light.direction);

    float3 p_light_direction = point_light.position - input.world_position;
    float p_distance = length(p_light_direction);
    float4 p_attenuation = point_light.attenuation.const_factor +
                           point_light.attenuation.linear_factor * p_distance +
                           point_light.attenuation.quad_factor * p_distance * p_distance;
    p_attenuation = (length(p_attenuation) > 0.0f) ? p_attenuation : float4(1.0f, 1.0f, 1.0f, 1.0f);
    Light p_light;
    p_light.ambient = point_light.ambient;
    p_light.diffuse = point_light.diffuse;
    p_light.specular = point_light.specular;
    float4 p_color = PhongLightning(p_light, material, input.world_position, input.normal, normalize(p_light_direction))
                         / p_attenuation;

    float3 s_light_direction = normalize(-spot_light.direction);
    float3 s_to_vertex_direction = spot_light.position - input.world_position;
    float s_distance = length(s_to_vertex_direction);
    float4 s_attenuation = spot_light.attenuation.const_factor +
                           spot_light.attenuation.linear_factor * s_distance +
                           spot_light.attenuation.quad_factor * s_distance * s_distance;
    s_attenuation = (length(s_attenuation) > 0.0f) ? s_attenuation : float4(1.0f, 1.0f, 1.0f, 1.0f);
    float4 s_cos_alpha = dot(s_light_direction, normalize(s_to_vertex_direction));
    float falloff = spot_light.inner_cone_angle != spot_light.outer_cone_angle
                        ? (s_cos_alpha - cos(spot_light.outer_cone_angle / 2.0f)) /
                          (cos(spot_light.inner_cone_angle / 2.0f) - cos(spot_light.outer_cone_angle / 2.0f))
                        : 0.0f;
    falloff = max(falloff, 0.0f);
    Light s_light;
    s_light.ambient = spot_light.ambient;
    s_light.diffuse = spot_light.diffuse;
    s_light.specular = spot_light.specular;
    float4 s_color = PhongLightning(s_light, material, input.world_position, input.normal, s_light_direction)
                         / s_attenuation * falloff;

    float4 dps_color = d_color + p_color + s_color;
    float4 emissive = material.emissive;
    return color * (dps_color + emissive);
}
