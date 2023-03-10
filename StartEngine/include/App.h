#pragma once
#include "CleanWindows.h"

// Systems and subsystems
#include "Window.h"
#include "Camera.h"
#include "Timer.h"

// Scene
#include "Model.h"
#include "PointLight.h"
#include "DirectionalLight.h"
#include "AmbientLight.h"

#include <memory>
#include <exception>
#include <utility>

class App
{

public:
	App();
	~App();

	int Run();

private:
	void DoFrame();

private:
	MSG msg;
	Timer timer;
	POINT lastMousePos;
	Camera* pCamera;

	std::unique_ptr<Model> pModel;
	std::unique_ptr<PointLight> pPointLight;
	std::unique_ptr<DirectionalLight> pDirectionalLight;
	std::unique_ptr<AmbientLight> pAmbientLight;
	std::unique_ptr<Window> pWindow;
};

