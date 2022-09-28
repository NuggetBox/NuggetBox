#include "NuggetBox.pch.h"
#include "PostProcessRenderer.h"

void PostProcessRenderer::Initialize()
{
	std::array<std::string, static_cast<size_t>(PostProcessPass::COUNT)> shaderPaths;
	shaderPaths[static_cast<size_t>(PostProcessPass::Copy)] = "PP_Copy.cso";
	shaderPaths[static_cast<size_t>(PostProcessPass::Luminance)] = "PP_Luminance.cso";
	shaderPaths[static_cast<size_t>(PostProcessPass::Gaussian)] = "PP_Gaussian.cso";
	shaderPaths[static_cast<size_t>(PostProcessPass::Bloom)] = "PP_Bloom.cso";

	for (int i = 0; i < static_cast<int>(PostProcessPass::COUNT); ++i)
	{
		myPassShaders[i] = PixelShader::Load(shaderPaths[i]);
	}
}

void PostProcessRenderer::Render(PostProcessPass aPass)
{
	DX11::Context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	DX11::Context->IASetInputLayout(nullptr);
	DX11::Context->IASetVertexBuffers(0, 0, nullptr, nullptr, nullptr);
	DX11::Context->IASetIndexBuffer(nullptr, DXGI_FORMAT_UNKNOWN, 0);
	myFullscreenVS->Bind();
	myPassShaders[static_cast<size_t>(aPass)]->Bind();
	DX11::Context->GSSetShader(nullptr, nullptr, 0);
	DX11::Context->Draw(3, 0);
}