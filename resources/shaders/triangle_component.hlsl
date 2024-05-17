#include "transform.hlsl"
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
    AmbientLight ambient_light;
    DirectionalLight directional_light;
}

typedef VS_Output PS_Input;

float4 PSMain(PS_Input input) : SV_Target
{
    float4 color = has_texture ? DiffuseMap.Sample(Sampler, input.texture_coordinate) : float4(1.0f, 1.0f, 1.0f, 1.0f);
    color *= input.color;

    const float3 view_direction = normalize(view_position - input.world_position);
    const float3 light_direction = normalize(-directional_light.direction);
    const float3 reflect_direction = reflect(light_direction, input.normal);

    float4 ambient = color * ambient_light.color;
    float4 diffuse = saturate(dot(light_direction, input.normal) * directional_light.color * color);
    float4 specular = pow(max(0.0f, dot(-view_direction, reflect_direction)), 32.0f) * 0.3f;
    return ambient + diffuse + specular;
}
