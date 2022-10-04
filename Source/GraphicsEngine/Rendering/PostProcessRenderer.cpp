#include "NuggetBox.pch.h"
#include "PostProcessRenderer.h"

#include <Timer.h>

#include "Camera.h"

void PostProcessRenderer::Initialize()
{
	myFullscreenVS = VertexShader::Load("Shaders/Fullscreen_VS.cso");

	std::array<std::string, static_cast<size_t>(PostProcessPass::COUNT)> shaderPaths;
	shaderPaths[static_cast<size_t>(PostProcessPass::Copy)] = "Shaders/PP_Copy_PS.cso";
	shaderPaths[static_cast<size_t>(PostProcessPass::Luminance)] = "Shaders/PP_Luminance_PS.cso";
	shaderPaths[static_cast<size_t>(PostProcessPass::Gaussian)] = "Shaders/PP_Gaussian_PS.cso";
	shaderPaths[static_cast<size_t>(PostProcessPass::Bloom)] = "Shaders/PP_Bloom_PS.cso";
	shaderPaths[static_cast<size_t>(PostProcessPass::SSAO)] = "Shaders/PP_SSAO_PS.cso";

	for (int i = 0; i < static_cast<int>(PostProcessPass::COUNT); ++i)
	{
		myPassShaders[i] = PixelShader::Load(shaderPaths[i]);
	}

	D3D11_BUFFER_DESC bufferDescription = { 0 };
	bufferDescription.Usage = D3D11_USAGE_DYNAMIC;
	bufferDescription.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDescription.CPUAccessFlags = D3D10_CPU_ACCESS_WRITE;

	bufferDescription.ByteWidth = sizeof(FrameBufferData);
	AssertIfFailed(DX11::Device->CreateBuffer(&bufferDescription, nullptr, myFrameBuffer.GetAddressOf()));
}

void PostProcessRenderer::Render(PostProcessPass aPass, std::shared_ptr<Camera>& aCamera)
{
	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	DX11::SwapChain->GetDesc(&swapChainDesc);
	const HWND handle = swapChainDesc.OutputWindow;
	RECT clientRect;
	GetClientRect(handle, &clientRect);
	Vector2<unsigned> resolution = { static_cast<unsigned>(clientRect.right - clientRect.left), static_cast<unsigned>(clientRect.bottom - clientRect.top) };

	myFrameBufferData.View = Matrix4f::GetFastInverse(aCamera->GetTransform().GetMatrix());
	myFrameBufferData.CamTranslation = aCamera->GetTransform().GetPosition();
	myFrameBufferData.Projection = aCamera->GetProjectionMatrix();
	myFrameBufferData.NearPlane = aCamera->GetNearPlane();
	myFrameBufferData.FarPlane = aCamera->GetFarPlane();
	myFrameBufferData.Resolution = resolution;
	myFrameBufferData.RenderMode = 0;
	myFrameBufferData.DeltaTime = Timer::GetDeltaTime();
	myFrameBufferData.TotalTime = Timer::GetTotalTime();

	const float aspectRatio = static_cast<float>(myFrameBufferData.Resolution.x) / static_cast<float>(myFrameBufferData.Resolution.y);
	const float halfHeight = myFrameBufferData.FarPlane * tanf(0.5f * aCamera->GetFov());
	const float halfWidth = aspectRatio * halfHeight;

	myFrameBufferData.FrustrumCorners[0] = Vector4f(-halfWidth, -halfHeight, myFrameBufferData.FarPlane, 0.0f);
	myFrameBufferData.FrustrumCorners[1] = Vector4f(-halfWidth, +halfHeight, myFrameBufferData.FarPlane, 0.0f);
	myFrameBufferData.FrustrumCorners[2] = Vector4f(+halfWidth, +halfHeight, myFrameBufferData.FarPlane, 0.0f);
	myFrameBufferData.FrustrumCorners[3] = Vector4f(+halfWidth, -halfHeight, myFrameBufferData.FarPlane, 0.0f);

	D3D11_MAPPED_SUBRESOURCE bufferData;

	ZeroMemory(&bufferData, sizeof(D3D11_MAPPED_SUBRESOURCE));
	AssertIfFailed(DX11::Context->Map(myFrameBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &bufferData));
	memcpy_s(bufferData.pData, sizeof(FrameBufferData), &myFrameBufferData, sizeof(FrameBufferData));
	DX11::Context->Unmap(myFrameBuffer.Get(), 0);

	DX11::Context->PSSetConstantBuffers(0, 1, myFrameBuffer.GetAddressOf());

	DX11::Context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	DX11::Context->IASetInputLayout(nullptr);
	DX11::Context->IASetVertexBuffers(0, 0, nullptr, nullptr, nullptr);
	DX11::Context->IASetIndexBuffer(nullptr, DXGI_FORMAT_UNKNOWN, 0);
	myFullscreenVS->Bind();
	myPassShaders[static_cast<size_t>(aPass)]->Bind();
	DX11::Context->GSSetShader(nullptr, nullptr, 0);
	DX11::Context->Draw(3, 0);
}