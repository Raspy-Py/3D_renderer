#include "DirectionalLight.h"

DirectionalLight::DirectionalLight(Graphics* gfx, const XMFLOAT3& direction, const XMFLOAT3& color, FLOAT intensity)
	:
	Light(gfx)
{
	Light::params.directional = { direction, color, intensity };
}

void DirectionalLight::Update(Graphics* gfx, float frameTime)
{
}
