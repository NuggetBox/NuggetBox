#include "NuggetBox.pch.h"
#include "GeometryShader.h"

#include "Core/DebugLogger.h"

void GeometryShader::Bind() const
{
	if (myGeometryShader != nullptr)
	{
		DX11::Context->GSSetShader(myGeometryShader.Get(), nullptr, 0);
	}
	else
	{
		DEBUGERROR("Geometry shader " + myName + " has not been loaded but is trying to be used as a resource");
	}
}

std::shared_ptr<GeometryShader> GeometryShader::Load(const std::filesystem::path& aPath)
{
	if (ourGeometryShaderRegistry.contains(aPath.string()))
	{
		DEBUGLOG("Loaded Geometry Shader " + aPath.filename().string() + " from registry successfully");
		return ourGeometryShaderRegistry.at(aPath.string());
	}

	assert(std::filesystem::exists(aPath));

	std::ifstream gsFile;
	gsFile.open(aPath.string(), std::ios::binary);
	const std::string gsData = { std::istreambuf_iterator<char>(gsFile), std::istreambuf_iterator<char>() };
	gsFile.close();

	ComPtr<ID3D11GeometryShader> geometryShader;
	AssertIfFailed(DX11::Device->CreateGeometryShader(gsData.data(), gsData.size(), nullptr, geometryShader.GetAddressOf()));

	std::string name = aPath.filename().string();
	DEBUGLOG("Loaded Geometry Shader " + name);

	std::shared_ptr<GeometryShader> loadedGeometryShader = std::make_shared<GeometryShader>();
	loadedGeometryShader->myName = name;
	loadedGeometryShader->myGeometryShader = geometryShader;
	ourGeometryShaderRegistry.insert(std::pair(aPath.string(), loadedGeometryShader));
	return loadedGeometryShader;
}
