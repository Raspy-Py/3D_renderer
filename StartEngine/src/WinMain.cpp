#include "CleanWindows.h"
#include "Window.h"
#include "Graphics.h"
#include "ExceptionBase.h"
#include "App.h"

int CALLBACK WinMain(
	_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPSTR lpCmdLine,
	_In_ int nCmdShow
)
{
	try
	{
		App{}.Run();
	}
	catch (const ExceptionBase& e)
	{
		MessageBoxA(nullptr, e.what(), e.GetType(), MB_OK | MB_ICONEXCLAMATION);
	}
	catch (const std::exception& e)
	{
		MessageBoxA(nullptr, e.what(), "Standart Exception", MB_OK | MB_ICONEXCLAMATION);
	}
	catch (...)
	{
		MessageBoxA(nullptr, "No details", "Unknown Exception", MB_OK | MB_ICONEXCLAMATION);
	}
	return 0;
}