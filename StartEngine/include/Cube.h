#pragma once
#include "Drawable.h"
#include "Bindable.h"

class Cube : public Drawable
{
private:
	struct SceneTransform
	{
		XMMATRIX world;
		XMMATRIX viewProj;
		XMFLOAT3 camPos;
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

	SceneTransform sceneTransform;
};

