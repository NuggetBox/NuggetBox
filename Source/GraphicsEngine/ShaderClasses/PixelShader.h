#pragma once
#include <d3d11.h>
#include <filesystem>
#include <unordered_map>

#include "Rendering/DX11.h"

class PixelShader
{
public:
	PixelShader() = default;

	void Bind() const;
	static std::shared_ptr<PixelShader> Load(const std::filesystem::path& aPath);

private:
	static inline std::unordered_map<std::string, std::shared_ptr<PixelShader>> ourPixelShaderRegistry;

	ComPtr<ID3D11PixelShader> myPixelShader;
};