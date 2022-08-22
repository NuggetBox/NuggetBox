#pragma once
#include <array>
#include <wrl.h>
#include <cassert>

using namespace Microsoft::WRL;

struct ID3D11Device;
struct ID3D11DeviceContext;
struct IDXGISwapChain;
struct ID3D11SamplerState;
struct ID3D11RenderTargetView;
struct ID3D11DepthStencilView;

#define AssertIfFailed(x) assert((x) == S_OK)

class DX11
{
public:
	DX11() = delete;
	~DX11() = delete;

	static void Initialize(HWND aWindowHandle, bool aEnableDeviceDebug);
	static void BeginFrame(std::array<float, 4> aClearColor = { 0, 0, 0, 0 });
	static void EndFrame();

	static void CreateSwapChain(HWND aWindowHandle, bool aEnableDeviceDebug);
	static void CreateDepthBuffer(RECT aClientRect);
	static void SetViewport(RECT aClientRect);
	static void CreateSamplerState();

	static ComPtr<ID3D11Device> Device;
	static ComPtr<ID3D11DeviceContext> Context;
	static ComPtr<IDXGISwapChain> SwapChain;

	static ComPtr<ID3D11SamplerState> SampleStateDefault;
	static ComPtr<ID3D11SamplerState> SampleStateWrap;

	static ComPtr<ID3D11RenderTargetView> BackBuffer;
	static ComPtr<ID3D11DepthStencilView> DepthBuffer;
};