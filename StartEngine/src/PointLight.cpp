#include "PointLight.h"

PointLight::PointLight(Graphics* gfx, const XMFLOAT3& pos, const XMFLOAT3& color, const XMFLOAT3& att, FLOAT intensity, FLOAT range)
	:
	Light(gfx)
{
	Light::params.point = { pos, color, att, intensity, range };
}

void PointLight::Draw(Graphics* gfx) const noexcept
{
 // TODO: add some indacator for light pos
}

void PointLight::Update(Graphics* gfx, float frameTime)
{
	globalTime += frameTime;
	Light::params.point.pos.x = cos(globalTime * 3.0) * 10.0;
	Light::params.point.pos.z = sin(globalTime * 3.0) * 10.0;
	pConstBuffer->Update(gfx, Light::params);
}
