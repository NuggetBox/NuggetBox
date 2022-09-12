#include "NuggetBox.pch.h"
#include "DirectionalLight.h"

#include "Core/DebugLogger.h"

std::shared_ptr<DirectionalLight> DirectionalLight::Create(Vector3f aColor, float anIntensity, Vector3f aDirection)
{
	DirectionalLight directionalLight;
	directionalLight.myLightBufferData.Color = aColor;
	directionalLight.myLightBufferData.Intensity = anIntensity;
	directionalLight.myLightBufferData.Direction = aDirection;
	DEBUGLOG("Created Directional Light");

	return std::make_shared<DirectionalLight>(directionalLight);
}

void DirectionalLight::SetAsResource(ComPtr<ID3D11Buffer> aLightBuffer)
{
	D3D11_MAPPED_SUBRESOURCE bufferData;
	ZeroMemory(&bufferData, sizeof(D3D11_MAPPED_SUBRESOURCE));

	AssertIfFailed(DX11::Context->Map(aLightBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &bufferData));

	memcpy_s(bufferData.pData, sizeof(LightBufferData), &myLightBufferData, sizeof(LightBufferData));

	DX11::Context->Unmap(aLightBuffer.Get(), 0);

	DX11::Context->PSSetConstantBuffers(3, 1, aLightBuffer.GetAddressOf());
}
