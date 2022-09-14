#include "NuggetBox.pch.h"
#include "ShadowRenderer.h"

#include <Timer.h>

void ShadowRenderer::Initialize()
{
    D3D11_BUFFER_DESC bufferDescription = { 0 };
    bufferDescription.Usage = D3D11_USAGE_DYNAMIC;
    bufferDescription.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    bufferDescription.CPUAccessFlags = D3D10_CPU_ACCESS_WRITE;

    //Create frame buffer
    bufferDescription.ByteWidth = sizeof(FrameBufferData);
    AssertIfFailed(DX11::Device->CreateBuffer(&bufferDescription, nullptr, myFrameBuffer.GetAddressOf()));
    //DEBUGLOG("Created Frame Buffer");

    //Create object buffer
    bufferDescription.ByteWidth = sizeof(ObjectBufferData);
    AssertIfFailed(DX11::Device->CreateBuffer(&bufferDescription, nullptr, myObjectBuffer.GetAddressOf()));
    //DEBUGLOG("Created Object Buffer");

    //Create material buffer, unused now?
    bufferDescription.ByteWidth = sizeof(MaterialData);
    AssertIfFailed(DX11::Device->CreateBuffer(&bufferDescription, nullptr, myMaterialBuffer.GetAddressOf()));
    //DEBUGLOG("Created Material Buffer");

    DEBUGLOG("Created CBuffers for Shadow Renderer");

    DEBUGLOG("Shadow Renderer Initialized");
}

void ShadowRenderer::RenderShadowPassPerLight(const std::shared_ptr<Light>& aLight, const std::vector<std::shared_ptr<Model>>& aModelList)
{
	const LightBufferData lightBufferData = aLight->GetLightBuffer();
	D3D11_MAPPED_SUBRESOURCE bufferData;

	myFrameBufferData.View = lightBufferData.ViewMatrix;
	myFrameBufferData.Projection = lightBufferData.ProjectionMatrix;
    myFrameBufferData.CamTranslation = lightBufferData.Position;
    myFrameBufferData.NearPlane = lightBufferData.NearPlane;
    myFrameBufferData.FarPlane = lightBufferData.FarPlane;
    myFrameBufferData.RenderMode = 0;
    myFrameBufferData.DeltaTime = Timer::GetDeltaTime();
    myFrameBufferData.TotalTime = Timer::GetTotalTime();

    DX11::Context->PSSetShader(nullptr, nullptr, 0);
    DX11::Context->GSSetShader(nullptr, nullptr, 0);

    //Map frame buffer resource
    ZeroMemory(&bufferData, sizeof(D3D11_MAPPED_SUBRESOURCE));
    AssertIfFailed(DX11::Context->Map(myFrameBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &bufferData));
    memcpy_s(bufferData.pData, sizeof(FrameBufferData), &myFrameBufferData, sizeof(FrameBufferData));
    DX11::Context->Unmap(myFrameBuffer.Get(), 0);
    DX11::Context->VSSetConstantBuffers(0, 1, myFrameBuffer.GetAddressOf());
    //


}

void ShadowRenderer::RenderShadowPassPerModel(const std::shared_ptr<Model>& aModel, const std::vector<std::shared_ptr<Light>>& aLightList)
{
}
