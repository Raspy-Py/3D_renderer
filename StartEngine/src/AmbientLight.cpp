#include "AmbientLight.h"

AmbientLight::AmbientLight(Graphics* gfx, const XMFLOAT3& color, FLOAT intensity)
	:
	Light(gfx)
{
	Light::params.ambient = { color, intensity };
}

void AmbientLight::Update(Graphics* gfx, float frameTime)
{
}
