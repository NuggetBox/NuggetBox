#include "NuggetBox.pch.h"
#include "Material.h"

std::unordered_map<std::string, std::shared_ptr<Material>> Material::ourMaterialRegistry;

void Material::SetAlbedoTexture(std::shared_ptr<Texture> aTexture)
{
	myTextures[MaterialTextureChannel::Albedo] = aTexture;
}

void Material::SetAsResource(ComPtr<ID3D11Resource> aMaterialBuffer)
{
	D3D11_MAPPED_SUBRESOURCE bufferData;
	ZeroMemory(&bufferData, sizeof(D3D11_MAPPED_SUBRESOURCE));
	AssertIfFailed(DX11::Context->Map(aMaterialBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &bufferData))
	memcpy_s(bufferData.pData, sizeof(MaterialData), &myMaterialData, sizeof(MaterialData));
	DX11::Context->Unmap(aMaterialBuffer.Get(), 0);

	if (myTextures[MaterialTextureChannel::Albedo])
	{
		myTextures[MaterialTextureChannel::Albedo]->SetAsResource(MaterialTextureChannel::Albedo);
	}
}