struct VS_OUTPUT
{
	float4 position : SV_POSITION;
	float2 texCoord : TEXCOORD;
};

Texture2D ObjTexture;
SamplerState ObjSamplerState;

float4 main(VS_OUTPUT input) : SV_TARGET
{
	float4 OutColor = ObjTexture.Sample(ObjSamplerState, float2(1.0, 1.0) - input.texCoord);
	clip(OutColor.a -.1);
	return OutColor;
}