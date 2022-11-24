#pragma once
#include "Bindable.h"
#include "ConstantBuffers.h"
#include "Drawable.h"

#include <memory>
#include <DirectXMath.h>

class TransformBuffer : public Bindable
{
public:
	TransformBuffer(Graphics* gfx, const Drawable* parent);
	void Bind(Graphics* gfx) override;
private:
	static std::unique_ptr<VertexConstantBuffer<XMMATRIX>> pVcbuf;
	const Drawable* parent;
};
