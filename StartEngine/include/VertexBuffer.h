#pragma once
#include "Bindable.h"

class VertexBuffer : public Bindable
{
public:
	template<class Vertex>
	VertexBuffer(Graphics* gfx, const std::vector<Vertex>& vertices);

	void Bind(Graphics* gfx) override;
private:
	UINT stride;
	Microsoft::WRL::ComPtr<ID3D11Buffer> pVertexBuffer;
};

template<class Vertex>
VertexBuffer::VertexBuffer(Graphics* gfx, const std::vector<Vertex>& vertices)
	:
	stride(sizeof(Vertex))
{
	INFOMAN(*gfx);

	D3D11_BUFFER_DESC vbd = {};
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.Usage = D3D11_USAGE_DEFAULT;
	vbd.CPUAccessFlags = 0u;
	vbd.MiscFlags = 0u;
	vbd.ByteWidth = UINT(sizeof(Vertex) * vertices.size());
	vbd.StructureByteStride = sizeof(Vertex);

	D3D11_SUBRESOURCE_DATA vsd = {};
	vsd.pSysMem = vertices.data();

	GFX_THROW_INFO(gfx->GetDevice()->CreateBuffer(&vbd, &vsd, &pVertexBuffer));
}
