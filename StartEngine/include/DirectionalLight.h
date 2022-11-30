#pragma once
#include "CleanWindows.h"
#include "Graphics.h"
#include "Light.h"

#include <DirectXMath.h>
#include <memory>
class DirectionalLight : public Light
{
public:
	DirectionalLight(Graphics* gfx, const XMFLOAT3& direction, const XMFLOAT3& color, FLOAT intensity);

	void Update(Graphics* gfx, float frameTime);

private:
	float globalTime = 0;
};

