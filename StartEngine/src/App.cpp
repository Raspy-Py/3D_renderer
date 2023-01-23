#include "App.h"

#include <imgui_impl_dx11.h>
#include <imgui_impl_win32.h>
#include <imgui.h>

#include "SceneImporter.h"

App::App()
{
	pWindow = std::make_unique<Window>(L"Window name", false, 1920, 1080, false);
	pModel = std::make_unique<Model>(pWindow->GetGfx(), 
		"./data/scenes/muro/muro.obj", 
		L"PhongLightVS.cso",
		L"PhongLightPS.cso");
	pCamera = Camera::GetInstance();
	pCamera->SetProjectionValues(70, 16.f / 9.0f, 0.01f, 1000.0f);
	pCamera->SetPosition(3.0f, 2.0f, -3.0f);
		
	ImGui::CreateContext();	
	Graphics* gfx = pWindow->GetGfx();
	ImGui_ImplWin32_Init(pWindow->GetWindow());
	ImGui_ImplDX11_Init(gfx->GetDevice(), gfx->GetDeviceContext());

	XMFLOAT3 pPos = { 9.9, -4.0, 0.0, };
	XMFLOAT3 pColor = { 1.0, 1.0, 1.0 };
	XMFLOAT3 att = { 1.0, 0.027, 0.0028 };
	FLOAT pInts = 1.0;
	FLOAT range = 160;
	pPointLight = std::make_unique<PointLight>(pWindow->GetGfx(), pPos, pColor, att, pInts, range);

	XMFLOAT3 dir = { 0.0, -2.0, -1.0 };
	XMFLOAT3 dColor = { 1.0, 1.0, 1.0 };
	FLOAT dInts = 0.6;
	pDirectionalLight = std::make_unique<DirectionalLight>(pWindow->GetGfx(), dir, dColor, dInts);

	XMFLOAT3 aColor = { 1.0, 1.0, 1.0 };
	FLOAT aInts = 0.1;
	pAmbientLight = std::make_unique<AmbientLight>(pWindow->GetGfx(), aColor, aInts);
}

App::~App()
{
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

int App::Run()
{
	while (true)
	{
		if (const auto code = Window::ProcessMessages())
		{
			return *code;
		}

		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();

		DoFrame();	
	}
}

void App::DoFrame()
{
	float deltaTime = timer.Mark();
	pWindow->ProcessInput(deltaTime);
		
	// Setting up ImGui elements
	{
		ImGui::Begin("Setting");
		ImGui::ColorEdit3("point light", reinterpret_cast<float*>(&(Light::GetParams()->point.color)));
		ImGui::ColorEdit3("directional light", reinterpret_cast<float*>(&(Light::GetParams()->directional.color)));
		ImGui::ColorEdit3("ambient light", reinterpret_cast<float*>(&(Light::GetParams()->ambient.color)));
		ImGui::SliderFloat3("point light position", reinterpret_cast<float*>(&(Light::GetParams()->point.pos)), -10.0f, 10.0f);
		ImGui::SliderFloat3("dir light dir", reinterpret_cast<float*>(&(Light::GetParams()->directional.direction)), -1.0f, 1.0f);
		ImGui::End();
	}

	//Updating resources
	Light::Update(pWindow->GetGfx());
	Light::Bind(pWindow->GetGfx());

	// Begin new frame
	pWindow->GetGfx()->ClearFrame(0, 0, 0.01);

	// Drawing D3D stuff
	pModel->Draw(pWindow->GetGfx());

	// Drawing ImGui stuff
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	pWindow->GetGfx()->EndFrame();
}
