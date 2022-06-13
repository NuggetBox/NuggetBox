#include "NuggetBox.pch.h"
#include "PixelShader.h"

#include "DebugLogger.h"

//std::unordered_map<std::string, std::shared_ptr<PixelShader>> PixelShader::ourPixelShaderRegistry;

void PixelShader::Bind() const
{
	DX11::Context->PSSetShader(myPixelShader.Get(), nullptr, 0);
}

std::shared_ptr<PixelShader> PixelShader::Load(const std::filesystem::path& aPath)
{
	if (ourPixelShaderRegistry.contains(aPath.string()))
	{
		DEBUGLOG("Loaded Pixel Shader " + aPath.filename().string() + " from registry");
		return ourPixelShaderRegistry.at(aPath.string());
	}

	//TODO: Maybe not assert later, right now I want it to crash and not default to a standard pixel shader
	assert(std::filesystem::exists(aPath));

	std::ifstream psFile;
	psFile.open(aPath.string(), std::ios::binary);
	const std::string psData = { std::istreambuf_iterator<char>(psFile), std::istreambuf_iterator<char>() };
	psFile.close();

	ComPtr<ID3D11PixelShader> pixelShader;
	AssertIfFailed(DX11::Device->CreatePixelShader(psData.data(), psData.size(), nullptr, pixelShader.GetAddressOf()));

	DEBUGLOG("Loaded Pixel Shader " + aPath.filename().string());

	std::shared_ptr<PixelShader> loadedPixelShader = std::make_shared<PixelShader>();
	loadedPixelShader->myPixelShader = pixelShader;
	ourPixelShaderRegistry.insert(std::pair(aPath.string(), loadedPixelShader));
	return loadedPixelShader;
}