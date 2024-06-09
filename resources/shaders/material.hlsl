#pragma pack_matrix(row_major)

struct Material
{
    float4 ambient;
    float4 diffuse;
    float4 specular;
    float4 emissive;
    float exponent;
};
