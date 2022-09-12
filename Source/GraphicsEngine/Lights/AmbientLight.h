#pragma once
#include "Light.h"
#include "Model/Texture.h"

class AmbientLight : Light
{
public:
	static std::shared_ptr<AmbientLight> Create(const std::filesystem::path& aTexturePath);

	void SetAsResource(ComPtr<ID3D11Buffer> aLightBuffer) override;

private:
	std::shared_ptr<Texture> myTexture;
};
