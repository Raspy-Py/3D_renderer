#include "App.h"

int App::Run()
{
	pWindow = std::make_unique<Window>(L"Window name",true);
	pGraphics = std::make_unique<Graphics>(pWindow->GetWindow());
	pRenderer = std::make_unique<Renderer>(pGraphics.get(), L"VertexShader.cso", L"PixelShader.cso");

	pCamera.reset(Camera::GetInstance());
	//pWindow->mouse.Hide();


	while (true)
	{
		if (const auto code = Window::ProcessMessages())
		{
			return *code;
		}
		DoFrame();	
	}
}

void App::DoFrame()
{
	float deltaTime = timer.Mark();

	RECT winRect = {};
	GetWindowRect(pWindow->GetWindow(), &winRect);

	POINT newMousePos{  };
	GetCursorPos(&newMousePos);
	ScreenToClient(pWindow->GetWindow(), &newMousePos);
	int cx = (winRect.right - winRect.left) / 2;
	int cy = (winRect.bottom - winRect.top) / 2;
	POINT deltaMousePos = {newMousePos.x - cx, newMousePos.y - cy};
	newMousePos = { cx, cy };
	ClientToScreen(pWindow->GetWindow(), &newMousePos);
	// Controle
	{
		if (pWindow->keyboard.KeyIsPressed('D'))
			pCamera->StrafeRight(deltaTime);
		if (pWindow->keyboard.KeyIsPressed('A'))
			pCamera->StrafeLeft(deltaTime);
		if (pWindow->keyboard.KeyIsPressed('W'))
			pCamera->MoveForward(deltaTime);
		if (pWindow->keyboard.KeyIsPressed('S'))
			pCamera->MoveBackward(deltaTime);
		if (pWindow->keyboard.KeyIsPressed(VK_SHIFT))
			pCamera->MoveDown(deltaTime);
		if (pWindow->keyboard.KeyIsPressed(VK_SPACE))
			pCamera->MoveUp(deltaTime);
		if (pWindow->keyboard.KeyIsPressed(VK_ESCAPE))
			PostQuitMessage(0);


		//pWindow->mouse.SetPos(newMousePos.x, newMousePos.y);

	}


	pRenderer->DoFrame(deltaTime);
}
