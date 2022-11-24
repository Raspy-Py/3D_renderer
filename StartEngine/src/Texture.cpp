#include "Texture.h"

Texture::Texture(Graphics* gfx, const wchar_t* textureFileName)
{
	INFOMAN(gfx);

	DirectX::DDS_ALPHA_MODE alphaMode = DirectX::DDS_ALPHA_MODE::DDS_ALPHA_MODE_OPAQUE;

	GFX_THROW_INFO(CreateDDSTextureFromFile(
		gfx->GetDevice(), 
		textureFileName, 
		&pShaderResource, &pShaderResourceView, 
		0, &alphaMode));
}

void Texture::Bind(Graphics* gfx)
{
	gfx->GetDeviceContext()->PSSetShaderResources(0, 1, pShaderResourceView.GetAddressOf());
}
