#include "NuggetBox.pch.h"
#include "GraphicsEngine.h"

#include <json.hpp>
#include <shellapi.h>

#include "Camera.h"
#include "Core/DebugLogger.h"
#include "Model/Model.h"
#include "InputHandler.h"
#include "Timer.h"
#include "imgui/imgui.h"
#include "Scene/Hierarchy.h"

bool GraphicsEngine::Initialize(unsigned someX, unsigned someY, unsigned someWidth, unsigned someHeight, bool enableDeviceDebug)
{
	DebugLogger::SetupCrashDump();

	InitializeWindow(someX, someY, someWidth, someHeight);

	DX11::Initialize(myWindowHandle, enableDeviceDebug);
	SetupBlendStates();
	SetupDepthStencilStates();
	SetupSamplerStates();

	RECT clientRect;
	GetClientRect(myWindowHandle, &clientRect);
	POINT clientSize = { clientRect.right - clientRect.left, clientRect.bottom - clientRect.top };

	Utility::Timer::Start();

	auto spiderCat = Model::Load("meshes/SpiderCat.fbx");
	spiderCat->AddPosition(0, 150, 0);
	spiderCat->AddRotation(0, 180, 0);
	myScene.AddModel(spiderCat);

	auto plane = Model::Load("Plane");
	plane->SetScale({ 100, 100, 100 });
	myScene.AddModel(plane);

	auto cube = Model::Load("Cube");
	cube->SetPosition(350, 50, 0);
	cube->ShouldSpin();
	myScene.AddModel(cube);

	auto pyramid = Model::Load("Pyramid");
	pyramid->SetPosition(200, 50, 0);
	myScene.AddModel(pyramid);

	auto chest = Model::Load("Meshes/Particle_Chest.fbx");
	chest->SetPosition(-200, 0, 0);
	chest->AddRotation(0, 180, 0);
	myScene.AddModel(chest);

	auto gremlin = Model::Load("Meshes/gremlin.fbx");
	gremlin->AddPosition(-20, 0, 0);
	gremlin->AddRotation(0, 180, 0);
	gremlin->LoadAnimation("Meshes/gremlin@walk.fbx", "Walk");
	gremlin->LoadAnimation("Meshes/gremlin@run.fbx", "Run");
	gremlin->PlayAnimation("Walk");
	myScene.AddModel(gremlin);

	auto gremlin2 = Model::Load("Meshes/gremlin.fbx");
	gremlin2->AddPosition(40, 0, 0);
	gremlin2->AddRotation(0, 180, 0);
	gremlin2->LoadAnimation("Meshes/gremlin@walk.fbx", "Walk");
	gremlin2->LoadAnimation("Meshes/gremlin@run.fbx", "Run");
	gremlin2->PlayAnimation("Run");
	myScene.AddModel(gremlin2);

	auto sphere = Model::Load("meshes/sphere.fbx");
	sphere->AddPosition(500, 250, 0);
	sphere->SetScale(Vector3f(50, 50, 50));
	myScene.AddModel(sphere);

	/*auto skybox = Model::Load("meshes/Sphere.fbx");
	skybox->SetScale(Vector3f(9000, 9000, 9000));
	myScene.AddModel(skybox);*/

	std::shared_ptr<Camera> camera = std::make_shared<Camera>();
	camera->SetPosition(0, 100, -500);
	myScene.SetCamera(camera);

	std::shared_ptr<ParticleSystem> system = std::make_shared<ParticleSystem>();
	system->SetPosition(0, 170, 0);
	system->LoadAndInitialize("Json/ParticleSystems/System1.json");
	myScene.AddParticleSystem(system);

	std::shared_ptr<ParticleSystem> fire = std::make_shared<ParticleSystem>();
	fire->SetPosition(200, 97, 0);
	fire->LoadAndInitialize("Json/ParticleSystems/System2.json");
	myScene.AddParticleSystem(fire);

	myScene.SetDirectionalLight(DirectionalLight::Create(Vector3f::One(), 1.0f, Vector3f(45, -45, 0)));
	myScene.SetAmbientLight(AmbientLight::Create("Textures/skansen_cubemap.dds"));

	//POINT LIGHT TESTING
	auto pointLight = PointLight::Create({ 0, 1, 0 }, 100000, { -500, 50, 0 }, 300);
	auto pointTestCube = Model::Load("Cube");
	pointTestCube->ShouldSpin();
	pointTestCube->SetPosition(-600, 50, 0);

	/*auto pointLight2 = PointLight::Create({ 0, 1, 0 }, 100000, { -200, 50, 0 }, 300);
	myScene.AddPointLight(pointLight2);*/

	myScene.AddModel(pointTestCube);
	myScene.AddPointLight(pointLight);
	//

	// Add some random pointlights
	/*for (int i = 0; i < 32 - 1; ++i)
	{
		auto pointLight = PointLight::Create({ 
			(std::rand() % 100) / 100.0f,  (std::rand() % 100) / 100.0f, (std::rand() % 100) / 100.0f }, 
			10000, { (static_cast<float>(i) - 32 / 2 + 1.5f) * 100.0f, 50.0f, -80.0f }, 300);
		myScene.AddPointLight(pointLight);
	}*/
	//

	auto spotLight = SpotLight::Create({1, 0, 1}, 5999999, { 500, 600, 0 }, 1000, {90, 0, 0}, 75, 100);
	myScene.AddSpotLight(spotLight);

	myIntermediateTargetA = RenderTarget::Create(clientSize.x, clientSize.y, DXGI_FORMAT_R32G32B32A32_FLOAT);
	myIntermediateTargetB = RenderTarget::Create(clientSize.x, clientSize.y, DXGI_FORMAT_R32G32B32A32_FLOAT);
	myHalfSizeTarget = RenderTarget::Create(clientSize.x / 2, clientSize.y / 2, DXGI_FORMAT_R32G32B32A32_FLOAT);
	myQuarterSizeTarget = RenderTarget::Create(clientSize.x / 4, clientSize.y / 4, DXGI_FORMAT_R32G32B32A32_FLOAT);
	myBlurTargetA = RenderTarget::Create(clientSize.x / 4, clientSize.y / 4, DXGI_FORMAT_R32G32B32A32_FLOAT);
	myBlurTargetB = RenderTarget::Create(clientSize.x / 4, clientSize.y / 4, DXGI_FORMAT_R32G32B32A32_FLOAT);

	mySSAOTarget = RenderTarget::Create(clientSize.x, clientSize.y, DXGI_FORMAT_R32_FLOAT);
	myBlueNoise = Texture::Load("Textures/BlueNoise.dds");

	myForwardRenderer.Initialize();
	myDeferredRenderer.Initialize();
	myShadowRenderer.Initialize();
	myPostProcessRenderer.Initialize();

	myLerpAnimations = true;
	myDragAccept = false;

	myEditor.Initialize(myClearColor, myLerpAnimations);

	Hierarchy::Initialize();

	myGBuffer = GBuffer::CreateGBuffer(clientRect);

	Timer::Update();
	DEBUGLOG("Graphics Engine Initialized");
	return true;
}

void GraphicsEngine::InitializeWindow(unsigned someX, unsigned someY, unsigned someWidth, unsigned someHeight)
{
	// Initialize our window:
	WNDCLASS windowClass = {};
	windowClass.style = CS_VREDRAW | CS_HREDRAW | CS_OWNDC;
	windowClass.lpfnWndProc = WinProc;
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

	DEBUGLOG("Initialized Window");
}

void GraphicsEngine::ResizeWindow(HWND aHWND, UINT someWidth, UINT someHeight)
{
	//TODO: Resize Window WIP
	//Resize funkar så länge fönstret är mindre än ursprungligen
	if (DX11::SwapChain)
	{
		DX11::Context->OMSetRenderTargets(0, 0, 0);

		DX11::BackBuffer->Release();

		AssertIfFailed(DX11::SwapChain->ResizeBuffers(2, 0, 0, DXGI_FORMAT_UNKNOWN, 0));

		ComPtr<ID3D11Texture2D> backBufferTexture;
		AssertIfFailed(DX11::SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)(backBufferTexture.GetAddressOf())));
		AssertIfFailed(DX11::Device->CreateRenderTargetView(backBufferTexture.Get(), nullptr, DX11::BackBuffer.GetAddressOf()));

		DX11::Context->OMSetRenderTargets(1, DX11::BackBuffer.GetAddressOf(), DX11::DepthBuffer.Get());

		RECT clientRect;
		GetClientRect(aHWND, &clientRect);
		DX11::SetViewport(clientRect);
	}
}

void GraphicsEngine::InputRenderMode()
{
#ifdef _DEBUG
	if (Utility::InputHandler::GetKeyDown(VK_F6))
	{
		myLuminanceSwitch = !myLuminanceSwitch;
	}

	/*UINT currentRenderMode = static_cast<UINT>(myRenderMode);

	if (Utility::InputHandler::GetKeyDown(VK_F6))
	{
		if (currentRenderMode == 0)
		{
			currentRenderMode = static_cast<UINT>(RenderMode::COUNT) - 1;
		}
		else
		{
			currentRenderMode--;
		}

		SetRenderMode(static_cast<RenderMode>(currentRenderMode));
		DebugLogger::Message("Render Mode set to: " + RenderModeToString(myRenderMode));
	}
	else if (Utility::InputHandler::GetKeyDown(VK_F7))
	{
		currentRenderMode++;

		if (currentRenderMode == static_cast<UINT>(RenderMode::COUNT))
		{
			currentRenderMode = 0;
		}

		SetRenderMode(static_cast<RenderMode>(currentRenderMode));
		DebugLogger::Message("Render Mode set to: " + RenderModeToString(myRenderMode));
	}*/
#endif
}

void GraphicsEngine::AcceptFiles(HWND aHwnd)
{
	if (!myDragAccept)
	{
		myDragAccept = true;
		DragAcceptFiles(aHwnd, true);
	}
}

void GraphicsEngine::HandleDroppedFile(const std::filesystem::path& aPath)
{
	//Copies file to asset folder right now
	if (aPath.extension().string() == ".dds")
	{
		std::string pathToCheck = "Textures/" + aPath.filename().string();

		//Insert a 0
		int count = 0;
		if (std::filesystem::exists(pathToCheck))
		{
			pathToCheck.insert(pathToCheck.find_last_of("."), std::to_string(count));
		}

		while (std::filesystem::exists(pathToCheck))
		{
			count++;
			pathToCheck.replace(pathToCheck.find_last_of(".") - 1, 1, std::to_string(count));
		}

		std::filesystem::copy(aPath, pathToCheck);
	}
	else
	{
		FORMATWARNING("Only .dds texture files supported by drag&drop! Drag&Drop detected: {}", aPath.filename().string());
	}
}

LRESULT CALLBACK GraphicsEngine::WinProc(_In_ HWND hWnd, _In_ UINT uMsg, _In_ WPARAM wParam, _In_ LPARAM lParam)
{
	// We want to be able to access the Graphics Engine instance from inside this function.
	static GraphicsEngine* engine = nullptr;

	if (engine != nullptr)
		engine->AcceptFiles(hWnd);

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
		engine = static_cast<GraphicsEngine*>(createdStruct->lpCreateParams);
	}
	else if (uMsg == WM_SIZE)
	{
		//ResizeWindow(hWnd, LOWORD(lParam), HIWORD(lParam));
		//return true;
	}
	else if (uMsg == WM_DROPFILES)
	{
		HDROP hDrop = (HDROP)wParam;
		TCHAR pathChars[MAX_PATH];
		DragQueryFile(hDrop, 0, pathChars, MAX_PATH);
		std::filesystem::path path(pathChars);
		engine->HandleDroppedFile(path);
		DragFinish(hDrop);
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

void GraphicsEngine::SetBlendState(BlendState aBlendState)
{
	DX11::Context->OMSetBlendState(myBlendStates[static_cast<UINT>(aBlendState)].Get(), nullptr, 0xffffffff);
}

void GraphicsEngine::SetDepthStencilState(DepthStencilState aDepthStencilState)
{
	DX11::Context->OMSetDepthStencilState(myDepthStencilStates[static_cast<UINT>(aDepthStencilState)].Get(), 0);
}

void GraphicsEngine::SetSamplerState(SamplerState aSamplerState, unsigned aSlot)
{
	DX11::Context->PSSetSamplers(aSlot, 1, mySamplerStates[static_cast<UINT>(aSamplerState)].GetAddressOf());
}

void GraphicsEngine::SetRenderMode(RenderMode aRenderMode)
{
	myRenderMode = aRenderMode;
}

RenderMode GraphicsEngine::GetRenderMode() const
{
	return myRenderMode;
}

void GraphicsEngine::SetupBlendStates()
{
	//Setup Alpha Blend
	D3D11_BLEND_DESC alphaBlendDesc = {};
	alphaBlendDesc.RenderTarget[0].BlendEnable = true;
	alphaBlendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	alphaBlendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	alphaBlendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	alphaBlendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	alphaBlendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;
	alphaBlendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_MAX;
	alphaBlendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	AssertIfFailed(DX11::Device->CreateBlendState(&alphaBlendDesc, myBlendStates[static_cast<unsigned int>(BlendState::AlphaBlend)].GetAddressOf()));
	//

	//Setup Additive Blend
	D3D11_BLEND_DESC additiveBlendDesc{};
	additiveBlendDesc.RenderTarget[0].BlendEnable = true;
	additiveBlendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	additiveBlendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
	additiveBlendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	additiveBlendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	additiveBlendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;
	additiveBlendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_MAX;
	additiveBlendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	AssertIfFailed(DX11::Device->CreateBlendState(&additiveBlendDesc, myBlendStates[static_cast<unsigned int>(BlendState::Additive)].GetAddressOf()));
	//

	//Setup BlendState None
	myBlendStates[static_cast<UINT>(BlendState::None)] = nullptr;
	//

	DEBUGLOG("Blend States setup successfully");
}

void GraphicsEngine::SetupDepthStencilStates()
{
	D3D11_DEPTH_STENCIL_DESC readOnlyDepthDesc{};
	readOnlyDepthDesc.DepthEnable = true;
	readOnlyDepthDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	readOnlyDepthDesc.DepthFunc = D3D11_COMPARISON_LESS;
	readOnlyDepthDesc.StencilEnable = false;
	AssertIfFailed(DX11::Device->CreateDepthStencilState(&readOnlyDepthDesc, &myDepthStencilStates[static_cast<UINT>(DepthStencilState::ReadOnly)]));

	D3D11_DEPTH_STENCIL_DESC depthOffDesc{};
	depthOffDesc.DepthEnable = false;
	depthOffDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	depthOffDesc.DepthFunc = D3D11_COMPARISON_LESS;
	depthOffDesc.StencilEnable = false;
	AssertIfFailed(DX11::Device->CreateDepthStencilState(&depthOffDesc, &myDepthStencilStates[static_cast<UINT>(DepthStencilState::Off)]));

	myDepthStencilStates[static_cast<UINT>(DepthStencilState::ReadWrite)] = nullptr;

	DEBUGLOG("Depth Stencil States setup successfully");
}

void GraphicsEngine::SetupSamplerStates()
{
	//Default
	D3D11_SAMPLER_DESC defaultSampleDesc{};
	defaultSampleDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	defaultSampleDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	defaultSampleDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	defaultSampleDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	defaultSampleDesc.MipLODBias = 0.0f;
	defaultSampleDesc.MaxAnisotropy = 1;
	defaultSampleDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	defaultSampleDesc.BorderColor[0] = 1.0f;
	defaultSampleDesc.BorderColor[1] = 1.0f;
	defaultSampleDesc.BorderColor[2] = 1.0f;
	defaultSampleDesc.BorderColor[3] = 1.0f;
	defaultSampleDesc.MinLOD = -D3D11_FLOAT32_MAX;
	defaultSampleDesc.MaxLOD = D3D11_FLOAT32_MAX;
	AssertIfFailed(DX11::Device->CreateSamplerState(&defaultSampleDesc, &mySamplerStates[static_cast<UINT>(SamplerState::Default)]));
	//

	//Dunno if works: Wrap
	/*D3D11_SAMPLER_DESC wrapSampleDesc{};
	wrapSampleDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	wrapSampleDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	wrapSampleDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	wrapSampleDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	wrapSampleDesc.MipLODBias = 0.0f;
	wrapSampleDesc.MaxAnisotropy = 1;
	wrapSampleDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	wrapSampleDesc.BorderColor[0] = 1.0f;
	wrapSampleDesc.BorderColor[1] = 1.0f;
	wrapSampleDesc.BorderColor[2] = 1.0f;
	wrapSampleDesc.BorderColor[3] = 1.0f;
	wrapSampleDesc.MinLOD = -D3D11_FLOAT32_MAX;
	wrapSampleDesc.MaxLOD = -D3D11_FLOAT32_MAX;
	AssertIfFailed(DX11::Device->CreateSamplerState(&wrapSampleDesc, &mySamplerStates[static_cast<UINT>(SamplerState::Wrap)]));*/
	//

	//Point Clamp
	D3D11_SAMPLER_DESC pointSampleDesc{};
	pointSampleDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
	pointSampleDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	pointSampleDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	pointSampleDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	pointSampleDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	pointSampleDesc.MinLOD = -FLT_MAX;
	pointSampleDesc.MaxLOD = FLT_MAX;
	AssertIfFailed(DX11::Device->CreateSamplerState(&pointSampleDesc, &mySamplerStates[static_cast<UINT>(SamplerState::PointClamp)]));
	//

	//Point Wrap
	pointSampleDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	pointSampleDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	pointSampleDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	AssertIfFailed(DX11::Device->CreateSamplerState(&pointSampleDesc, &mySamplerStates[static_cast<UINT>(SamplerState::PointWrap)]));
	//

	DEBUGLOG("Sampler States Setup Successfully");
}

void GraphicsEngine::ResetStates()
{
	SetBlendState(BlendState::None);
	SetDepthStencilState(DepthStencilState::ReadWrite);
	SetSamplerState(SamplerState::Default, static_cast<unsigned>(SamplerState::Default));
	SetSamplerState(SamplerState::PointClamp, static_cast<unsigned>(SamplerState::PointClamp));
	SetSamplerState(SamplerState::PointWrap, static_cast<unsigned>(SamplerState::PointWrap));
}

void GraphicsEngine::ResetViewport()
{
	RECT clientRect;
	GetClientRect(myWindowHandle, &clientRect);
	DX11::SetViewport(clientRect);
}

void GraphicsEngine::CameraControls(std::shared_ptr<Camera> aCamera)
{
	float cameraSpeed = myCameraSpeed;

	if (InputHandler::GetKeyHeld(VK_SHIFT))
	{
		cameraSpeed *= 4.0f;
	}

	const float step = Timer::GetDeltaTime() * cameraSpeed;

	if (InputHandler::GetKeyHeld('W'))
	{
		aCamera->AddPosition(aCamera->GetTransform().GetForward() * step);
	}
	if (InputHandler::GetKeyHeld('A'))
	{
		aCamera->AddPosition(aCamera->GetTransform().GetLeft() * step);
	}
	if (InputHandler::GetKeyHeld('S'))
	{
		aCamera->AddPosition(aCamera->GetTransform().GetBackward() * step);
	}
	if (InputHandler::GetKeyHeld('D'))
	{
		aCamera->AddPosition(aCamera->GetTransform().GetRight() * step);
	}

	if (InputHandler::GetKeyHeld(VK_CONTROL))
	{
		aCamera->AddPosition(Vector3f(0, -1, 0) * step);
	}
	if (InputHandler::GetKeyHeld(VK_SPACE))
	{
		aCamera->AddPosition(Vector3f(0, 1, 0) * step);
	}

	const int scrollDelta = InputHandler::GetScrollWheelDelta();

	//Increase speed on scroll up
	if (scrollDelta > 0)
	{
		myCameraSpeed += Timer::GetDeltaTime() * 1000;
	}
	//Decrease on scroll down
	else if (scrollDelta < 0)
	{
		myCameraSpeed -= Timer::GetDeltaTime() * 1000;
	}

	if (InputHandler::GetRightClickHeld())
	{
		aCamera->SetRotation(Vector3f(
			aCamera->GetTransform().GetRotation().x + static_cast<float>(InputHandler::GetMouseDelta().y) / 5, 
			aCamera->GetTransform().GetRotation().y + static_cast<float>(InputHandler::GetMouseDelta().x) / 5, 0));
	}
}

std::string GraphicsEngine::RenderModeToString(RenderMode aRenderMode)
{
	switch (aRenderMode)
	{
	case RenderMode::Default: return "Default";
	case RenderMode::UV: return "UV";
	case RenderMode::VertexColor: return "Vertex Color";
	case RenderMode::VertexNormal: return "Vertex Normal";
	case RenderMode::PixelNormal: return "Pixel Normal";
	case RenderMode::AlbedoMap: return "Albedo Texture";
	case RenderMode::NormalMap: return "Normal Map";
	case RenderMode::DiffuseLight: return "Diffuse Light & Albedo";
	case RenderMode::DiffuseLightNoAlbedo: return "Diffuse Light Only";
	case RenderMode::AmbientLight: return "Ambient Light & Albedo";
	case RenderMode::AmbientLightNoAlbedo: return "Ambient Light Only";
	case RenderMode::AmbientOcclusion: return "Ambient Occlusion";
	case RenderMode::Roughness: return "Roughness";
	case RenderMode::Metalness: return "Metalness";
	case RenderMode::Emissiveness: return "Emissiveness";
	case RenderMode::Linear: return "Linear";
	default: return "Not implemented";
	}
}

void GraphicsEngine::BeginFrame()
{
	Timer::Update();

	myIntermediateTargetA->Clear(myClearColor);
	myIntermediateTargetB->Clear(myClearColor);
	myHalfSizeTarget->Clear(myClearColor);
	myQuarterSizeTarget->Clear(myClearColor);
	myBlurTargetA->Clear(myClearColor);
	myBlurTargetB->Clear(myClearColor);

	ResetViewport();

	DX11::BeginFrame(myClearColor);
}

void GraphicsEngine::RenderFrame()
{
	auto& camera = myScene.GetCamera();
	auto& models = myScene.GetModels();
	auto& particleSystems = myScene.GetParticleSystems();

	InputRenderMode();

	//Update position of Directional light to help with shadow map resolution
	myScene.GetDirectionalLight()->Update(camera->GetTransform().GetPosition());

	CameraControls(camera);

	float rotationPerSec = 30.0f;

	for (auto& model : models) 
	{
		if (model->GetShouldSpin())
		{
			model->AddRotation(0.f, rotationPerSec * Timer::GetDeltaTime(), 0.f);
		}
		model->Update(myLerpAnimations);
	}

	for (auto& particleSystem : particleSystems)
	{
		particleSystem->Update();
	}

	std::filesystem::path path;
	myEditor.UpdateEditorInterface(myClearColor, myLerpAnimations, path);

	Hierarchy::Update();

	if (path != "")
	{
		Material mat;
		mat.SetAlbedoTexture(Texture::Load(path));
		mat.SetNormalMap(Texture::Load("Textures/T_SpiderCat_N.DDS"));
		mat.SetSurfaceTexture(Texture::Load("Textures/T_SpiderCat_M.DDS"));
		myScene.GetModels()[0]->SetMaterial(std::make_shared<Material>(mat));
	}

	//LEGENDARY TRANSPARENCY MODE
	/*SetBlendState(BlendState::Additive);
	SetDepthStencilState(DepthStencilState::ReadOnly);
	myForwardRenderer.RenderParticles(camera, myScene.GetParticleSystems(), myRenderMode);
	myForwardRenderer.RenderModels(camera, models, myScene.GetDirectionalLight(), myScene.GetAmbientLight(), myRenderMode);*/

	//Render everything with Forward Renderer
	/*SetBlendState(BlendState::None);
	SetDepthStencilState(DepthStencilState::ReadWrite);
	myForwardRenderer.RenderModels(camera, models, myScene.GetDirectionalLight(), myScene.GetAmbientLight(), myScene.GetLights(), myRenderMode);

	SetBlendState(BlendState::Additive);
	SetDepthStencilState(DepthStencilState::ReadOnly);
	myForwardRenderer.RenderParticles(camera, myScene.GetParticleSystems(), myRenderMode);*/
	//

	//Render Models with deferred but particles with forward
	ResetStates();

	if (myScene.GetDirectionalLight()->CastShadows())
	{
		myScene.GetDirectionalLight()->ClearShadowMap();
		myScene.GetDirectionalLight()->SetShadowMapAsTarget();
		myShadowRenderer.RenderShadowPassPerLight(myScene.GetDirectionalLight(), models);
		ResetViewport();
	}

	for (auto& light : myScene.GetLights())
	{
		if (light->CastShadows())
		{
			if (light->GetLightType() == LightType::SpotLight)
			{
				light->ClearShadowMap();
				light->SetShadowMapAsTarget();
				myShadowRenderer.RenderShadowPassPerLight(light, models);
			}
			else if (light->GetLightType() == LightType::PointLight)
			{
				const std::shared_ptr<PointLight> pointLight = reinterpret_cast<const std::shared_ptr<PointLight>&>(light);
				pointLight->ResetAllViewMatrix();

				for (int i = 0; i < 6; ++i)
				{
					light->ClearShadowMap(i);
					pointLight->SetShadowMapAsTarget(i);
					pointLight->SetViewMatrix(i, 0);
					myShadowRenderer.RenderShadowPassPerLight(pointLight, models);
				}
				pointLight->ResetAllViewMatrix();
			}

			ResetViewport();
		}
	}

	myGBuffer->Clear();
	myGBuffer->SetAsRenderTarget();

	myScene.GetDirectionalLight()->BindShadowMapAsResource(10);

	for (auto& light : myScene.GetLights())
	{
		switch (light->GetLightType())
		{
			case LightType::SpotLight:
			{
				light->BindShadowMapAsResource(20);
				break;
			}
			case LightType::PointLight:
			{
				light->BindShadowMapAsResource(30);
				break;
			}
			default: break;
		}
	}

	//Deferred Pass
	myDeferredRenderer.GenerateGBuffer(camera, models);
	//

	myGBuffer->ClearRenderTarget();
	myGBuffer->SetAsResource(0);

	//SSAO Pass
	SetBlendState(BlendState::None);
	SetDepthStencilState(DepthStencilState::Off);
	mySSAOTarget->SetAsRenderTarget();
	myBlueNoise->SetAsResource(8);
	myPostProcessRenderer.Render(PostProcessPass::SSAO, camera);
	mySSAOTarget->RemoveRenderTarget();
	//

	mySSAOTarget->SetAsResource(8);

	//DX11::Context->OMSetRenderTargets(1, DX11::BackBuffer.GetAddressOf(), DX11::DepthBuffer.Get());
	myIntermediateTargetA->SetAsRenderTarget();

	//Draw deferred pass to screen space triangle
	SetBlendState(BlendState::None);
	SetDepthStencilState(DepthStencilState::Off);
	myDeferredRenderer.Render(camera, myScene.GetDirectionalLight(), myScene.GetAmbientLight(), myScene.GetLights(), myRenderMode);
	//

	mySSAOTarget->RemoveResource(8);

	myScene.GetDirectionalLight()->RemoveShadowMapResource(10);

	for (auto& light : myScene.GetLights())
	{
		switch (light->GetLightType())
		{
			case LightType::SpotLight:
			{
				light->RemoveShadowMapResource(20);
				break;
			}
			case LightType::PointLight:
			{
				light->RemoveShadowMapResource(30);
				break;
			}
			default: break;
		}
	}

	myGBuffer->RemoveResource(0);

	//Particle pass
	SetBlendState(BlendState::Additive);
	SetDepthStencilState(DepthStencilState::ReadOnly);
	myForwardRenderer.RenderParticles(camera, myScene.GetParticleSystems(), myRenderMode);
	//

	SetBlendState(BlendState::None);
	SetDepthStencilState(DepthStencilState::Off);

	if (myLuminanceSwitch)
	{
		ResetViewport();
		DX11::Context->OMSetRenderTargets(1, DX11::BackBuffer.GetAddressOf(), DX11::DepthBuffer.Get());
		myIntermediateTargetA->SetAsResource(0);
		myPostProcessRenderer.Render(PostProcessPass::Luminance, camera);
	}
	else
	{
		DX11::Context->OMSetRenderTargets(1, DX11::BackBuffer.GetAddressOf(), nullptr);
		myIntermediateTargetB->SetAsRenderTarget();
		myIntermediateTargetA->SetAsResource(0);
		myPostProcessRenderer.Render(PostProcessPass::Luminance, camera);

		myHalfSizeTarget->SetAsRenderTarget();
		myIntermediateTargetB->SetAsResource(0);
		myPostProcessRenderer.Render(PostProcessPass::Copy, camera);

		myQuarterSizeTarget->SetAsRenderTarget();
		myHalfSizeTarget->SetAsResource(0);
		myPostProcessRenderer.Render(PostProcessPass::Copy, camera);

		myBlurTargetA->SetAsRenderTarget();
		myQuarterSizeTarget->SetAsResource(0);
		myPostProcessRenderer.Render(PostProcessPass::Gaussian, camera);

		myBlurTargetB->SetAsRenderTarget();
		myBlurTargetA->SetAsResource(0);
		myPostProcessRenderer.Render(PostProcessPass::Gaussian, camera);

		myQuarterSizeTarget->SetAsRenderTarget();
		myBlurTargetB->SetAsResource(0);
		myPostProcessRenderer.Render(PostProcessPass::Copy, camera);

		myHalfSizeTarget->SetAsRenderTarget();
		myQuarterSizeTarget->SetAsResource(0);
		myPostProcessRenderer.Render(PostProcessPass::Copy, camera);

		//Render to backbuffer
		ResetViewport();
		DX11::Context->OMSetRenderTargets(1, DX11::BackBuffer.GetAddressOf(), DX11::DepthBuffer.Get());

		myIntermediateTargetA->SetAsResource(0);
		myHalfSizeTarget->SetAsResource(1);
		myPostProcessRenderer.Render(PostProcessPass::Bloom, camera);

		myIntermediateTargetA->RemoveResource(0);
		myHalfSizeTarget->RemoveResource(1);
	}
	//

	InputHandler::UpdatePreviousState();
}

void GraphicsEngine::EndFrame()
{
	DX11::EndFrame();
}