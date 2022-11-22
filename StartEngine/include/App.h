#pragma once
#include "CleanWindows.h"
#include "Window.h"
#include "Camera.h"
#include "Renderer.h"
#include "Timer.h"

#include <memory>
#include <exception>
#include <utility>

class App
{

public:
	App() = default;
	~App() = default;

	int Run();

private:
	void DoFrame();

private:
	MSG msg;
	Timer timer;
	POINT lastMousePos;

	std::unique_ptr<Graphics> pGraphics;
	std::unique_ptr<Window> pWindow;
	std::unique_ptr<Renderer> pRenderer;

	std::shared_ptr<Camera> pCamera;
};

