#pragma once
#include "CleanWindows.h"
#include "Window.h"
#include "Camera.h"
#include "Renderer.h"
#include "Timer.h"
#include "Cube.h"

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
	Camera* pCamera;
	Graphics* pGraphics;

	std::unique_ptr<Cube> pCube;
	std::unique_ptr<Window> pWindow;
	std::unique_ptr<Renderer> pRenderer;

};

