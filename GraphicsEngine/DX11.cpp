#include "NuggetBox.pch.h"
#include <d3d11.h>

#include "DX11.h"

ComPtr<ID3D11Device> DX11::Device;
ComPtr<ID3D11DeviceContext> DX11::Context;
ComPtr<IDXGISwapChain> DX11::SwapChain;

ComPtr<ID3D11SamplerState> DX11::SampleStateDefault;
ComPtr<ID3D11SamplerState> DX11::SampleStateWrap;

ComPtr<ID3D11RenderTargetView> DX11::BackBuffer;
ComPtr<ID3D11DepthStencilView> DX11::DepthBuffer;

void DX11::Initialize(HWND aWindowHandle, bool aEnableDeviceDebug)
{
	DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
	swapChainDesc.BufferCount = 1;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; // 4 färgkanaler RGBA, unsigned, normalized
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.OutputWindow = aWindowHandle;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.Windowed = true;

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

	ComPtr<ID3D11Texture2D> backBufferTexture;
	AssertIfFailed(SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)(backBufferTexture.GetAddressOf())))

	AssertIfFailed(Device->CreateRenderTargetView(backBufferTexture.Get(), nullptr, BackBuffer.GetAddressOf()))

	AssertIfFailed(backBufferTexture->Release())

	RECT clientRect = { 0, 0, 0, 0 };
	GetClientRect(aWindowHandle, &clientRect);

	ComPtr<ID3D11Texture2D> depthBufferTexture;
	D3D11_TEXTURE2D_DESC depthBufferDesc = { 0 };
	depthBufferDesc.Width = clientRect.right - clientRect.left;
	depthBufferDesc.Height = clientRect.bottom - clientRect.top;
	depthBufferDesc.ArraySize = 1;
	depthBufferDesc.Format = DXGI_FORMAT_D32_FLOAT;
	depthBufferDesc.SampleDesc.Count = 1;
	depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;

	AssertIfFailed(Device->CreateTexture2D(&depthBufferDesc, nullptr, depthBufferTexture.GetAddressOf()))

	AssertIfFailed(Device->CreateDepthStencilView(depthBufferTexture.Get(), nullptr, DepthBuffer.GetAddressOf()))

	Context->OMSetRenderTargets(1, BackBuffer.GetAddressOf(), DepthBuffer.Get());

	D3D11_VIEWPORT viewport;
	viewport.TopLeftX = 0.0f;
	viewport.TopLeftY = 0.0f;
	viewport.Width = static_cast<float>(clientRect.right - clientRect.left);
	viewport.Height = static_cast<float>(clientRect.bottom - clientRect.top);
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;

	Context->RSSetViewports(1, &viewport);

	D3D11_SAMPLER_DESC samplerDesc;
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MaxAnisotropy = 1;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	samplerDesc.BorderColor[0] = 1.0f;
	samplerDesc.BorderColor[1] = 1.0f;
	samplerDesc.BorderColor[2] = 1.0f;
	samplerDesc.BorderColor[3] = 1.0f;
	samplerDesc.MinLOD = -D3D11_FLOAT32_MAX;
	samplerDesc.MaxLOD = -D3D11_FLOAT32_MAX;

	AssertIfFailed(Device->CreateSamplerState(&samplerDesc, SampleStateDefault.GetAddressOf()))

	//Seed random
	srand(static_cast<unsigned>(time(0)));
}

void DX11::BeginFrame(std::array<float, 4> aClearColor)
{
	Context->ClearRenderTargetView(BackBuffer.Get(), &aClearColor[0]);
	Context->ClearDepthStencilView(DepthBuffer.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}

void DX11::EndFrame()
{
	SwapChain->Present(1, 0);
}