cbuffer Transform
{
    float4x4 world;
    float4x4 viewProj;
};

struct VS_Output
{
    float4 position      : SV_POSITION;
    float4 worldPosition : WORLD_POSITION;
    float3 normal        : NORMAL;
    float2 texCoord      : TEXCOORD;
};

VS_Output main(
    float3 inPos      : POSITION, 
    float3 inNormal   : NORMAL, 
    float2 inTexCoord : TEXCOORD
)
{
    VS_Output output;
    
    output.position = mul(mul(world, viewProj), float4(inPos, 1.0));
    output.worldPosition = mul(world, float4(inPos, 1.0));
    output.normal = mul(world, float4(inNormal, 1.0));
    
    output.texCoord = inTexCoord;
    
    return output;
}