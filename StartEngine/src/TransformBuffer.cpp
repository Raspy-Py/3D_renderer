#include "TransformBuffer.h"

std::unique_ptr<VertexConstantBuffer<XMMATRIX>> TransformBuffer::pVcbuf;

TransformBuffer::TransformBuffer(Graphics* gfx, const Drawable* parent)
	:
	parent(parent)
{
	if (!pVcbuf)
	{
		pVcbuf = std::make_unique<VertexConstantBuffer<XMMATRIX>>(gfx);
	}
}

void TransformBuffer::Bind(Graphics* gfx)
{
	Camera* camera = Camera::GetInstance();
	pVcbuf->Update(gfx, 
		parent->GetTransformXM() * 
		camera->GetViewMatrix() * 
		camera->GetProjectionMatrix()
		);
	pVcbuf->Bind(gfx);
}
