#pragma once
#include "Camera.h"
#include "RenderStructs.h"
#include "ShaderClasses/VertexShader.h"
#include "ShaderClasses/PixelShader.h"

enum class PostProcessPass
{
	Copy,
	Luminance,
	Gaussian,
	Bloom,
	//SSAO,
	COUNT
};

class PostProcessRenderer
{
public:
	PostProcessRenderer() = default;
	void Initialize();
	void Render(PostProcessPass aPass, std::shared_ptr<Camera>& aCamera);

private:
	std::shared_ptr<VertexShader> myFullscreenVS;

	std::array<std::shared_ptr<PixelShader>, static_cast<size_t>(PostProcessPass::COUNT)> myPassShaders;

	FrameBufferData myFrameBufferData;
	ComPtr<ID3D11Buffer> myFrameBuffer;
};