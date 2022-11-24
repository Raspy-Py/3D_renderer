#pragma once
#include "Graphics.h"
#include "IndexBuffer.h"

#include <memory>
#include <DirectXMath.h>

class Bindable;

class Drawable
{
public:
	Drawable() = default;
	Drawable(const Drawable&) = delete;
	virtual XMMATRIX GetTransformXM() const = 0;
	void Draw(Graphics* gfx) const;
	virtual void Update(Graphics* gfx, float dt) = 0;
	virtual ~Drawable() = default;
protected:
	void AddBind(std::unique_ptr<Bindable> bind);
	void AddIndexBuffer(std::unique_ptr<IndexBuffer> ibuf);
//private:
//	virtual const std::vector<std::unique_ptr<Bindable>>& GetStaticBinds() const = 0;
protected:
	const IndexBuffer* pIndexBuffer = nullptr;
	std::vector<std::unique_ptr<Bindable>> binds;
};