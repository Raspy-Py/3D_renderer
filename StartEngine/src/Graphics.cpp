#include "Graphics.h"
#include "dxerr.h"
#include <vector>
#include <iostream>
#include <sstream>
#include <stdlib.h>

#pragma comment(lib,"d3d11.lib")
#pragma comment(lib,"dxgi.lib")
#pragma comment(lib,"D3DCompiler.lib")

Graphics::Graphics(HWND hWnd, unsigned int screenWidth, unsigned int screenHeight, bool fullscreen, bool vsync)
{
	params.screenWidth = screenWidth;
	params.screenHeight = screenHeight;
	params.isFullScreen = fullscreen;
	HRESULT hr;

	unsigned int numModes = 0, i, numerator, denominator;

	// Store the vsync setting.
	params.isVSyncEnabled = vsync;

	// Create a DirectX graphics interface factory.
	wrl::ComPtr<IDXGIFactory> factory;
	GFX_THROW_INFO(CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&factory));

	// Use the factory to create an adapter for the primary graphics interface (video card).
	wrl::ComPtr<IDXGIAdapter> adapter;
	GFX_THROW_INFO(factory->EnumAdapters(0, &adapter));

	// Enumerate the primary adapter output (monitor).
	wrl::ComPtr<IDXGIOutput> adapterOutput;
	GFX_THROW_INFO(adapter->EnumOutputs(0, &adapterOutput));

	// Get the number of modes that fit the DXGI_FORMAT_R8G8B8A8_UNORM display format for the adapter output (monitor).
	GFX_THROW_INFO(adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, NULL));

	// Create a list to hold all the possible display modes for this monitor/video card combination.
	std::vector<DXGI_MODE_DESC> displayModeList(numModes);

	// Now fill the display mode list structures.
	GFX_THROW_INFO(adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, displayModeList.data()));

	// Now go through all the display modes and find the one that matches the screen width and height.
	// When a match is found store the numerator and denominator of the refresh rate for that monitor.
	for(i=0; i<numModes; i++)
	{
		if(displayModeList[i].Width == screenWidth)
		{
			if(displayModeList[i].Height == screenHeight)
			{
				numerator = displayModeList[i].RefreshRate.Numerator;
				denominator = displayModeList[i].RefreshRate.Denominator;
			}
		}
	}

	// Get the adapter (video card) description.
	DXGI_ADAPTER_DESC adapterDesc;
	GFX_THROW_INFO(adapter->GetDesc(&adapterDesc));
	params.videoCardMemoryMB = (int)(adapterDesc.DedicatedVideoMemory / 1024 / 1024);

	// Initialize the swap chain description.
	DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
    swapChainDesc.BufferCount = 1;
    swapChainDesc.BufferDesc.Width = screenWidth;
    swapChainDesc.BufferDesc.Height = screenHeight;
    swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapChainDesc.OutputWindow = hWnd;
    swapChainDesc.SampleDesc.Count = 1;
    swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	swapChainDesc.Flags = 0;
	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	if(params.isVSyncEnabled)
	{
	    swapChainDesc.BufferDesc.RefreshRate.Numerator = numerator;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = denominator;
	}
	else
	{
	    swapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	}
	// Set to full screen or windowed mode.
	if(fullscreen)
		swapChainDesc.Windowed = false;
	else
		swapChainDesc.Windowed = true;
	D3D_FEATURE_LEVEL featureLevel = D3D_FEATURE_LEVEL_11_0;

	// Create the swap chain, Direct3D device, and Direct3D device context.
	GFX_THROW_INFO(D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, &featureLevel, 1, 
										   D3D11_SDK_VERSION, &swapChainDesc, &pSwapChain, &pDevice, NULL, &pDeviceContext));

	// Get the pointer to the back buffer.
	GFX_THROW_INFO(pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), &pBackBuffer));
	GFX_THROW_INFO(pDevice->CreateRenderTargetView(pBackBuffer.Get(), NULL, &pTarget));

	// Set up the description of the depth buffer.
	D3D11_TEXTURE2D_DESC depthBufferDesc;
	depthBufferDesc.Width = screenWidth;
	depthBufferDesc.Height = screenHeight;
	depthBufferDesc.MipLevels = 1;
	depthBufferDesc.ArraySize = 1;
	depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthBufferDesc.SampleDesc.Count = 1;
	depthBufferDesc.SampleDesc.Quality = 0;
	depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthBufferDesc.CPUAccessFlags = 0;
	depthBufferDesc.MiscFlags = 0;

	// Create the texture for the depth buffer using the filled out description.
	GFX_THROW_INFO(pDevice->CreateTexture2D(&depthBufferDesc, NULL, &pDepthStencilBuffer));

	// Initialize the description of the stencil state.
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc = {};
	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
	depthStencilDesc.StencilEnable = true;
	depthStencilDesc.StencilReadMask = 0xFF;
	depthStencilDesc.StencilWriteMask = 0xFF;

	depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	GFX_THROW_INFO(pDevice->CreateDepthStencilState(&depthStencilDesc, &pDepthStencilState));

	pDeviceContext->OMSetDepthStencilState(pDepthStencilState.Get(), 1);

	// Initialize the depth stencil view.
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc = {};
	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	GFX_THROW_INFO(pDevice->CreateDepthStencilView(pDepthStencilBuffer.Get(), &depthStencilViewDesc, &pDepthStencilView));

	pDeviceContext->OMSetRenderTargets(1, pTarget.GetAddressOf(), pDepthStencilView.Get());

	D3D11_RASTERIZER_DESC rasterDesc = {};
	rasterDesc.AntialiasedLineEnable = false;
	rasterDesc.CullMode = D3D11_CULL_NONE;
	rasterDesc.DepthBias = 0;
	rasterDesc.DepthBiasClamp = 0.0f;
	rasterDesc.DepthClipEnable = true;
	rasterDesc.FillMode = D3D11_FILL_SOLID;
	rasterDesc.FrontCounterClockwise = false;
	rasterDesc.MultisampleEnable = false;
	rasterDesc.ScissorEnable = false;
	rasterDesc.SlopeScaledDepthBias = 0.0f;

	// Create the rasterizer state from the description we just filled out.
	GFX_THROW_INFO(pDevice->CreateRasterizerState(&rasterDesc, &pRasterState));

	// Now set the rasterizer state.
	pDeviceContext->RSSetState(pRasterState.Get());
	
	// Setup the viewport for rendering.
	D3D11_VIEWPORT viewport = {};
    viewport.Width = (float)screenWidth;
    viewport.Height = (float)screenHeight;
    viewport.MinDepth = 0.0f;
    viewport.MaxDepth = 1.0f;
    viewport.TopLeftX = 0.0f;
    viewport.TopLeftY = 0.0f;

	// Create the viewport.
    pDeviceContext->RSSetViewports(1, &viewport);
	/*
	*  MY CODE
	*/
//
//	HRESULT hr; // used for exception throwing mocroses
//	UINT deviceFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;
//
//#if !NDEBUG
//	deviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
//#endif
//
//	DXGI_SWAP_CHAIN_DESC desc = {};
//    desc.Windowed = FALSE; // Sets the initial state of full-screen mode.
//    desc.BufferCount = 1;
//	desc.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
//	desc.BufferDesc.Width = 1920;
//	desc.BufferDesc.Height = 1080;
//    desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
//    desc.SampleDesc.Count = 1;      //multisampling setting
//    desc.SampleDesc.Quality = 0;    //vendor-specific flag
//    desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
//    desc.OutputWindow = hWnd;
//	desc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
//
//    GFX_THROW_INFO(D3D11CreateDeviceAndSwapChain(
//        nullptr,
//        D3D_DRIVER_TYPE_HARDWARE,
//        nullptr,
//        deviceFlags,
//        nullptr,
//        0,
//        D3D11_SDK_VERSION,
//        &desc,
//        &pSwapChain,
//        &pDevice,
//        nullptr,
//        &pDeviceContext
//	));
//
//	GFX_THROW_INFO(pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), &pBackBuffer));
//	GFX_THROW_INFO(pDevice->CreateRenderTargetView(pBackBuffer.Get(), nullptr, &pTarget));
//
//	D3D11_TEXTURE2D_DESC bbDesc;
//	pBackBuffer->GetDesc(&bbDesc);
//
//	CD3D11_TEXTURE2D_DESC depthStencilDesc(
//		DXGI_FORMAT::DXGI_FORMAT_D24_UNORM_S8_UINT,
//		static_cast<UINT>(bbDesc.Width),
//		static_cast<UINT>(bbDesc.Height),
//		1,
//		1,
//		D3D11_BIND_DEPTH_STENCIL
//	);
//
//	// creating texture for back buffer
//	GFX_THROW_INFO(pDevice->CreateTexture2D(
//		&depthStencilDesc,
//		nullptr,
//		&pDepthStencilBuffer
//	));
//
//	// creating view on back buffer texture
//	CD3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc(D3D11_DSV_DIMENSION_TEXTURE2D);
//	GFX_THROW_INFO(pDevice->CreateDepthStencilView(
//		pDepthStencilBuffer.Get(),
//		&depthStencilViewDesc,
//		&pDepthStencilView
//	));
//
//	D3D11_VIEWPORT viewPort = {};
//	viewPort.TopLeftX = 0;
//	viewPort.TopLeftY = 0;
//	viewPort.Height = (float)bbDesc.Height;
//	viewPort.Width = (float)bbDesc.Width;
//	viewPort.MinDepth = 0;
//	viewPort.MaxDepth = 1;
//
//	pDeviceContext->RSSetViewports(
//		1,
//		&viewPort
//	);
//
//	pDeviceContext->OMSetRenderTargets(1u, pTarget.GetAddressOf(), pDepthStencilView.Get());
}

void Graphics::ClearFrame(float r, float g, float b)
{
	const float color[] = { r,g,b,1.0f };
	pDeviceContext->ClearRenderTargetView(pTarget.Get(), color);
	pDeviceContext->ClearDepthStencilView(pDepthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0u);
}

void Graphics::BeginFrame(float r, float g, float b)
{
	ClearFrame(r, g, b);
}

void Graphics::EndFrame()
{
	pSwapChain->Present(1u, 0);
}

void Graphics::DrawIndexed(unsigned int count)
{
	GFX_THROW_INFO_ONLY(pDeviceContext->DrawIndexed(count, 0u, 0u));
}

ID3D11Device* Graphics::GetDevice()
{
	return pDevice.Get();
}

ID3D11DeviceContext* Graphics::GetDeviceContext()
{
	return pDeviceContext.Get();
}

ID3D11RenderTargetView* Graphics::GetRenderTarget()
{
	return pTarget.Get();
}

IDXGISwapChain* Graphics::GetSwapChain()
{
	return pSwapChain.Get();
}

/*
*  EXCEPTIONS
*/

Graphics::HrException::HrException(int line, const char* file, HRESULT hr, std::vector<std::string> infoMsgs) noexcept
	:
	Exception(line, file),
	hr(hr)
{
	// join all info messages with newlines into single string
	for (const auto& m : infoMsgs)
	{
		info += m;
		info.push_back('\n');
	}
	// remove final newline if exists
	if (!info.empty())
	{
		info.pop_back();
	}
}

const char* Graphics::HrException::what() const noexcept
{
	std::ostringstream oss;
	oss << GetType() << std::endl
		<< "[Error Code] 0x" << std::hex << std::uppercase << GetErrorCode()
		<< std::dec << " (" << (uint32_t)GetErrorCode() << ")" << std::endl
		<< "[Error String] " << GetErrorString() << std::endl
		<< "[Description] " << GetErrorDescription() << std::endl;
	if (!info.empty())
	{
		oss << "\n[Error Info]\n" << GetErrorInfo() << std::endl << std::endl;
	}
	oss << GetOriginString();
	whatBuffer = oss.str();
	return whatBuffer.c_str();
}

const char* Graphics::HrException::GetType() const noexcept
{
	return "Graphics Exception";
}

HRESULT Graphics::HrException::GetErrorCode() const noexcept
{
	return hr;
}

std::string Graphics::HrException::GetErrorString() const noexcept
{
	return DXGetErrorStringA(hr);
}

std::string Graphics::HrException::GetErrorDescription() const noexcept
{
	char buf[512];
	DXGetErrorDescriptionA(hr, buf, sizeof(buf));
	return buf;
}

std::string Graphics::HrException::GetErrorInfo() const noexcept
{
	return info;
}

const char* Graphics::DeviceRemovedException::GetType() const noexcept
{
	return "Graphics Exception [Device Removed] (DXGI_ERROR_DEVICE_REMOVED)";
}

Graphics::InfoException::InfoException(int line, const char* file, std::vector<std::string> infoMsgs) noexcept
	:
	Exception(line, file)
{
	// join all info messages with newlines into single string
	for (const auto& m : infoMsgs)
	{
		info += m;
		info.push_back('\n');
	}
	// remove final newline if exists
	if (!info.empty())
	{
		info.pop_back();
	}
}

const char* Graphics::InfoException::what() const noexcept
{
	std::ostringstream oss;
	oss << GetType() << std::endl
		<< "\n[Error Info]\n" << GetErrorInfo() << std::endl << std::endl;
	oss << GetOriginString();
	whatBuffer = oss.str();
	return whatBuffer.c_str();
}

const char* Graphics::InfoException::GetType() const noexcept
{
	return "Graphics Info Exception";
}

std::string Graphics::InfoException::GetErrorInfo() const noexcept
{
	return info;
}
