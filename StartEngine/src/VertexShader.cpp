#include "VertexShader.h"

VertexShader::VertexShader(Graphics* gfx, const wchar_t* shaderFilePath)
{
    INFOMAN(gfx);

    GFX_THROW_INFO(D3DReadFileToBlob(shaderFilePath, &pBlob));

    GFX_THROW_INFO(gfx->GetDevice()->CreateVertexShader(
        pBlob->GetBufferPointer(),
        pBlob->GetBufferSize(),
        nullptr,
        &pVertexShader
    ));
}

void VertexShader::Bind(Graphics* gfx)
{
    gfx->GetDeviceContext()->VSSetShader(pVertexShader.Get(), nullptr, 0);
}

ID3DBlob* VertexShader::GetBlob()
{
    return pBlob.Get();
}
