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
    AmbientLight ambient_light;
    DirectionalLight directional_light;
    PointLight point_light;
}

typedef VS_Output PS_Input;

float4 PhongLightning(float3 to_light_direction, float4 light_diffuse,
                      Material material, float3 position, float3 normal)
{
    float3 to_view_direction = normalize(view_position - position);
    float3 reflect_direction = normalize(reflect(to_light_direction, normal));

    float4 diffuse = max(dot(to_light_direction, normal), 0.0f) * light_diffuse * material.diffuse;
    float4 specular = pow(max(dot(-to_view_direction, reflect_direction), 0.0f), material.exponent) * material.specular;
    return diffuse + specular;
}

float4 PSMain(PS_Input input) : SV_Target
{
    float4 color = has_texture ? DiffuseMap.Sample(Sampler, input.texture_coordinate) : float4(1.0f, 1.0f, 1.0f, 1.0f);
    color *= input.color;

    float4 d_diffuse_specular = PhongLightning(-directional_light.direction, directional_light.color,
                                               material, input.world_position, input.normal);

    float3 p_light_direction = point_light.position - input.world_position;
    float p_distance = length(p_light_direction);
    float4 p_attenuation = point_light.attenuation.const_factor +
                           point_light.attenuation.linear_factor * p_distance +
                           point_light.attenuation.quad_factor * p_distance * p_distance;
    float4 p_diffuse_specular = PhongLightning(normalize(p_light_direction), point_light.color,
                                               material, input.world_position, input.normal) / p_attenuation;

    float4 ambient = ambient_light.color * material.ambient;
    float4 diffuse_specular = d_diffuse_specular + p_diffuse_specular;
    float4 emissive = material.emissive;
    return color * (ambient + diffuse_specular + emissive);
}
