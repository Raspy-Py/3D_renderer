#include "App.h"

int App::Run()
{
	pWindow = std::make_unique<Window>(L"Window name", false, 1920, 1080, false);
	pCamera = Camera::GetInstance();
	pWindow->mouse.Hide();
	pCube = std::make_unique<Cube>(pWindow->GetGfx(), 0.0f, 0.0f, 0.0f);

	pCamera->SetProjectionValues(70, 16.f / 9.0f, 0.01f, 1000.0f);
	pGraphics = pWindow->GetGfx();

	RECT winRect = {};
	GetWindowRect(pWindow->GetWindow(), &winRect);
	POINT newMousePos{
		(winRect.right - winRect.left) / 2,
		(winRect.bottom - winRect.top) / 2
	};
	lastMousePos = newMousePos;
	SetCursorPos(newMousePos.x, newMousePos.y);

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
	OutputDebugString(L"Debug %d");

	RECT winRect = {};
	GetWindowRect(pWindow->GetWindow(), &winRect);
	POINT lastMousePos{
		(winRect.right - winRect.left) / 2,
		(winRect.bottom - winRect.top) / 2
	};
	POINT newMousePos{  };
	GetCursorPos(&newMousePos);
	POINT mouseDelta = {
		newMousePos.x - lastMousePos.x,
		newMousePos.y - lastMousePos.y
	};

	// Controle
	{
		float speed = 2.0f;
		if (pWindow->keyboard.KeyIsPressed('D'))
			pCamera->StrafeRight(deltaTime * speed);
		if (pWindow->keyboard.KeyIsPressed('A'))
			pCamera->StrafeLeft(deltaTime * speed);
		if (pWindow->keyboard.KeyIsPressed('W'))
			pCamera->MoveForward(deltaTime * speed);
		if (pWindow->keyboard.KeyIsPressed('S'))
			pCamera->MoveBackward(deltaTime * speed);
		if (pWindow->keyboard.KeyIsPressed(VK_SHIFT))
			pCamera->MoveDown(deltaTime * speed);
		if (pWindow->keyboard.KeyIsPressed(VK_SPACE))
			pCamera->MoveUp(deltaTime * speed);
		if (pWindow->keyboard.KeyIsPressed(VK_ESCAPE))
			PostQuitMessage(0);

		if (pWindow->mouse.IsInWindow())
		{
			pCamera->AdjustRotation((float)mouseDelta.y * 0.002, (float)mouseDelta.x * 0.002, 0.0f);
			SetCursorPos(lastMousePos.x, lastMousePos.y); 
		}
	}

	pGraphics->ClearFrame(0.5, 0.4, 1.0);
	pCube->Update(pWindow->GetGfx(), deltaTime);
	pCube->Draw(pWindow->GetGfx());
	pGraphics->EndFrame();
}
