#include "App.h"

#include "SceneImporter.h"

int App::Run()
{
	SceneImporter scene("./data/scenes/skull/12140_Skull_v3_L2.obj");

	pWindow = std::make_unique<Window>(L"Window name", false, 1920, 1080, false);
	pCamera = Camera::GetInstance();
	pWindow->mouse.Hide();
	pCube = std::make_unique<Cube>(pWindow->GetGfx(), 0.0f, 0.0f, 0.0f);

	pCamera->SetProjectionValues(70, 16.f / 9.0f, 0.01f, 1000.0f);
	pGraphics = pWindow->GetGfx();

	XMFLOAT3 pPos = { 9.9, 4.0, 0.0, };
	XMFLOAT3 pColor = { 1.0, 1.0, 1.0 };
	XMFLOAT3 att = { 1.0, 0.027, 0.0028 };
	FLOAT pInts = 1.0;
	FLOAT range = 160;
	pPointLight = std::make_unique<PointLight>(pWindow->GetGfx(), pPos, pColor, att, pInts, range);

	XMFLOAT3 dir = {4.0, -7.0, 3.0};
	XMFLOAT3 dColor = { 1.0, 1.0, 1.0 };
	FLOAT dInts = 0.4;
	pDirectionalLight = std::make_unique<DirectionalLight>(pWindow->GetGfx(), dir, dColor, dInts);

	XMFLOAT3 aColor = {1.0, 1.0, 1.0,};
	FLOAT aInts = 0.1;
	pAmbientLight = std::make_unique<AmbientLight>(pWindow->GetGfx(), aColor, aInts);



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
	pPointLight->Update(pWindow->GetGfx(), deltaTime);
	Light::Bind(pWindow->GetGfx());

	pGraphics->ClearFrame(0.5, 0.4, 1.0);
	pCube->Update(pWindow->GetGfx(), deltaTime);
	pCube->Draw(pWindow->GetGfx());
	pGraphics->EndFrame();
}
