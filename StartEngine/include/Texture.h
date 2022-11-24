#pragma once
#include "Bindable.h"
#include "DDSTextureLoader11.h"

class Texture : public Bindable
{
public:
	Texture(Graphics* gfx, const wchar_t* textureFileName);
	void Bind(Graphics* gfx) override;
private:
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> pShaderResourceView;
	Microsoft::WRL::ComPtr<ID3D11Resource> pShaderResource;
};

