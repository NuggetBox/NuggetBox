#include "NuggetBox.pch.h"
#include "DirectionalLight.h"

#include "Core/DebugLogger.h"

std::shared_ptr<DirectionalLight> DirectionalLight::Create(Vector3f aColor, float anIntensity, Vector3f aRotation)
{
	DirectionalLight directionalLight;
	ZeroMemory(&directionalLight.myLightBufferData, sizeof(LightBufferData));

	directionalLight.myTransform.SetRotation(aRotation);

	directionalLight.myLightBufferData.Color = aColor;
	directionalLight.myLightBufferData.Intensity = anIntensity;
	directionalLight.myLightBufferData.Direction = directionalLight.myTransform.GetForward();
	directionalLight.myLightBufferData.LightType = static_cast<unsigned>(LightType::DirectionalLight);

	//TODO: Tweak values to make for good shadow rendering, dirlight
	constexpr float nearPlane = 1.0f;
	constexpr float farPlane = 15000.0f;
	constexpr POINT shadowResolution = { 2048, 2048 };
	constexpr POINT projectionSize = { 2048, 2048 };

	directionalLight.myLightBufferData.ProjectionMatrix = Matrix4f::CreateProjectionMatrixOrthographic(projectionSize.x, projectionSize.y, nearPlane, farPlane);
	directionalLight.myLightBufferData.CastShadows = true;

	directionalLight.myLightBufferData.FarPlane = farPlane;
	directionalLight.myLightBufferData.NearPlane = nearPlane;

	directionalLight.myShadowMap = DepthStencil::Create("ShadowMap", shadowResolution.x, shadowResolution.y);

	DEBUGLOG("Created Directional Light");

	return std::make_shared<DirectionalLight>(directionalLight);
}

void DirectionalLight::SetAsResource(ComPtr<ID3D11Buffer> aLightBuffer)
{
	//Outdated, renderers now fill a scene light buffer instead, with dir light & point lights etc.

	/*D3D11_MAPPED_SUBRESOURCE bufferData;
	ZeroMemory(&bufferData, sizeof(D3D11_MAPPED_SUBRESOURCE));
	AssertIfFailed(DX11::Context->Map(aLightBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &bufferData));
	memcpy_s(bufferData.pData, sizeof(LightBufferData), &myLightBufferData, sizeof(LightBufferData));
	DX11::Context->Unmap(aLightBuffer.Get(), 0);
	DX11::Context->PSSetConstantBuffers(3, 1, aLightBuffer.GetAddressOf());*/
}

void DirectionalLight::Update(const Vector3f& aCameraPosition)
{
	//TODO: Find a good distance to move the camera relative to player camera
	Vector3f moveBackDiff = myTransform.GetBackward() * 10000.0f;
	myTransform.SetPosition(aCameraPosition + moveBackDiff);
	FORMATLOG("Dir light pos: X:{} Y:{} Z:{} Length:{}", myTransform.GetXPosition(), myTransform.GetYPosition(), myTransform.GetZPosition(), myTransform.GetPosition().Length());
	myLightBufferData.ViewMatrix = Matrix4f::GetFastInverse(myTransform.GetMatrix());
}