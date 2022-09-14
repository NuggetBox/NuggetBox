#include "NuggetBox.pch.h"
#include "PointLight.h"

std::shared_ptr<PointLight> PointLight::Create(Vector3f aColor, float anIntensity, Vector3f aPosition, float aRange)
{
	PointLight pointLight;
	pointLight.myLightBufferData.Color = aColor;
	pointLight.myLightBufferData.Intensity = anIntensity;
	pointLight.myLightBufferData.Position = aPosition;
	pointLight.myLightBufferData.Range = aRange;
	pointLight.myLightBufferData.LightType = static_cast<unsigned>(LightType::PointLight);

	pointLight.SetPosition(aPosition);

	DEBUGLOG("Created a Point Light");
	return std::make_shared<PointLight>(pointLight);
}

void PointLight::SetAsResource(ComPtr<ID3D11Buffer> aLightBuffer)
{
}
