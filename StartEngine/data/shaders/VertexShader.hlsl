cbuffer cbPerObject
{
	float4x4 world;
	float4x4 VP;
}

struct VS_OUTPUT
{
	float4 position : SV_POSITION;
	float2 texCoord : TEXCOORD;
	float3 normal : NORMAL;
};

VS_OUTPUT main(float3 inPos : POSITION, float2 inTexCoord : TEXCOORD, float3 inNormal : NORMAL)
{
	VS_OUTPUT output;
	float4x4 WVP = mul(world, VP);
	output.position = mul(WVP, float4(inPos, 1.0));
    output.normal = mul(world, float4(inNormal, 1.0)).xyz;
	output.texCoord = inTexCoord;

	return output;
}