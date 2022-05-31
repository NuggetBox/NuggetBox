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
		//TODO: Refactor
		size_t dot = aPath.string().rfind('.');
		std::string defaultPath = myDefaultAlbedoPath;

		//Check if there is a dot at all in the file name
		if (dot < aPath.string().length())
		{
			std::string ending = aPath.string().substr(dot - 3, 2);

			if (ending == "_C")
			{
				defaultPath = myDefaultAlbedoPath;
			}
			else if (ending == "_N")
			{
				defaultPath = myDefaultNormalPath;
			}
			else if (ending == "_M")
			{
				defaultPath = myDefaultRoughnessPath;
			}

			if (ourTextureRegistry.contains(defaultPath))
			{
				return ourTextureRegistry.at(defaultPath);
			}
		}

		AssertIfFailed(DirectX::CreateDDSTextureFromFile(DX11::Device.Get(), std::wstring(defaultPath.begin(), defaultPath.end()).c_str(), texture.myTexture.GetAddressOf(), texture.myShaderResourceView.GetAddressOf()))
		ourTextureRegistry.insert(std::pair(defaultPath, std::make_shared<Texture>(texture)));
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