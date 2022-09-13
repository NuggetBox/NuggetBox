#include "NuggetBox.pch.h"
#include "SpotLight.h"

std::shared_ptr<SpotLight> SpotLight::Create(Vector3f aColor, float anIntensity, Vector3f aDirection, float aRange, float anInnerRadius, float anOuterRadius)
{
	SpotLight spotLight;
	spotLight.myLightBufferData.Color = aColor;
	spotLight.myLightBufferData.Intensity = anIntensity;
	spotLight.myLightBufferData.Direction = aDirection;
	spotLight.myLightBufferData.Range = aRange;
	spotLight.myLightBufferData.SpotInnerRadius = anInnerRadius;
	spotLight.myLightBufferData.SpotOuterRadius = anOuterRadius;
	spotLight.myLightBufferData.LightType = static_cast<unsigned>(LightType::SpotLight);
	DEBUGLOG("Created a Spot Light");

	return std::make_shared<SpotLight>(spotLight);
}

void SpotLight::SetAsResource(ComPtr<ID3D11Buffer> aLightBuffer)
{
}
