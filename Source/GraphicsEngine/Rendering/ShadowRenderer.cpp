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

	myFrameBufferData.View = lightBufferData.ViewMatrix[0];
	myFrameBufferData.Projection = lightBufferData.ProjectionMatrix;
    myFrameBufferData.CamTranslation = lightBufferData.Position;
    myFrameBufferData.NearPlane = lightBufferData.NearPlane;
    myFrameBufferData.FarPlane = lightBufferData.FarPlane;
    myFrameBufferData.RenderMode = 0;
    myFrameBufferData.DeltaTime = Utility::Timer::GetDeltaTime();
    myFrameBufferData.TotalTime = static_cast<float>(Utility::Timer::GetTotalTime());

    DX11::Context->PSSetShader(nullptr, nullptr, 0);
    DX11::Context->GSSetShader(nullptr, nullptr, 0);

    //Map frame buffer resource
    ZeroMemory(&bufferData, sizeof(D3D11_MAPPED_SUBRESOURCE));
    AssertIfFailed(DX11::Context->Map(myFrameBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &bufferData));
    memcpy_s(bufferData.pData, sizeof(FrameBufferData), &myFrameBufferData, sizeof(FrameBufferData));
    DX11::Context->Unmap(myFrameBuffer.Get(), 0);
    DX11::Context->VSSetConstantBuffers(0, 1, myFrameBuffer.GetAddressOf());
    //

    for (auto& model : aModelList)
    {
        //Set Object buffer data
        myObjectBufferData.World = model->GetTransform().GetMatrix();
        memcpy_s(&myObjectBufferData.BoneData, sizeof(myObjectBufferData.BoneData) * MAX_BONES, model->GetBoneTransforms(), sizeof(Utility::Matrix4f) * MAX_BONES);
        ZeroMemory(&myObjectBufferData.HasBones, 16);
        myObjectBufferData.HasBones = model->HasBones();
        myObjectBufferData.IsInstanced = model->HasRenderedInstances();
        //

        //Map object buffer resource
        ZeroMemory(&bufferData, sizeof(D3D11_MAPPED_SUBRESOURCE));
        AssertIfFailed(DX11::Context->Map(myObjectBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &bufferData));
        memcpy_s(bufferData.pData, sizeof(ObjectBufferData), &myObjectBufferData, sizeof(ObjectBufferData));
        DX11::Context->Unmap(myObjectBuffer.Get(), 0);
        DX11::Context->VSSetConstantBuffers(1, 1, myObjectBuffer.GetAddressOf());
        //

	    for (auto& mesh : model->GetMeshes())
	    {
            const auto& meshData = mesh.GetMeshData();

            //Configure IA
            DX11::Context->IASetIndexBuffer(meshData.myIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
            DX11::Context->IASetPrimitiveTopology(static_cast<D3D_PRIMITIVE_TOPOLOGY>(meshData.myPrimitiveTopology));
            meshData.myVertexShader->Bind();
            //

            if (model->HasRenderedInstances())
            {
                ID3D11Buffer* buffers[2] = { meshData.myVertexBuffer.Get(), model->GetInstanceBuffer().Get() };
                UINT stride[2] = { meshData.myStride, sizeof(InstanceData) };
                UINT offset[2] = { 0, 0 };

                DX11::Context->IASetVertexBuffers(0, 2, buffers, stride, offset);
                DX11::Context->DrawIndexedInstanced(meshData.myNumberOfIndices, model->GetNumberOfInstances(), 0, 0, 0);
            }
            else
            {
	            DX11::Context->IASetVertexBuffers(0, 1, meshData.myVertexBuffer.GetAddressOf(), &meshData.myStride, &meshData.myOffset);
	            DX11::Context->DrawIndexed(meshData.myNumberOfIndices, 0, 0);
            }
	    }
    }
}

void ShadowRenderer::RenderShadowPassPerModel(const std::shared_ptr<Model>& aModel, const std::vector<std::shared_ptr<Light>>& aLightList)
{
}
