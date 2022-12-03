#pragma once
#include "Drawable.h"
#include "PointLight.h"

class PointLightMarker : public Drawable
{
public:
	PointLightMarker(Graphics* gfx);

	XMMATRIX GetTransformXM() const;
	void Draw(Graphics* gfx) const;
	void Update(Graphics* gfx, float dt);
	~PointLightMarker();
};

