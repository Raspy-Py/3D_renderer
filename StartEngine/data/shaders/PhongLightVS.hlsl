cbuffer ModelTransform : register(b0)
{
    float4x4 world;
}

cbuffer ViewerTransform : register(b1)
{
    float4x4 viewProj;
    float3 cameraPos;
};

struct VS_Output
{
    float4 position      : SV_POSITION;
    float4 worldPosition : WORLD_POSITION;
    float3 cameraPos     : CAMERA_POSITION;
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
    
    output.position = mul(mul(viewProj, world), float4(inPos, 1.0));
    output.worldPosition = mul(world, float4(inPos, 1.0));
    output.normal = mul((float3x3)world, inNormal);
    output.cameraPos = cameraPos;
    output.texCoord = float2(1.0, 1.0) - inTexCoord;
    
    return output;
}
