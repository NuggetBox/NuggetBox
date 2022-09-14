#include "NuggetBox.pch.h"
#include "DirectionalLight.h"

#include "Core/DebugLogger.h"

std::shared_ptr<DirectionalLight> DirectionalLight::Create(Vector3f aColor, float anIntensity, Vector3f aRotation)
{
	DirectionalLight directionalLight;
	directionalLight.myTransform.SetRotation(aRotation);

	directionalLight.myLightBufferData.Color = aColor;
	directionalLight.myLightBufferData.Intensity = anIntensity;
	directionalLight.myLightBufferData.Direction = directionalLight.myTransform.GetForward();
	directionalLight.myLightBufferData.LightType = static_cast<unsigned>(LightType::DirectionalLight);

	constexpr float nearPlane = 1.0f;
	constexpr float farPlane = 25000.0f;
	constexpr POINT resolution = { 2048, 2048 };

	//TODO: IS THIS RIGHT? Update every frame, move with camera/player
	//directionalLight.myLightBufferData.ViewMatrix = Matrix4f::GetFastInverse(myTransform.GetMatrix());

	directionalLight.myLightBufferData.ProjectionMatrix = Matrix4f::CreateProjectionMatrixOrthographic(resolution.x, resolution.y, nearPlane, farPlane);
	directionalLight.myLightBufferData.CastShadows = true;

	directionalLight.myLightBufferData.FarPlane = farPlane;
	directionalLight.myLightBufferData.NearPlane = nearPlane;

	directionalLight.myShadowMap = DepthStencil::Create("ShadowMap", resolution.x, resolution.y);

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
