#pragma once
#include "Bindable.h"

class VertexShader : public Bindable
{
public:
	VertexShader(Graphics* gfx, const wchar_t* shaderFilePath);
	void Bind(Graphics* gfx) override;

	ID3DBlob* GetBlob();
private:
	Microsoft::WRL::ComPtr<ID3D11VertexShader> pVertexShader;
	Microsoft::WRL::ComPtr<ID3DBlob> pBlob;
};

