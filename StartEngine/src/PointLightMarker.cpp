#include "PointLightMarker.h"

PointLightMarker::PointLightMarker(Graphics* gfx)
{
}

XMMATRIX PointLightMarker::GetTransformXM() const
{
	XMVECTOR vec = XMLoadFloat3(&(Light::GetParams()->point.pos));

	return XMMatrixTranslationFromVector(vec);
}

void PointLightMarker::Draw(Graphics* gfx) const
{
}

void PointLightMarker::Update(Graphics* gfx, float dt)
{
}

PointLightMarker::~PointLightMarker()
{
}
