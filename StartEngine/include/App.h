#pragma once
#include "CleanWindows.h"

// Systems and subsystems
#include "Window.h"
#include "Camera.h"
#include "Timer.h"

// Scene
#include "Cube.h"
#include "PointLight.h"
#include "DirectionalLight.h"
#include "AmbientLight.h"

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
	std::unique_ptr<PointLight> pPointLight;
	std::unique_ptr<DirectionalLight> pDirectionalLight;
	std::unique_ptr<AmbientLight> pAmbientLight;
	std::unique_ptr<Window> pWindow;
};

