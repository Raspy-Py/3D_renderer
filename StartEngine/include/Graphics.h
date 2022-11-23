#pragma once
#include "CleanWindows.h"
#include "ExceptionBase.h"
#include "DxgiInfoManager.h"
#include "GraphicsExceptionMacroses.h"
#include "Camera.h"

#include <wrl.h>
#include <dxgi.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>

namespace wrl = Microsoft::WRL;

class Graphics
{
	friend class Renderer;
	friend class Bindable;
public:
	class Exception : public ExceptionBase
	{
		using ExceptionBase::ExceptionBase;
	};
	class HrException : public Exception
	{
	public:
		HrException(int line, const char* file, HRESULT hr, std::vector<std::string> infoMsgs = {}) noexcept;
		const char* what() const noexcept override;
		const char* GetType() const noexcept override;
		HRESULT GetErrorCode() const noexcept;
		std::string GetErrorString() const noexcept;
		std::string GetErrorDescription() const noexcept;
		std::string GetErrorInfo() const noexcept;
	private:
		HRESULT hr;
		std::string info;
	};
	class InfoException : public Exception
	{
	public:
		InfoException(int line, const char* file, std::vector<std::string> infoMsgs) noexcept;
		const char* what() const noexcept override;
		const char* GetType() const noexcept override;
		std::string GetErrorInfo() const noexcept;
	private:
		std::string info;
	};
	class DeviceRemovedException : public HrException
	{
		using HrException::HrException;
	public:
		const char* GetType() const noexcept override;
	private:
		std::string reason;
	};
private:
	struct Parameters
	{
		bool isVSyncEnabled;
		bool isFullScreen;
		unsigned int screenWidth;
		unsigned int screenHeight;
		unsigned int videoCardMemoryMB;
		char videoCardDescription[128];
	};
public:
	Graphics(HWND hWnd, unsigned int screenWidth, unsigned int screenHeight, bool fullscreen, bool vsync);
	Graphics(const Graphics&) = delete;
	Graphics& operator=(const Graphics&) = delete;
	~Graphics()
	{
		pSwapChain->SetFullscreenState(FALSE, NULL);
	}

	void ClearFrame(float r, float g, float b);
	void EndFrame();
	void DrawIndexed(unsigned int count);

	ID3D11Device* GetDevice();
	ID3D11DeviceContext* GetDeviceContext();
	ID3D11RenderTargetView* GetRenderTarget();
	IDXGISwapChain* GetSwapChain();
#if !NDEBUG
	DxgiInfoManager infoManager;
#endif
private:
	
	/*
	* Hardware parameters and settings
	*/
	Parameters params;

	/*
	* D3D COM objects
	*/
	wrl::ComPtr<IDXGISwapChain> pSwapChain;

	wrl::ComPtr<ID3D11Device> pDevice;
	wrl::ComPtr<ID3D11DeviceContext> pDeviceContext;
	wrl::ComPtr<ID3D11RenderTargetView> pTarget;

	wrl::ComPtr<ID3D11Texture2D> pBackBuffer;
	wrl::ComPtr<ID3D11Texture2D> pDepthStencilBuffer;

	wrl::ComPtr<ID3D11DepthStencilView> pDepthStencilView;
	wrl::ComPtr<ID3D11DepthStencilState> pDepthStencilState;
	wrl::ComPtr<ID3D11RasterizerState> pRasterState;
};

