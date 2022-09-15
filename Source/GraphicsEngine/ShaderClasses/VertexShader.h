#pragma once
#include <d3d11.h>
#include <filesystem>
#include <unordered_map>

#include "Rendering/DX11.h"

enum class InputLayoutType
{
	MeshVertex,
	ParticleVertex
};

class VertexShader
{
public:
	VertexShader() = default;

	void Bind() const;
	static std::shared_ptr<VertexShader> Load(const std::filesystem::path& aPath, InputLayoutType anInputLayoutType = InputLayoutType::MeshVertex);

	void CreateInputLayout(const std::string& someVertexShaderData, InputLayoutType anInputLayoutType = InputLayoutType::MeshVertex);
	//const ComPtr<ID3D11InputLayout>& GetInputLayout() const;

private:
	static inline std::unordered_map<std::string, std::shared_ptr<VertexShader>> ourVertexShaderRegistry;

	ComPtr<ID3D11VertexShader> myVertexShader;
	ComPtr<ID3D11InputLayout> myInputLayout;

	//Might be useful
	std::string myName;
};