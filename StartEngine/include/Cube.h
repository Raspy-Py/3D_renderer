#pragma once
#include "Drawable.h"
#include "Bindable.h"

class Cube : public Drawable
{
private:
	struct WVP
	{
		XMMATRIX world;
		XMMATRIX viewProj;
	};
public:
	Cube() = default;
	Cube(Graphics* gfx, float x, float y, float z);
	void Update(Graphics* gfx, float frameTime) override;
	~Cube() override = default;
private:
	XMMATRIX GetTransformXM() const override;
private:
	float x;
	float y;
	float z;

	WVP wvp;
};

