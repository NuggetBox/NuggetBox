#include "NuggetBox.pch.h"
#include "Texture.h"
#include "DDSTextureLoader/DDSTextureLoader11.h"

std::unordered_map<std::string, std::shared_ptr<Texture>> Texture::ourTextureRegistry;

std::shared_ptr<Texture> Texture::Load(const std::filesystem::path& aPath)
{
	if (ourTextureRegistry.contains(aPath.string()))
	{
		return ourTextureRegistry.at(aPath.string());
	}

	Texture texture;

	//Try to load texture from file
	if (std::filesystem::exists(aPath))
	{
		AssertIfFailed(DirectX::CreateDDSTextureFromFile(DX11::Device.Get(), aPath.c_str(), texture.myTexture.GetAddressOf(), texture.myShaderResourceView.GetAddressOf()))
		ourTextureRegistry.insert(std::pair(aPath.string(), std::make_shared<Texture>(texture)));
	}
	//Couldn't find a texture that matches our TGA standard, load default texture
	else
	{
		if (ourTextureRegistry.contains(myDefaultAlbedoPath))
		{
			return ourTextureRegistry.at(myDefaultAlbedoPath);
		}

		AssertIfFailed(DirectX::CreateDDSTextureFromFile(DX11::Device.Get(), std::wstring(myDefaultAlbedoPath.begin(), myDefaultAlbedoPath.end()).c_str(), texture.myTexture.GetAddressOf(), texture.myShaderResourceView.GetAddressOf()))
		ourTextureRegistry.insert(std::pair(myDefaultAlbedoPath, std::make_shared<Texture>(texture)));
	}

	return std::make_shared<Texture>(texture);
}

void Texture::SetAsResource(UINT aSlot)
{
	DX11::Context->PSSetShaderResources(aSlot, 1, myShaderResourceView.GetAddressOf());
}

Texture::~Texture()
{
	myTexture.Reset();
	myShaderResourceView.Reset();
}