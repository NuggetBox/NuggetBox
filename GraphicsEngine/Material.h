#pragma once
#include "Math/Vector3.hpp"
#include "DX11.h"
#include "Texture.h"

namespace MaterialTextureChannel
{
	enum
	{
		Albedo,
		Normal,
		COUNT
	};
}

struct MaterialData
{
	Utility::Vector3f Albedo;
	float padding;
};

class Material
{
public:
	//void Initialize(const std::string& aName, Utility::Vector3f aAlbedo);
	void SetAlbedoTexture(std::shared_ptr<Texture> aTexture);
	void SetNormalMap(std::shared_ptr<Texture> aTexture);

	void SetAsResource(ComPtr<ID3D11Resource> aMaterialBuffer);

	const std::string& GetName() const { return myName; }
	//const Utility::Vector3<float>& GetAlbedo() const { return myMaterialData.Albedo; }
private:
	static std::unordered_map<std::string, std::shared_ptr<Material>> ourMaterialRegistry;

	std::array<std::shared_ptr<Texture>, MaterialTextureChannel::COUNT> myTextures;

	std::string myName;
	MaterialData myMaterialData;
};