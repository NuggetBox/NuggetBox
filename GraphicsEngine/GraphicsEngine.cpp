#include "NuggetBox.pch.h"
#include "GraphicsEngine.h"

#include <iostream>

#include "Camera.h"
#include "Model.h"
#include "InputHandler.h"
#include "Timer.h"

bool GraphicsEngine::Initialize(unsigned someX, unsigned someY, unsigned someWidth, unsigned someHeight, bool enableDeviceDebug)
{
	// Initialize our window:
	WNDCLASS windowClass = {};
	windowClass.style = CS_VREDRAW | CS_HREDRAW | CS_OWNDC;
	windowClass.lpfnWndProc = GraphicsEngine::WinProc;
	windowClass.hCursor = LoadCursor(nullptr, IDC_ARROW);
	windowClass.lpszClassName = L"ModelViewer";
	RegisterClass(&windowClass);

	myWindowHandle = CreateWindow(
		L"ModelViewer",
		L"NuggetBox",
		WS_OVERLAPPEDWINDOW | WS_POPUP | WS_VISIBLE,
		someX,
		someY,
		someWidth,
		someHeight,
		nullptr, nullptr, nullptr,
		this
		);

	// F1 -- This is where we should init our Framework
	// ex: myFramework.Initialize(myWindowHandle, false);
	DX11::Initialize(myWindowHandle, enableDeviceDebug);

	Utility::Timer::Start();

	auto cube = Model::Load("Cube");
	cube->SetPosition(0, 0, 150);
	myScene.AddGameObject(cube);

	auto pyramid = Model::Load("Pyramid");
	pyramid->SetPosition(150, 0, 0);
	myScene.AddGameObject(pyramid);

	auto chest = Model::Load("Meshes/Particle_Chest.fbx");
	chest->SetPosition(-150, -50, 0);
	chest->AddRotation(0, 180, 0);
	myScene.AddGameObject(chest);

	auto gremlin = Model::Load("Meshes/gremlin.fbx");
	gremlin->AddPosition(0, -50, 0);
	gremlin->AddRotation(0, 180, 0);
	gremlin->LoadAnimation("Meshes/gremlin@walk.fbx", "Walk");
	gremlin->LoadAnimation("Meshes/gremlin@run.fbx", "Run");
	gremlin->PlayAnimation("Run");
	myScene.AddGameObject(gremlin);

	auto spiderCat = Model::Load("meshes/SpiderCat.fbx");
	spiderCat->AddPosition(0, 100, 0);
	spiderCat->AddRotation(0, 180, 0);
	myScene.AddGameObject(spiderCat);

	std::shared_ptr<Camera> camera = std::make_shared<Camera>();
	camera->SetPosition(0, 0, -300);
	myScene.SetCamera(camera);

	myForwardRenderer.Initialize();

	return true;
}

LRESULT CALLBACK GraphicsEngine::WinProc(_In_ HWND hWnd, _In_ UINT uMsg, _In_ WPARAM wParam, _In_ LPARAM lParam)
{
	// We want to be able to access the Graphics Engine instance from inside this function.
	static GraphicsEngine* graphicsEnginePtr = nullptr;

	if (InputHandler::UpdateEvents(uMsg, wParam, lParam))
	{
		return 0;
	}

	if (uMsg == WM_DESTROY || uMsg == WM_CLOSE)
	{
		PostQuitMessage(0);
	}
	else if (uMsg == WM_CREATE)
	{
		const CREATESTRUCT* createdStruct = reinterpret_cast<CREATESTRUCT*>(lParam);
		graphicsEnginePtr = static_cast<GraphicsEngine*>(createdStruct->lpCreateParams);
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

void GraphicsEngine::BeginFrame()
{
	Timer::Update();
	// F1 - This is where we clear our buffers and start the DX frame.
	// ex: myFramework.BeginFrame({1, 0.5f, 0, 1});
	DX11::BeginFrame({0.6f, 0.2f, 0.4f, 1.0f});
}

void GraphicsEngine::RenderFrame()
{
	auto& camera = myScene.GetCamera();
	auto& models = myScene.GetModels();

	float cameraSpeed = 100.0f;

	if (InputHandler::GetKeyHeld(VK_SHIFT))
	{
		cameraSpeed *= 3;
	}

	//TODO: Atomisera camera movement
	if (InputHandler::GetKeyHeld('W'))
	{
		camera->AddPosition(camera->GetTransform().GetForward() * Timer::GetDeltaTime() * cameraSpeed);
	}
	if (InputHandler::GetKeyHeld('A'))
	{
		camera->AddPosition(camera->GetTransform().GetLeft() * Timer::GetDeltaTime() * cameraSpeed);
	}
	if (InputHandler::GetKeyHeld('S'))
	{
		camera->AddPosition(camera->GetTransform().GetBackward() * Timer::GetDeltaTime() * cameraSpeed);
	}
	if (InputHandler::GetKeyHeld('D'))
	{
		camera->AddPosition(camera->GetTransform().GetRight() * Timer::GetDeltaTime() * cameraSpeed);
	}
	if (InputHandler::GetKeyHeld(VK_CONTROL))
	{
		camera->AddPosition(Vector3f(0, -1, 0) * Timer::GetDeltaTime() * cameraSpeed);
	}
	if (InputHandler::GetKeyHeld(VK_SPACE))
	{
		camera->AddPosition(Vector3f(0, 1, 0) * Timer::GetDeltaTime() * cameraSpeed);
	}

	if (InputHandler::GetRightClickHeld())
	{
		camera->SetRotation(Vector3f(camera->GetTransform().GetRotation().x + static_cast<float>(InputHandler::GetMouseDelta().y) / 5, 
			camera->GetTransform().GetRotation().y + static_cast<float>(InputHandler::GetMouseDelta().x) / 5, 0));
	}

	float rotationPerSec = 90.0f;

	for (auto& model : models) 
	{
		//model->AddRotation(0.f, rotationPerSec * Timer::GetDeltaTime(), 0.f);
		std::cout << "yo" << std::endl;
		model->Update();
	}

	myForwardRenderer.Render(camera, models);
	InputHandler::UpdatePreviousState();
}

void GraphicsEngine::EndFrame()
{
	// F1 - This is where we finish our rendering and tell the framework
	// to present our result to the screen.
	// ex: myFramework.EndFrame();
	DX11::EndFrame();
}
