#pragma once
#include "Drawable.h"
#include "Bindable.h"

class Cube : public Drawable
{
public:
	Cube(Graphics* gfx, float x, float y, float z);
	void Update(float frameTime) override;
	~Cube() override = default;
private:
	XMMATRIX GetTransformXM() const override;
private:
	float x;
	float y;
	float z;
};

