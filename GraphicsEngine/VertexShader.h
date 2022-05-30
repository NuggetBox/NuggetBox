#pragma once
#include <d3d11.h>
#include <filesystem>
#include <unordered_map>

#include "DX11.h"

class VertexShader
{
public:
	VertexShader() = default;

	void Bind() const;
	static std::shared_ptr<VertexShader> Load(const std::filesystem::path& aPath);

private:
	static void SetInputLayout(const std::string& someVertexShaderData);

	static std::unordered_map<std::string, std::shared_ptr<VertexShader>> ourVertexShaderRegistry;

	ComPtr<ID3D11VertexShader> myVertexShader;
	//Needed?
	static ComPtr<ID3D11InputLayout> ourInputLayout;
	static bool ourInputLayoutIsSet;
};