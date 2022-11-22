#include "PixelShader.h"

PixelShader::PixelShader(Graphics* gfx, const wchar_t* shaderFilePath)
{
	INFOMAN(*gfx);

	Microsoft::WRL::ComPtr<ID3DBlob> pBlob;
	GFX_THROW_INFO(D3DReadFileToBlob(shaderFilePath, &pBlob));

	GFX_THROW_INFO(gfx->GetDevice()->CreatePixelShader(
		pBlob->GetBufferPointer(),
		pBlob->GetBufferSize(),
		nullptr,
		&pPixelShader
	));
}

void PixelShader::Bind(Graphics* gfx)
{
}
