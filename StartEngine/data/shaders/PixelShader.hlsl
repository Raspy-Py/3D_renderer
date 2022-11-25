struct VS_OUTPUT
{
	float4 position : SV_POSITION;
	float2 texCoord : TEXCOORD;
	float3 normal : NORMAL;
};

Texture2D ObjTexture;
SamplerState ObjSamplerState;

float4 main(VS_OUTPUT input) : SV_TARGET
{
	float4 albedo = ObjTexture.Sample(ObjSamplerState, float2(1.0, 1.0) - input.texCoord);

	clip(albedo.a -.1);

	float3 lightDir = normalize(float3(-2.0, 1.0, 3.0));

	float3 diffuse = dot(input.normal, -lightDir);
	float3 ambient = float3(0.1, 0.1, 0.1);

	float3 lightColor = float3(1.0, 1.0, 1.0);

	float4 OutColor = albedo * float4(ambient, 1.0);
	OutColor += saturate(float4(diffuse * lightColor, OutColor.a) * albedo);

	OutColor = pow(OutColor, 1.0/2.2);

	return OutColor;
}