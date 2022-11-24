#include "Sampler.h"

Sampler::Sampler(Graphics* gfx)
{
	D3D11_SAMPLER_DESC sd = {};
	sd.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sd.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sd.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sd.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sd.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sd.MinLOD = 0;
	sd.MaxLOD = D3D11_FLOAT32_MAX;

	gfx->GetDevice()->CreateSamplerState(&sd, &pSampleState);
}

void Sampler::Bind(Graphics* gfx)
{
	gfx->GetDeviceContext()->PSSetSamplers(0, 1, pSampleState.GetAddressOf());
}
