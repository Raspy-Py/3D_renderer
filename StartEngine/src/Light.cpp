#include "Light.h"

Light::Params Light::params;
std::unique_ptr<PixelConstantBuffer<Light::Params>> Light::pConstBuffer;

Light::Light(Graphics* gfx)
{
	if (Light::pConstBuffer == nullptr)
	{
		pConstBuffer = std::make_unique<PixelConstantBuffer<Light::Params>>(gfx, 0, Light::params);
	}
}

void Light::Bind(Graphics* gfx)
{
	pConstBuffer->Bind(gfx);
}

Light::Params* Light::GetParams()
{
	return &params;
}
