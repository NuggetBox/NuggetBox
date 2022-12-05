#include "NuggetBox.pch.h"
#include "GraphicsEngine.h"

#include <json.hpp>
#include <shellapi.h>
#include <TGAFBXImporter/include/FBXImporter.h>

#include "Camera.h"
#include "Core/DebugLogger.h"
#include "Model/Model.h"
#include "InputHandler.h"
#include "Timer.h"
#include "imgui/imgui.h"

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

	auto first = Model::Load("Cube");
	first->SetPosition(350, 50, 1500.0f);
	myScene.AddModel(first);

	auto second = Model::Load("Cube");
	second->SetPosition(350, 50, 2000.0f);
	myScene.AddModel(second);

	auto third = Model::Load("Cube");
	third->SetPosition(350, 50, 2500.0f);
	myScene.AddModel(third);

	for (int i = 0; i < 100; ++i)
	{
		auto plane = Model::Load("Plane");
		plane->SetPosition(0, 0, 10000.0f * i);
		plane->SetScale({ 100.0f, 100.0f, 100.0f });
		myScene.AddModel(plane);
	}

	std::shared_ptr<Camera> camera = std::make_shared<Camera>();
	camera->SetPosition(0, 100, 0);
	myScene.SetCamera(camera);

	myScene.SetDirectionalLight(DirectionalLight::Create(Utility::Vector3f::One(), 1.0f, Utility::Vector3f(45, -45, 0)));
	myScene.SetAmbientLight(AmbientLight::Create("assets/Textures/skansen_cubemap.dds"));

	myIntermediateTargetA = RenderTarget::Create(clientSize.x, clientSize.y, DXGI_FORMAT_R32G32B32A32_FLOAT);
	myIntermediateTargetB = RenderTarget::Create(clientSize.x, clientSize.y, DXGI_FORMAT_R32G32B32A32_FLOAT);
	myHalfSizeTarget = RenderTarget::Create(clientSize.x / 2, clientSize.y / 2, DXGI_FORMAT_R32G32B32A32_FLOAT);
	myQuarterSizeTarget = RenderTarget::Create(clientSize.x / 4, clientSize.y / 4, DXGI_FORMAT_R32G32B32A32_FLOAT);
	myBlurTargetA = RenderTarget::Create(clientSize.x / 4, clientSize.y / 4, DXGI_FORMAT_R32G32B32A32_FLOAT);
	myBlurTargetB = RenderTarget::Create(clientSize.x / 4, clientSize.y / 4, DXGI_FORMAT_R32G32B32A32_FLOAT);

	mySSAOTarget = RenderTarget::Create(clientSize.x, clientSize.y, DXGI_FORMAT_R32_FLOAT);
	myBlueNoise = Texture::Load("assets/Textures/BlueNoise.dds");

	myForwardRenderer.Initialize();
	myDeferredRenderer.Initialize();
	myShadowRenderer.Initialize();
	myPostProcessRenderer.Initialize();

	myLerpAnimations = true;
	myDragAccept = false;

	myEditor.Initialize(myClearColor, myLerpAnimations);

	myGBuffer = GBuffer::CreateGBuffer(clientRect);

	Utility::Timer::Update();
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
	UINT currentRenderMode = static_cast<UINT>(myRenderMode);

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
	}
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
}

LRESULT CALLBACK GraphicsEngine::WinProc(_In_ HWND hWnd, _In_ UINT uMsg, _In_ WPARAM wParam, _In_ LPARAM lParam)
{
	// We want to be able to access the Graphics Engine instance from inside this function.
	static GraphicsEngine* engine = nullptr;

	if (engine != nullptr)
		engine->AcceptFiles(hWnd);

	if (Utility::InputHandler::UpdateEvents(uMsg, wParam, lParam))
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

void GraphicsEngine::LoadModel(std::filesystem::path path, std::shared_ptr<Model> model)
{
	std::shared_ptr<Model> modelload = Model::Load(path);
	model->myModelData = modelload->myModelData;
	if (path.string() == "Assets/Meshes/pedal.fbx")
	{
		model->SetScale({ 10 });
	}
}

void GraphicsEngine::CameraControls(std::shared_ptr<Camera> aCamera)
{
	if (!Utility::InputHandler::GetRightClickHeld())
	{
		const float step = Utility::Timer::GetDeltaTime() * myCameraSpeed;
		aCamera->AddPosition(aCamera->GetTransform().GetForward() * step);
	}
	else
	{
		float cameraSpeed = myCameraSpeed;

		if (Utility::InputHandler::GetKeyHeld(VK_SHIFT))
		{
			cameraSpeed *= 4.0f;
		}

		const float step = Utility::Timer::GetDeltaTime() * cameraSpeed;

		if (Utility::InputHandler::GetKeyHeld('W'))
		{
			aCamera->AddPosition(aCamera->GetTransform().GetForward() * step);
		}
		if (Utility::InputHandler::GetKeyHeld('A'))
		{
			aCamera->AddPosition(aCamera->GetTransform().GetLeft() * step);
		}
		if (Utility::InputHandler::GetKeyHeld('S'))
		{
			aCamera->AddPosition(aCamera->GetTransform().GetBackward() * step);
		}
		if (Utility::InputHandler::GetKeyHeld('D'))
		{
			aCamera->AddPosition(aCamera->GetTransform().GetRight() * step);
		}

		if (Utility::InputHandler::GetKeyHeld(VK_CONTROL))
		{
			aCamera->AddPosition(Utility::Vector3f(0, -1, 0) * step);
		}
		if (Utility::InputHandler::GetKeyHeld(VK_SPACE))
		{
			aCamera->AddPosition(Utility::Vector3f(0, 1, 0) * step);
		}

		const int scrollDelta = Utility::InputHandler::GetScrollWheelDelta();

		//Increase speed on scroll up
		if (scrollDelta > 0)
		{
			myCameraSpeed += Utility::Timer::GetDeltaTime() * 1000;
		}
		//Decrease on scroll down
		else if (scrollDelta < 0)
		{
			myCameraSpeed -= Utility::Timer::GetDeltaTime() * 1000;
		}

		if (Utility::InputHandler::GetRightClickHeld())
		{
			aCamera->SetRotation(Utility::Vector3f(
				aCamera->GetTransform().GetRotation().x + static_cast<float>(Utility::InputHandler::GetMouseDelta().y) / 5,
				aCamera->GetTransform().GetRotation().y + static_cast<float>(Utility::InputHandler::GetMouseDelta().x) / 5, 0));
		}
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
	Utility::Timer::Update();

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

	if ((models[0]->GetTransform().GetPosition().z - camera->GetTransform().GetPosition().z) < 1000.0f)
	{
		if (!myFirstThreadStarted)
		{
			std::thread firstThread(LoadModel, std::filesystem::path("Assets/Meshes/pedal.fbx"), models[0]);
			firstThread.detach();
			myFirstThreadStarted = true;
		}
	}

	if ((models[1]->GetTransform().GetPosition().z - camera->GetTransform().GetPosition().z) < 1000.0f)
	{
		if (!mySecondThreadStarted)
		{
			std::thread secondThread(LoadModel, std::filesystem::path("assets/meshes/Particle_Chest.fbx"), models[1]);
			secondThread.detach();
			mySecondThreadStarted = true;
		}
	}

	if ((models[2]->GetTransform().GetPosition().z - camera->GetTransform().GetPosition().z) < 1000.0f)
	{
		if (!myThirdThreadStarted)
		{
			std::thread thirdThread(LoadModel, std::filesystem::path("assets/meshes/Buddah_Test_Bakes_low.fbx"), models[2]);
			thirdThread.detach();
			myThirdThreadStarted = true;
		}
	}

	//Update position of Directional light to help with shadow map resolution
	myScene.GetDirectionalLight()->Update(camera->GetTransform().GetPosition());

	CameraControls(camera);

	for (auto& model : models) 
	{
		model->Update(myLerpAnimations);
	}

	for (auto& particleSystem : particleSystems)
	{
		particleSystem->Update();
	}

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

	//Deferred Pass
	myGBuffer->Clear();
	myGBuffer->SetAsRenderTarget();
	myDeferredRenderer.GenerateGBuffer(camera, models, myRenderMode);
	myGBuffer->RemoveRenderTarget();
	//

	myGBuffer->SetAsResource(0);

	//SSAO Pass
	SetBlendState(BlendState::None);
	SetDepthStencilState(DepthStencilState::Off);
	mySSAOTarget->SetAsRenderTarget();
	myBlueNoise->SetAsResource(8);
	myPostProcessRenderer.Render(PostProcessPass::SSAO, camera);
	mySSAOTarget->RemoveRenderTarget();
	myBlueNoise->RemoveAsResource(8);
	//

	//Start rendering to intermediate target to which we apply post processing effects
	myIntermediateTargetA->SetAsRenderTarget();

	//Calculate lighting and draw the previosly drawn deferred pass to a screen space triangle
	//Using SSAO target (which we drew SSAO on) as resource for drawing SSAO.
	//Also Drawing it to IntermediateTargetA so we can use that texture for post processing
	SetBlendState(BlendState::None);
	SetDepthStencilState(DepthStencilState::Off);
	mySSAOTarget->SetAsResource(8);
	myScene.BindLightsAsResources();
	myDeferredRenderer.Render(camera, myScene.GetDirectionalLight(), myScene.GetAmbientLight(), myScene.GetLights(), myRenderMode);
	myGBuffer->RemoveResource(0);
	mySSAOTarget->RemoveResource(8);
	myScene.RemoveLightsAsResources();
	//

	//Particle pass
	SetBlendState(BlendState::Additive);
	SetDepthStencilState(DepthStencilState::ReadOnly);
	myForwardRenderer.RenderParticles(camera, myScene.GetParticleSystems(), myRenderMode);
	//

	SetBlendState(BlendState::None);
	SetDepthStencilState(DepthStencilState::Off);

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

	myIntermediateTargetB->Clear({ 0,0,0,0 });
	myIntermediateTargetB->SetAsRenderTarget();

	myIntermediateTargetA->SetAsResource(0);
	myHalfSizeTarget->SetAsResource(1);
	myPostProcessRenderer.Render(PostProcessPass::Bloom, camera);
	myIntermediateTargetA->RemoveResource(0);
	myHalfSizeTarget->RemoveResource(1);

	//Render to backbuffer
	ResetViewport();
	DX11::Context->OMSetRenderTargets(1, DX11::BackBuffer.GetAddressOf(), DX11::DepthBuffer.Get());

	myIntermediateTargetB->SetAsResource(0);
	myPostProcessRenderer.Render(PostProcessPass::TONEMAP, camera);
	myIntermediateTargetB->RemoveResource(0);
	//

	Utility::InputHandler::UpdatePreviousState();
}

void GraphicsEngine::EndFrame()
{
	DX11::EndFrame();
}