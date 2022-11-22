#include "VertexBuffer.h"

void VertexBuffer::Bind(Graphics* gfx)
{
	UINT offset = 0;
	gfx->GetDeviceContext()->IASetVertexBuffers(0, 1u, pVertexBuffer.GetAddressOf(), &stride, &offset);
}
