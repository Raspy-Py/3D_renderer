#pragma once
#include "Bindable.h"

template<typename C>
class ConstantBuffer : public Bindable
{
public:
	void Update(Graphics* gfx, const C& consts)
	{
		INFOMAN(gfx);
		D3D11_MAPPED_SUBRESOURCE msr;
		GFX_THROW_INFO(gfx->GetDeviceContext()->Map(
			pConstantBuffer.Get(), 0u,
			D3D11_MAP_WRITE_DISCARD, 0u,
			&msr
		));
		memcpy(msr.pData, &consts, sizeof(consts));
		gfx->GetDeviceContext()->Unmap(pConstantBuffer.Get(), 0u);
	}
	ConstantBuffer(Graphics* gfx, UINT slot, const C& consts)
		:
		slot(slot)
	{
		INFOMAN(gfx);
		D3D11_BUFFER_DESC cbd;
		cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		cbd.Usage = D3D11_USAGE_DYNAMIC;
		cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		cbd.MiscFlags = 0u;
		cbd.ByteWidth = sizeof(consts);
		cbd.StructureByteStride = 0u;

		D3D11_SUBRESOURCE_DATA csd = {};
		csd.pSysMem = &consts;
		ID3D11Device* device = gfx->GetDevice();
		GFX_THROW_INFO(device->CreateBuffer(&cbd, &csd, &pConstantBuffer));
	}
	ConstantBuffer(Graphics* gfx, UINT slot)
		:
		slot(slot)
	{
		INFOMAN(gfx);
		D3D11_BUFFER_DESC cbd;
		cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		cbd.Usage = D3D11_USAGE_DYNAMIC;
		cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		cbd.MiscFlags = 0u;
		cbd.ByteWidth = sizeof(C);
		cbd.StructureByteStride = 0u;
		GFX_THROW_INFO(gfx->GetDevice()->CreateBuffer(&cbd, nullptr, &pConstantBuffer));
	}
protected:
	UINT slot;
	Microsoft::WRL::ComPtr<ID3D11Buffer> pConstantBuffer;
};

template<typename C>
class VertexConstantBuffer : public ConstantBuffer<C>
{
	using ConstantBuffer<C>::pConstantBuffer;
	using ConstantBuffer<C>::slot;
public:
	using ConstantBuffer<C>::ConstantBuffer;
	void Bind(Graphics* gfx) override
	{
		gfx->GetDeviceContext()->VSSetConstantBuffers(slot, 1u, pConstantBuffer.GetAddressOf());
	}
};

template<typename C>
class PixelConstantBuffer : public ConstantBuffer<C>
{
	using ConstantBuffer<C>::pConstantBuffer;
	using ConstantBuffer<C>::slot;
public:
	using ConstantBuffer<C>::ConstantBuffer;
	void Bind(Graphics* gfx) override
	{
		gfx->GetDeviceContext()->PSSetConstantBuffers(slot, 1u, pConstantBuffer.GetAddressOf());
	}
};
