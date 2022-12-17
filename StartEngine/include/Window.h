#pragma once
#include "CleanWindows.h"
#include "ExceptionBase.h"
#include "Graphics.h"
 
// Input devices
#include "Mouse.h"
#include "Keyboard.h"

#include <memory>

class Window
{
private:
	class WindowClass
	{
	public:
		static const wchar_t* GetName() noexcept;
		static HINSTANCE GetInstance() noexcept;
	private:
		WindowClass() noexcept;
		~WindowClass();
		WindowClass(const WindowClass&) = delete;
		WindowClass& operator=(const WindowClass&) = delete;
		static constexpr const wchar_t* wndClassName = L"Direct3D Window";
		static WindowClass wndClass;
		HINSTANCE hInst;
	};
public:
	class Exception : public ExceptionBase
	{
		using ExceptionBase::ExceptionBase;
	public:
		static std::string TranslateErrorCode(HRESULT hr) noexcept;
	};
	class HrException : public Exception
	{
	public:
		HrException(int line, const char* file, HRESULT hr) noexcept;
		const char* what() const noexcept override;
		const char* GetType() const noexcept override;
		HRESULT GetErrorCode() const noexcept;
		std::string GetErrorDescription() const noexcept;
	private:
		HRESULT hr;
	};
	class NoGfxException : public Exception
	{
	public:
		using Exception::Exception;
		const char* GetType() const noexcept override;
	};
public:
	Window(const wchar_t* name, bool fullscreen = true, int height = 450, int width = 800, bool vsync = true) noexcept(NDEBUG);
	~Window();
	Window(const Window&) = delete;
	Window& operator=(const Window&) = delete;
	static std::optional<int> ProcessMessages() noexcept;
	void ProcessInput(float deltaTime) noexcept;

	HWND GetWindow();
	Graphics* GetGfx();
private:
	static LRESULT CALLBACK HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
	static LRESULT CALLBACK HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
	LRESULT HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
public:
	Mouse mouse;
	Keyboard keyboard;
private:
	int width;
	int height; 
	HWND hWnd;
	std::unique_ptr<Graphics> pGfx;
};
