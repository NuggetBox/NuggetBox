#pragma once
#include <array>
#include <wrl.h>
#include <cassert>
#include <Math/Vector4.hpp>

#include "DebugLogger.h"

using namespace Microsoft::WRL;

struct ID3D11Device;
struct ID3D11DeviceContext;
struct IDXGISwapChain;
struct ID3D11SamplerState;
struct ID3D11RenderTargetView;
struct ID3D11DepthStencilView;

#define AssertIfFailed(x) {HRESULT result = (x); if (result != S_OK) DEBUGERROR("Error HRESULT recieved: " + DX11::HResultToString(result)); assert((result) == S_OK);}

class DX11
{
public:
	DX11() = delete;
	~DX11() = delete;

	static void Initialize(HWND aWindowHandle, bool aEnableDeviceDebug);
	static void BeginFrame(Utility::Vector4<float> aClearColor = {0, 0, 0, 0});
	static void EndFrame();

	static void CreateSwapChain(HWND aWindowHandle, bool aEnableDeviceDebug);
	static void CreateDepthBuffer(RECT aClientRect);
	static void SetViewport(RECT aClientRect);
	static void CreateSamplerState();

	static std::string HResultToString(HRESULT aResult);

	static ComPtr<ID3D11Device> Device;
	static ComPtr<ID3D11DeviceContext> Context;
	static ComPtr<IDXGISwapChain> SwapChain;

	static ComPtr<ID3D11SamplerState> SampleStateDefault;
	static ComPtr<ID3D11SamplerState> SampleStateWrap;

	static ComPtr<ID3D11RenderTargetView> BackBuffer;
	static ComPtr<ID3D11DepthStencilView> DepthBuffer;
};