cbuffer cbPerObject
{
	float4x4 WVP;
};

struct VS_OUTPUT
{
	float4 position : SV_POSITION;
	float4 color : COLOR;
};

VS_OUTPUT main(float4 inPos : POSITION, float4 inColor : COLOR)
{
	VS_OUTPUT output;

	output.position = mul(WVP, inPos);
	output.color = inColor;

	return output;
}