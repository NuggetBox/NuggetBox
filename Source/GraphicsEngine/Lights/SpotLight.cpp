#include "NuggetBox.pch.h"
#include "SpotLight.h"

std::shared_ptr<SpotLight> SpotLight::Create(Utility::Vector3f aColor, float anIntensity, Utility::Vector3f aPosition, float aRange, Utility::Vector3f aRotation, float anInnerRadius, float anOuterRadius)
{
	SpotLight spotLight;
	ZeroMemory(&spotLight.myLightBufferData, sizeof(LightBufferData));

	spotLight.SetRotation(aRotation);
	spotLight.SetPosition(aPosition);

	spotLight.myLightBufferData.ObjectColor = aColor;
	spotLight.myLightBufferData.Intensity = anIntensity;
	spotLight.myLightBufferData.Position = aPosition;
	spotLight.myLightBufferData.Range = aRange;
	spotLight.myLightBufferData.Direction = spotLight.myTransform.GetForward();
	spotLight.myLightBufferData.SpotInnerRadius = anInnerRadius;
	spotLight.myLightBufferData.SpotOuterRadius = anOuterRadius;
	spotLight.myLightBufferData.LightType = static_cast<unsigned>(LightType::SpotLight);

	//TODO: Tweak values to make for good shadow rendering, spotlight
	constexpr float nearPlane = 1.0f;
	constexpr float farPlane = 15000.0f;
	constexpr POINT shadowResolution = { 2048, 2048 };
	constexpr POINT projectionSize = { 2048, 2048 };

	spotLight.myLightBufferData.ViewMatrix[0] = Utility::Matrix4f::GetFastInverse(spotLight.myTransform.GetMatrix());
	spotLight.myLightBufferData.ProjectionMatrix = Utility::Matrix4f::CreateProjectionMatrixPerspective(projectionSize.x, projectionSize.y, nearPlane, farPlane, 90);
	spotLight.myLightBufferData.CastShadows = true;

	spotLight.myLightBufferData.FarPlane = farPlane;
	spotLight.myLightBufferData.NearPlane = nearPlane;

	spotLight.myShadowMap = DepthStencil::Create(shadowResolution.x, shadowResolution.y);

	DEBUGLOG("Created a Spot Light");
	return std::make_shared<SpotLight>(spotLight);
}

void SpotLight::SetAsResource(ComPtr<ID3D11Buffer> aLightBuffer)
{
}
