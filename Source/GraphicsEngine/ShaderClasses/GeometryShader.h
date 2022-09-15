#pragma once
#include <d3d11.h>
#include <filesystem>
#include <unordered_map>

#include "Rendering/DX11.h"

class GeometryShader
{
public:
	GeometryShader() = default;

	void SetRenderTarget() const;
	static std::shared_ptr<GeometryShader> Load(const std::filesystem::path& aPath);

private:
	static inline std::unordered_map<std::string, std::shared_ptr<GeometryShader>> ourGeometryShaderRegistry;

	ComPtr<ID3D11GeometryShader> myGeometryShader;

	std::string myName;
};