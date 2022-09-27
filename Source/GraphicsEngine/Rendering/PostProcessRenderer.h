#pragma once
#include "ShaderClasses/VertexShader.h"
#include "ShaderClasses/PixelShader.h"

enum class PostProcessPass
{
	Copy,
	Luminance,
	Gaussian,
	Bloom,
	COUNT
};

class PostProcessRenderer
{
public:
	void Initialize();
	void Render(PostProcessPass aPass);

private:
	std::shared_ptr<VertexShader> myFullscreenVS;

	std::array<std::shared_ptr<PixelShader>, static_cast<size_t>(PostProcessPass::COUNT)> myPassShaders;
};