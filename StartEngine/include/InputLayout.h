#pragma once
#include "Bindable.h"

class InputLayout : public Bindable
{
public:
	InputLayout(Graphics* gfx, const std::vector<D3D11_INPUT_ELEMENT_DESC>& layouts, ID3DBlob* blob);
	void Bind(Graphics* gfx) override;
private:
	Microsoft::WRL::ComPtr<ID3D11InputLayout> pInputLayout;
};

