#include "NuggetBox.pch.h"
#include "ForwardRenderer.h"

#include "AmbientLight.h"
#include "DirectionalLight.h"
#include "DX11.h"

void ForwardRenderer::Initialize()
{
    // Rasterizer that disables backface culling
    /*D3D11_RASTERIZER_DESC cmDesc = {};
    cmDesc.FillMode = D3D11_FILL_SOLID;
    cmDesc.CullMode = D3D11_CULL_NONE;
    cmDesc.FrontCounterClockwise = false;
    DX11::Device->CreateRasterizerState(&cmDesc, myRasterizerSate.GetAddressOf());
    DX11::Context->RSSetState(myRasterizerSate.Get());*/

    D3D11_BUFFER_DESC bufferDescription = { 0 };
    bufferDescription.Usage = D3D11_USAGE_DYNAMIC;
    bufferDescription.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    bufferDescription.CPUAccessFlags = D3D10_CPU_ACCESS_WRITE;

    //Create frame buffer
    bufferDescription.ByteWidth = sizeof(FrameBufferData);
    AssertIfFailed(DX11::Device->CreateBuffer(&bufferDescription, nullptr, myFrameBuffer.GetAddressOf()))

    //Create object buffer
    bufferDescription.ByteWidth = sizeof(ObjectBufferData);
    AssertIfFailed(DX11::Device->CreateBuffer(&bufferDescription, nullptr, myObjectBuffer.GetAddressOf()))

    //Create material buffer, unused now?
    bufferDescription.ByteWidth = sizeof(MaterialData);
    AssertIfFailed(DX11::Device->CreateBuffer(&bufferDescription, nullptr, myMaterialBuffer.GetAddressOf()))

    //Create light buffer
    bufferDescription.ByteWidth = sizeof(LightBufferData);
    AssertIfFailed(DX11::Device->CreateBuffer(&bufferDescription, nullptr, myLightBuffer.GetAddressOf()))
}

void ForwardRenderer::Render(const std::shared_ptr<Camera>& aCamera, const std::vector<std::shared_ptr<Model>>& aModelList, 
    const std::shared_ptr<DirectionalLight> aDirectionalLight, const std::shared_ptr<AmbientLight> anAmbientLight)
{
    D3D11_MAPPED_SUBRESOURCE bufferData;

    myFrameBufferData.View = Matrix4x4<float>::GetFastInverse(aCamera->GetTransform().GetMatrix());
    myFrameBufferData.Projection = aCamera->GetProjectionMatrix();

    //Map frame buffer resource
    ZeroMemory(&bufferData, sizeof(D3D11_MAPPED_SUBRESOURCE));
    AssertIfFailed(DX11::Context->Map(myFrameBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &bufferData))
    memcpy_s(bufferData.pData, sizeof(FrameBufferData), &myFrameBufferData, sizeof(FrameBufferData));
    DX11::Context->Unmap(myFrameBuffer.Get(), 0);

    DX11::Context->VSSetConstantBuffers(0, 1, myFrameBuffer.GetAddressOf());

    for (auto& model : aModelList)
    {
        for (auto& mesh : model->GetMeshes())
        {
            const auto& meshData = mesh.GetMeshData();
            myObjectBufferData.World = model->GetTransform().GetMatrix();

            //Copy BoneTransforms
            memcpy_s(&myObjectBufferData.BoneData, sizeof(myObjectBufferData.BoneData) * MAX_BONES, model->GetBoneTransforms(), sizeof(Matrix4f) * MAX_BONES);

            //Cursed bool padding problems, zeromemory for now
            ZeroMemory(&myObjectBufferData.HasBones, 16);
            myObjectBufferData.HasBones = model->HasBones();

            //Map object buffer resource
            ZeroMemory(&bufferData, sizeof(D3D11_MAPPED_SUBRESOURCE));
            AssertIfFailed(DX11::Context->Map(myObjectBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &bufferData))
            memcpy_s(bufferData.pData, sizeof(ObjectBufferData), &myObjectBufferData, sizeof(ObjectBufferData));
            DX11::Context->Unmap(myObjectBuffer.Get(), 0);

            //Map material buffer resource
            if (meshData.myMaterial)
            {
                meshData.myMaterial->SetAsResource(myMaterialBuffer);
            }
            else
            {
                //TODO: Default material
            }
            myMaterialBufferData.Albedo = meshData.myMaterial->GetAlbedo();
            ZeroMemory(&bufferData, sizeof(D3D11_MAPPED_SUBRESOURCE));
            AssertIfFailed(DX11::Context->Map(myMaterialBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &bufferData))
            memcpy_s(bufferData.pData, sizeof(MaterialData), &myMaterialBufferData, sizeof(MaterialData));
            DX11::Context->Unmap(myMaterialBuffer.Get(), 0);

            //Set lights as resources
            if (aDirectionalLight)
            {
                aDirectionalLight->SetAsResource(myLightBuffer);
            }
            if (anAmbientLight)
            {
                anAmbientLight->SetAsResource(nullptr);
            }

            DX11::Context->IASetVertexBuffers(0, 1, meshData.myVertexBuffer.GetAddressOf(), &meshData.myStride, &meshData.myOffset);
            DX11::Context->IASetIndexBuffer(meshData.myIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);

            DX11::Context->IASetPrimitiveTopology(static_cast<D3D_PRIMITIVE_TOPOLOGY>(meshData.myPrimitiveTopology));
            //DX11::Context->IASetInputLayout(meshData.myInputLayout.Get());

            //DX11::Context->VSSetShader(meshData.myVertexShader.Get(), nullptr, 0);
            //DX11::Context->PSSetShader(meshData.myPixelShader.Get(), nullptr, 0);
            meshData.myVertexShader->Bind();
            meshData.myPixelShader->Bind();

            DX11::Context->VSSetConstantBuffers(1, 1, myObjectBuffer.GetAddressOf());
            DX11::Context->PSSetConstantBuffers(1, 1, myObjectBuffer.GetAddressOf());

            DX11::Context->PSSetConstantBuffers(2, 1, myMaterialBuffer.GetAddressOf());

            DX11::Context->DrawIndexed(meshData.myNumberOfIndices, 0, 0);
        }
    }
}