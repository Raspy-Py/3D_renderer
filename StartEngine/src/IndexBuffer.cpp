#include "IndexBuffer.h"

IndexBuffer::IndexBuffer(Graphics* gfx, const std::vector<uint16_t>& indices)
	:
	count(indices.size())
{
	INFOMAN(gfx);

	D3D11_BUFFER_DESC ibd = {};
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.Usage = D3D11_USAGE_DEFAULT;
	ibd.ByteWidth = UINT(indices.size() * sizeof(uint16_t));
	ibd.StructureByteStride = sizeof(uint16_t);

	D3D11_SUBRESOURCE_DATA isd = {};
	isd.pSysMem = indices.data();
	GFX_THROW_INFO(gfx->GetDevice()->CreateBuffer(&ibd, &isd, &pIndexBuffer));
}

void IndexBuffer::Bind(Graphics* gfx)
{
	gfx->GetDeviceContext()->IASetIndexBuffer(pIndexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0u);
}

UINT IndexBuffer::GetCount() const noexcept
{
	return count;
}
