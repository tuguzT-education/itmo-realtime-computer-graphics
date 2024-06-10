#pragma pack_matrix(row_major)

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
    float3 position : POSITION0;
    float3 normal : NORMAL0;
    float4 color : COLOR0;
    float2 texture_coordinates : TEXCOORD0;
};

struct VS_Output
{
    float4 position : SV_Position;
    float3 normal : NORMAL0;
    float4 color : COLOR0;
    float2 texture_coordinates : TEXCOORD0;
    float3 world_position : TEXCOORD1;
    float3 world_view_position : TEXCOORD2;
};

VS_Output VSMain(VS_Input input)
{
    VS_Output output = (VS_Output)0;

    output.position = mul(float4(input.position, 1.0f), WorldViewProjection(transform));
    output.normal = normalize(mul(float4(input.normal, 0.0f), transform.world).xyz);
    output.color = input.color;
    output.texture_coordinates = input.texture_coordinates * tile_count;
    output.world_position = mul(float4(input.position, 1.0f), transform.world).xyz;
    output.world_view_position = mul(float4(output.world_position, 1.0f), transform.view).xyz;

    return output;
}

Texture2DArray ShadowMapDirectionalLight : register(t0);
SamplerComparisonState ShadowMapSampler : register(s0);

Texture2D DiffuseMap : register(t1);
SamplerState TextureSampler : register(s1);

cbuffer PSShadowMapConstantBuffer : register(b0)
{
    float4x4 shadow_map_view_projections[SHADOW_MAP_CASCADE_COUNT];
    float4 shadow_map_debug_colors[SHADOW_MAP_CASCADE_COUNT];
    float4 shadow_map_distances[((SHADOW_MAP_CASCADE_COUNT - 1) / 4) + 1];
};

cbuffer PSConstantBuffer : register(b1)
{
    bool has_texture;
    float3 view_position;
    Material material;
    DirectionalLight directional_light;
    PointLight point_light;
    SpotLight spot_light;
}

typedef VS_Output PS_Input;

float4 PhongAmbientLightning(in Light light, in Material material)
{
    return light.ambient * material.ambient;
}

float4 PhongDiffuseLightning(in Light light, in Material material, float3 normal, float3 to_light_direction)
{
    float diffuse = max(dot(to_light_direction, normal), 0.0f);
    return diffuse * light.diffuse * material.diffuse;
}

float4 PhongSpecularLightning(in Light light, in Material material, float3 world_position, float3 normal,
                              float3 to_light_direction)
{
    float3 to_view_direction = normalize(view_position - world_position);
    float3 reflect_direction = normalize(reflect(to_light_direction, normal));

    float specular = pow(max(dot(-to_view_direction, reflect_direction), 0.0f), material.exponent);
    return specular * light.specular * material.specular;
}

float4 PhongLightning(in Light light, in Material material,
                      float3 world_position, float3 normal, float3 to_light_direction)
{
    float4 ambient = PhongAmbientLightning(light, material);
    float4 diffuse = PhongDiffuseLightning(light, material, normal, to_light_direction);
    float4 specular = PhongSpecularLightning(light, material, world_position, normal, to_light_direction);
    return ambient + diffuse + specular;
}

float4 DirectionalLightningShadow(float3 world_position, float3 world_view_position)
{
    int shadow_map_slice = SHADOW_MAP_CASCADE_COUNT - 1;
    float world_view_distance = abs(world_view_position.z);
    for (int i = 0; i < SHADOW_MAP_CASCADE_COUNT; i++)
    {
        float shadow_map_distance = shadow_map_distances[i / 4][i % 4];
        if (world_view_distance < shadow_map_distance)
        {
            shadow_map_slice = i;
            break;
        }
    }

    float4 light_position = mul(float4(world_position, 1.0f), shadow_map_view_projections[shadow_map_slice]);
    light_position /= light_position.w;

    float2 shadow_map_coordinates = (light_position.xy + float2(1.0f, 1.0f)) * 0.5f;
    shadow_map_coordinates.y = 1.0f - shadow_map_coordinates.y;

    float4 result = 0.0f;
    float3 texel_size = 1.0f / SHADOW_MAP_RESOLUTION;
    for (float x = -1.0f; x <= 1.0f; x += 1.0f)
    {
        for (float y = -1.0f; y <= 1.0f; y += 1.0f)
        {
            float current_depth = light_position.z;
            float3 sample_coordinates = float3(shadow_map_coordinates + float2(x, y) * texel_size, shadow_map_slice);
            result += ShadowMapDirectionalLight.SampleCmpLevelZero(ShadowMapSampler, sample_coordinates, current_depth);
        }
    }
    result /= 9.0f;

    result *= shadow_map_debug_colors[shadow_map_slice];
    return result;
}

float4 DirectionalLightning(in DirectionalLight directional_light, in Material material,
                            float3 world_position, float3 normal, float3 world_view_position)
{
    Light light;
    light.ambient = directional_light.ambient;
    light.diffuse = directional_light.diffuse;
    light.specular = directional_light.specular;

    float3 to_light_direction = -directional_light.direction;

    float4 ambient = PhongAmbientLightning(light, material);
    float4 diffuse = PhongDiffuseLightning(light, material, normal, to_light_direction);
    float4 specular = PhongSpecularLightning(light, material, world_position, normal, to_light_direction);

    return ambient + (diffuse + specular) * DirectionalLightningShadow(world_position, world_view_position);
}

float4 PointLightning(in PointLight point_light, in Material material,
                      float3 world_position, float3 normal)
{
    float3 to_light = point_light.position - world_position;
    float to_light_distance = length(to_light);
    float4 attenuation = point_light.attenuation.const_factor +
                         point_light.attenuation.linear_factor * to_light_distance +
                         point_light.attenuation.quad_factor * to_light_distance * to_light_distance;
    attenuation = (length(attenuation) > 0.0f) ? attenuation : float4(1.0f, 1.0f, 1.0f, 1.0f);

    Light light;
    light.ambient = point_light.ambient;
    light.diffuse = point_light.diffuse;
    light.specular = point_light.specular;

    float3 to_light_direction = normalize(to_light);

    return PhongLightning(light, material, world_position, normal, to_light_direction) / attenuation;
}

float4 SpotLightning(in SpotLight spot_light, in Material material,
                     float3 world_position, float3 normal)
{
    float3 to_light_direction = normalize(-spot_light.direction);

    float3 to_vertex = spot_light.position - world_position;
    float to_vertex_distance = length(to_vertex);
    float4 attenuation = spot_light.attenuation.const_factor +
                         spot_light.attenuation.linear_factor * to_vertex_distance +
                         spot_light.attenuation.quad_factor * to_vertex_distance * to_vertex_distance;
    attenuation = (length(attenuation) > 0.0f) ? attenuation : float4(1.0f, 1.0f, 1.0f, 1.0f);

    float4 cos_alpha = dot(to_light_direction, normalize(to_vertex));
    float falloff = spot_light.inner_cone_angle != spot_light.outer_cone_angle
                        ? (cos_alpha - cos(spot_light.outer_cone_angle / 2.0f)) /
                          (cos(spot_light.inner_cone_angle / 2.0f) - cos(spot_light.outer_cone_angle / 2.0f))
                        : 0.0f;
    falloff = max(falloff, 0.0f);

    Light light;
    light.ambient = spot_light.ambient;
    light.diffuse = spot_light.diffuse;
    light.specular = spot_light.specular;

    return PhongLightning(light, material, world_position, normal, to_light_direction) / attenuation * falloff;
}

float4 PSMain(PS_Input input) : SV_Target
{
    float4 color = has_texture
                       ? DiffuseMap.Sample(TextureSampler, input.texture_coordinates)
                       : float4(1.0f, 1.0f, 1.0f, 1.0f);
    color *= input.color;

    float4 dl_color = DirectionalLightning(directional_light, material, input.world_position, input.normal,
                                           input.world_view_position);
    float4 pl_color = PointLightning(point_light, material, input.world_position, input.normal);
    float4 sl_color = SpotLightning(spot_light, material, input.world_position, input.normal);

    float4 l_color = dl_color + pl_color + sl_color;
    float4 emissive = material.emissive;
    return color * (l_color + emissive);
}
