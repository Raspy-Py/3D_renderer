#include "InputLayout.h"

InputLayout::InputLayout(Graphics* gfx, const std::vector<D3D11_INPUT_ELEMENT_DESC>& layouts, ID3DBlob* blob)
{
	INFOMAN(gfx);

	GFX_THROW_INFO(gfx->GetDevice()->CreateInputLayout(
		layouts.data(), (UINT)layouts.size(),
		blob->GetBufferPointer(),
		blob->GetBufferSize(),
		&pInputLayout
	));
}

void InputLayout::Bind(Graphics* gfx)
{
	gfx->GetDeviceContext()->IASetInputLayout(pInputLayout.Get());
}
