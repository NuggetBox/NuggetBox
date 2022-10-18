#include "NuggetBox.pch.h"
#include <d3d11.h>
#include "DX11.h"

#include "Core/DebugLogger.h"

void DX11::Initialize(HWND aWindowHandle, bool aEnableDeviceDebug)
{
	CreateSwapChain(aWindowHandle, aEnableDeviceDebug);

	ComPtr<ID3D11Texture2D> backBufferTexture;
	AssertIfFailed(SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)(backBufferTexture.GetAddressOf())));

	AssertIfFailed(Device->CreateRenderTargetView(backBufferTexture.Get(), nullptr, BackBuffer.GetAddressOf()));

	RECT clientRect;
	GetClientRect(aWindowHandle, &clientRect);

	CreateDepthBuffer(clientRect);

	Context->OMSetRenderTargets(1, BackBuffer.GetAddressOf(), DepthBuffer.Get());

	SetViewport(clientRect);

	//Seed random
	srand(static_cast<unsigned>(time(0)));
}

void DX11::BeginFrame(Utility::Vector4<float> aClearColor)
{
	Context->ClearRenderTargetView(BackBuffer.Get(), &aClearColor[0]);
	Context->ClearDepthStencilView(DepthBuffer.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	//Daniels windows 8+ change
	Context->OMSetRenderTargets(1, BackBuffer.GetAddressOf(), DepthBuffer.Get());
}

void DX11::EndFrame()
{
	SwapChain->Present(0, DXGI_PRESENT_ALLOW_TEARING);
}

void DX11::CreateSwapChain(HWND aWindowHandle, bool aEnableDeviceDebug)
{
	DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; // 4 färgkanaler RGBA, unsigned, normalized
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.OutputWindow = aWindowHandle;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.Windowed = true;

	//Daniels windows 8+ change
	//swapChainDesc.BufferCount = 1;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING;
	swapChainDesc.BufferCount = 2;

	AssertIfFailed(D3D11CreateDeviceAndSwapChain
	(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		aEnableDeviceDebug ? D3D11_CREATE_DEVICE_DEBUG : 0,
		nullptr,
		0,
		D3D11_SDK_VERSION,
		&swapChainDesc,
		SwapChain.GetAddressOf(),
		Device.GetAddressOf(),
		nullptr,
		&Context
	));

	DEBUGLOG("Created Swap Chain");
}

void DX11::CreateDepthBuffer(RECT aClientRect)
{
	ComPtr<ID3D11Texture2D> depthBufferTexture;
	D3D11_TEXTURE2D_DESC depthBufferDesc{};
	depthBufferDesc.Width = aClientRect.right - aClientRect.left;
	depthBufferDesc.Height = aClientRect.bottom - aClientRect.top;
	depthBufferDesc.ArraySize = 1;
	depthBufferDesc.Format = DXGI_FORMAT_D32_FLOAT;
	depthBufferDesc.SampleDesc.Count = 1;
	depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;

	AssertIfFailed(Device->CreateTexture2D(&depthBufferDesc, nullptr, depthBufferTexture.GetAddressOf()));

	AssertIfFailed(Device->CreateDepthStencilView(depthBufferTexture.Get(), nullptr, DepthBuffer.GetAddressOf()));

	DEBUGLOG("Created Depth Buffer");
}

void DX11::SetViewport(RECT aClientRect)
{
	D3D11_VIEWPORT viewport = {};
	viewport.TopLeftX = 0.0f;
	viewport.TopLeftY = 0.0f;
	viewport.Width = static_cast<float>(aClientRect.right - aClientRect.left);
	viewport.Height = static_cast<float>(aClientRect.bottom - aClientRect.top);
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;

	Context->RSSetViewports(1, &viewport);
}

std::string DX11::HResultToString(HRESULT aResult)
{
	switch (aResult)
	{
	case S_OK: return "Successful"; // Operation successful	0x00000000
	case E_ABORT: return "Aborted"; // Operation aborted 0x80004004
	case E_ACCESSDENIED: return "Access Denied"; // General access denied error	0x80070005
	case E_FAIL: return "Unspecified Failure"; // Unspecified failure 0x80004005
	case E_HANDLE: return "Invalid Handle"; // Handle that is not valid 0x80070006
	case E_INVALIDARG: return "Invalid Args"; // One or more arguments are not valid 0x80070057
	case E_NOINTERFACE: return "Interface not Supported"; //  No such interface supported 0x80004002
	case E_NOTIMPL: return "Not Implemented"; // Not implemented 0x80004001
	case E_OUTOFMEMORY: return "Out of Memory"; //  Failed to allocate necessary memory	0x8007000E
	case E_POINTER:return "Invalid Pointer"; // Pointer that is not valid	0x80004003
	case E_UNEXPECTED: return "Unexpected Failure"; // Unexpected failure 0x8000FFFF
	default: return "Unknown Error Result";
	}
}