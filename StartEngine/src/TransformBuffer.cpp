#include "TransformBuffer.h"

std::unique_ptr<VertexConstantBuffer<XMMATRIX>> TransformBuffer::pVcbuf;

TransformBuffer::TransformBuffer(Graphics* gfx, UINT slot, const Drawable* parent)
	:
	parent(parent)
{
	if (!pVcbuf)
	{
		pVcbuf = std::make_unique<VertexConstantBuffer<XMMATRIX>>(gfx, slot);
	}
}

void TransformBuffer::Bind(Graphics* gfx)
{
	pVcbuf->Update(gfx, XMMatrixIdentity());
	pVcbuf->Bind(gfx);
}
