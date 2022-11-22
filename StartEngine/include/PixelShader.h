#pragma once
#include "Bindable.h"

class PixelShader : public Bindable
{
public:
	PixelShader(Graphics* gfx, const wchar_t* shaderFilePath);
	void Bind(Graphics* gfx) override;
private:
	Microsoft::WRL::ComPtr<ID3D11PixelShader> pPixelShader;
};

