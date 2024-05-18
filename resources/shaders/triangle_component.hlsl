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

float4 PSMain(PS_Input input) : SV_Target
{
    float4 color = has_texture ? DiffuseMap.Sample(Sampler, input.texture_coordinate) : float4(1.0f, 1.0f, 1.0f, 1.0f);
    color *= input.color;

    float3 view_direction = normalize(input.world_position - view_position);

    float3 d_light_direction = normalize(-directional_light.direction);
    float3 d_reflect_direction = reflect(d_light_direction, input.normal);
    float4 d_diffuse = saturate(dot(d_light_direction, input.normal) * directional_light.color * material.diffuse * color);
    float4 d_specular = saturate(pow(max(0.0f, dot(view_direction, d_reflect_direction)), material.exponent) * material.specular);

    float3 p_light_direction = normalize(point_light.position - input.world_position);
    float3 p_reflect_direction = reflect(p_light_direction, input.normal);
    float p_distance = length(p_light_direction);
    float4 p_attenuation = point_light.attenuation.const_factor +
                           point_light.attenuation.linear_factor * p_distance +
                           point_light.attenuation.quad_factor * p_distance * p_distance;
    float4 p_diffuse = saturate(dot(p_light_direction, input.normal) * point_light.color * material.diffuse * color) / p_attenuation;
    float4 p_specular = saturate(pow(max(0.0f, dot(view_direction, p_reflect_direction)), material.exponent) * material.specular) / p_attenuation;

    float4 ambient = color * ambient_light.color * material.ambient;
    float4 diffuse = d_diffuse + p_diffuse;
    float4 specular = d_specular + p_specular;
    float4 emissive = material.emissive;
    return saturate(ambient + diffuse + specular + emissive);
}
