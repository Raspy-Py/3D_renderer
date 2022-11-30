#pragma once
#include "CleanWindows.h"
#include "Graphics.h"
#include "Light.h"

#include <DirectXMath.h>
#include <memory>

using namespace DirectX;

class PointLight : public Light
{
public:
	PointLight(Graphics* gfx, const XMFLOAT3& pos, const XMFLOAT3& color, const XMFLOAT3& att, FLOAT intensity, FLOAT range);

	void Draw(Graphics* gfx) const noexcept;
	void Update(Graphics* gfx, float frameTime);
	
private:
	float globalTime = 0;
};
