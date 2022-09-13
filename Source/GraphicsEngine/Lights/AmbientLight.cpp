#include "NuggetBox.pch.h"
#include "AmbientLight.h"

#include "Core/DebugLogger.h"

std::shared_ptr<AmbientLight> AmbientLight::Create(const std::filesystem::path& aTexturePath)
{
	AmbientLight ambientLight;
	ambientLight.myTexture = Texture::Load(aTexturePath);
	ambientLight.myLightBufferData.LightType = static_cast<unsigned>(LightType::AmbientLight);
	DEBUGLOG("Created Ambient Light");

	return std::make_shared<AmbientLight>(ambientLight);
}

void AmbientLight::SetAsResource(ComPtr<ID3D11Buffer> aLightBuffer)
{
	if (myTexture)
	{
		myTexture->SetAsResource(10);
	}
}