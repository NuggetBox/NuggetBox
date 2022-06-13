#include "NuggetBox.pch.h"
#include "Material.h"

void Material::SetAlbedoTexture(std::shared_ptr<Texture> aTexture)
{
	myTextures[MaterialTextureChannel::Albedo] = aTexture;
}

void Material::SetNormalMap(std::shared_ptr<Texture> aTexture)
{
	myTextures[MaterialTextureChannel::Normal] = aTexture;
}

void Material::SetSurfaceTexture(std::shared_ptr<Texture> aTexture)
{
	myTextures[MaterialTextureChannel::Surface] = aTexture;
}

void Material::SetAsResource(ComPtr<ID3D11Resource> aMaterialBuffer)
{
	//Unused now that we sample from textures?
	D3D11_MAPPED_SUBRESOURCE bufferData;
	ZeroMemory(&bufferData, sizeof(D3D11_MAPPED_SUBRESOURCE));
	AssertIfFailed(DX11::Context->Map(aMaterialBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &bufferData));
	memcpy_s(bufferData.pData, sizeof(MaterialData), &myMaterialData, sizeof(MaterialData));
	DX11::Context->Unmap(aMaterialBuffer.Get(), 0);

	if (myTextures[MaterialTextureChannel::Albedo])
	{
		myTextures[MaterialTextureChannel::Albedo]->SetAsResource(MaterialTextureChannel::Albedo);
	}

	if (myTextures[MaterialTextureChannel::Normal])
	{
		myTextures[MaterialTextureChannel::Normal]->SetAsResource(MaterialTextureChannel::Normal);
	}

	if (myTextures[MaterialTextureChannel::Surface])
	{
		myTextures[MaterialTextureChannel::Surface]->SetAsResource(MaterialTextureChannel::Surface);
	}
}
