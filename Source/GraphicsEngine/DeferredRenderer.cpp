#include "NuggetBox.pch.h"
#include "DeferredRenderer.h"

#include "RenderStructs.h"

void DeferredRenderer::Initialize()
{
    D3D11_BUFFER_DESC bufferDescription = { 0 };
    bufferDescription.Usage = D3D11_USAGE_DYNAMIC;
    bufferDescription.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    bufferDescription.CPUAccessFlags = D3D10_CPU_ACCESS_WRITE;

    //Create frame buffer
    bufferDescription.ByteWidth = sizeof(FrameBufferData);
    AssertIfFailed(DX11::Device->CreateBuffer(&bufferDescription, nullptr, myFrameBuffer.GetAddressOf()));

    //Create object buffer
    bufferDescription.ByteWidth = sizeof(ObjectBufferData);
    AssertIfFailed(DX11::Device->CreateBuffer(&bufferDescription, nullptr, myObjectBuffer.GetAddressOf()));

    //Create material buffer, unused now?
    bufferDescription.ByteWidth = sizeof(MaterialData);
    AssertIfFailed(DX11::Device->CreateBuffer(&bufferDescription, nullptr, myMaterialBuffer.GetAddressOf()));

    //Create light buffer
    bufferDescription.ByteWidth = sizeof(LightBufferData);
    AssertIfFailed(DX11::Device->CreateBuffer(&bufferDescription, nullptr, myLightBuffer.GetAddressOf()));

    DEBUGLOG("Created Buffers for Deferred Renderer");

    myPixelShader = PixelShader::Load("Shaders/Deferred_PS.cso");
    //TODO: Load fullscreen shader
    myFullscreenShader = VertexShader::Load("Shaders/Fullscreen_VS.cso");
    //TODO: Load environment shader
    myEnvironmentShader = PixelShader::Load("Shaders/Environment_PS.cso");

    DEBUGLOG("Deferred Renderer Initialized");
}

void DeferredRenderer::GenerateGBuffer(const std::shared_ptr<Camera>& aCamera,
                                       const std::vector<std::shared_ptr<Model>>& aModelList, float aDeltaTime, float aTotalTime)
{
    D3D11_MAPPED_SUBRESOURCE bufferData;

    myFrameBufferData.View = Matrix4x4<float>::GetFastInverse(aCamera->GetTransform().GetMatrix());
    myFrameBufferData.Projection = aCamera->GetProjectionMatrix();
    myFrameBufferData.CamTranslation = aCamera->GetTransform().GetPosition();

    //Used for deferred rendering?
    //myFrameBufferData.RenderMode = static_cast<UINT>(aRenderMode);

    //Map frame buffer resource
    ZeroMemory(&bufferData, sizeof(D3D11_MAPPED_SUBRESOURCE));
    AssertIfFailed(DX11::Context->Map(myFrameBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &bufferData));
    memcpy_s(bufferData.pData, sizeof(FrameBufferData), &myFrameBufferData, sizeof(FrameBufferData));
    DX11::Context->Unmap(myFrameBuffer.Get(), 0);

    DX11::Context->VSSetConstantBuffers(0, 1, myFrameBuffer.GetAddressOf());
    DX11::Context->PSSetConstantBuffers(0, 1, myFrameBuffer.GetAddressOf());

    for (auto& model : aModelList)
    {
        //Object Buffer
        myObjectBufferData.World = model->GetTransform().GetMatrix();
        memcpy_s(&myObjectBufferData.BoneData, sizeof(myObjectBufferData.BoneData) * MAX_BONES, model->GetBoneTransforms(), sizeof(Matrix4f) * MAX_BONES);
        ZeroMemory(&myObjectBufferData.HasBones, 16);
        myObjectBufferData.HasBones = model->HasBones();

        ZeroMemory(&bufferData, sizeof(D3D11_MAPPED_SUBRESOURCE));
        AssertIfFailed(DX11::Context->Map(myObjectBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &bufferData));
        memcpy_s(bufferData.pData, sizeof(ObjectBufferData), &myObjectBufferData, sizeof(ObjectBufferData));
        DX11::Context->Unmap(myObjectBuffer.Get(), 0);
        //

        DX11::Context->VSSetConstantBuffers(1, 1, myObjectBuffer.GetAddressOf());
        DX11::Context->PSSetConstantBuffers(1, 1, myObjectBuffer.GetAddressOf());

	    for (auto& mesh : model->GetMeshes())
	    {
            const auto& meshData = mesh.GetMeshData();

            //Configure Input Assembler (IA)
            DX11::Context->IASetVertexBuffers(0, 1, meshData.myVertexBuffer.GetAddressOf(), &meshData.myStride, &meshData.myOffset);
            DX11::Context->IASetIndexBuffer(meshData.myIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
            DX11::Context->IASetPrimitiveTopology(static_cast<D3D_PRIMITIVE_TOPOLOGY>(meshData.myPrimitiveTopology));
            //DX11::Context->IASetInputLayout(meshData.myInputLayout.Get());
            //

            meshData.myMaterial->SetAsResource(myMaterialBuffer);

            DX11::Context->PSSetConstantBuffers(2, 1, myMaterialBuffer.GetAddressOf());

            meshData.myVertexShader->Bind();
            myPixelShader->Bind();

            //TODO: Turn off geo shader somewhere else //Turn off Geometry Shader so that models work
            DX11::Context->GSSetShader(nullptr, nullptr, 0);

            DX11::Context->DrawIndexed(meshData.myNumberOfIndices, 0, 0);
	    }
    }
}

void DeferredRenderer::Render(const std::shared_ptr<Camera>& aCamera,
	const std::shared_ptr<DirectionalLight>& aDirectionalLight, const std::shared_ptr<AmbientLight>& anAmbientLight,
	float aDeltaTime, float aTotalTime, RenderMode aRenderMode)
{
    //Frame Buffer
    D3D11_MAPPED_SUBRESOURCE bufferData;
    myFrameBufferData.View = Matrix4x4<float>::GetFastInverse(aCamera->GetTransform().GetMatrix());
    myFrameBufferData.Projection = aCamera->GetProjectionMatrix();
    myFrameBufferData.CamTranslation = aCamera->GetTransform().GetPosition();
    myFrameBufferData.RenderMode = static_cast<UINT>(aRenderMode);

    //Used for deferred rendering?
    //myFrameBufferData.RenderMode = static_cast<UINT>(aRenderMode);
    ZeroMemory(&bufferData, sizeof(D3D11_MAPPED_SUBRESOURCE));
    AssertIfFailed(DX11::Context->Map(myFrameBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &bufferData));
    memcpy_s(bufferData.pData, sizeof(FrameBufferData), &myFrameBufferData, sizeof(FrameBufferData));
    DX11::Context->Unmap(myFrameBuffer.Get(), 0);
    //

    if (aDirectionalLight)
    {
        aDirectionalLight->SetAsResource(myLightBuffer);
    }

    if (anAmbientLight)
    {
        anAmbientLight->SetAsResource(nullptr);
    }

    DX11::Context->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    DX11::Context->IASetInputLayout(nullptr);
    DX11::Context->IASetVertexBuffers(0, 0, nullptr, nullptr, nullptr);
    DX11::Context->IASetIndexBuffer(nullptr, DXGI_FORMAT_UNKNOWN, 0);

    DX11::Context->GSSetShader(nullptr, nullptr, 0);

    myFullscreenShader->Bind();
    myEnvironmentShader->Bind();

    DX11::Context->Draw(3, 0);
}