#include "NuggetBox.pch.h"
#include <filesystem>

#include "DX11.h"
#include "Mesh.h"

#include "VertexShader.h"

Mesh::Mesh(std::vector<VertexData>& someVertices, std::vector<unsigned>& someIndices)
{
	D3D11_BUFFER_DESC vertexBufferDesc{};
	vertexBufferDesc.ByteWidth = static_cast<UINT>(someVertices.size()) * static_cast<UINT>(sizeof(VertexData));
	vertexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	D3D11_SUBRESOURCE_DATA vertexSubresourceData{};
	vertexSubresourceData.pSysMem = &someVertices[0];

	ID3D11Buffer* vertexBuffer;
	AssertIfFailed(DX11::Device->CreateBuffer(&vertexBufferDesc, &vertexSubresourceData, &vertexBuffer))

	D3D11_BUFFER_DESC indexBufferDesc {};
	indexBufferDesc.ByteWidth = static_cast<UINT>(someIndices.size()) * static_cast<UINT>(sizeof(unsigned int));
	indexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

	D3D11_SUBRESOURCE_DATA indexSubresourceData{};
	indexSubresourceData.pSysMem = &someIndices[0];

	ID3D11Buffer* indexBuffer;
	AssertIfFailed(DX11::Device->CreateBuffer(&indexBufferDesc, &indexSubresourceData, &indexBuffer))

	//Load Vertex Shader
	std::shared_ptr<VertexShader> vertexShader = VertexShader::Load("Shaders/Standard_VS.cso");
	/*std::ifstream vsFile;
	vsFile.open("Shaders/Standard_VS.cso", std::ios::binary);
	std::string vsData = { std::istreambuf_iterator<char>(vsFile), std::istreambuf_iterator<char>() };
	ID3D11VertexShader* vertexShader;
	AssertIfFailed(DX11::Device->CreateVertexShader(vsData.data(), vsData.size(), nullptr, &vertexShader))
	vsFile.close();*/

	//Load Pixel Shader
	std::shared_ptr<PixelShader> pixelShader = PixelShader::Load("Shaders/Standard_PS.cso");
	/*std::ifstream psFile;
	psFile.open("Shaders/Standard_PS.cso", std::ios::binary);
	std::string psData = { std::istreambuf_iterator<char>(psFile), std::istreambuf_iterator<char>() };
	ID3D11PixelShader* pixelShader;
	AssertIfFailed(DX11::Device->CreatePixelShader(psData.data(), psData.size(), nullptr, &pixelShader))
	psFile.close();*/

	//Create input layout
	/*D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"COLOR", 1, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"COLOR", 2, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"COLOR", 3, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"UV", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"UV", 1, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"UV", 2, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"UV", 3, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"BINORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"BONEIDS", 0, DXGI_FORMAT_R32G32B32A32_UINT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"BONEWEIGHTS", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};*/

	/*ID3D11InputLayout* inputLayout;
	AssertIfFailed(DX11::Device->CreateInputLayout(layout, sizeof(layout) / sizeof(D3D11_INPUT_ELEMENT_DESC), vsData.data(), vsData.size(), &inputLayout))*/

	myMeshData.myNumberOfVertices = static_cast<UINT>(someVertices.size());
	myMeshData.myNumberOfIndices = static_cast<UINT>(someIndices.size());
	myMeshData.myStride = sizeof(VertexData);
	myMeshData.myOffset = 0;
	myMeshData.myVertexBuffer = vertexBuffer;
	myMeshData.myIndexBuffer = indexBuffer;
	myMeshData.myVertexShader = vertexShader;
	myMeshData.myPixelShader = pixelShader;
	myMeshData.myPrimitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	//myMeshData.myInputLayout = inputLayout;
}

void Mesh::SetMaterial(std::shared_ptr<Material> aMaterial)
{
	myMeshData.myMaterial = aMaterial;
}

MeshData Mesh::GetMeshData() const
{
	return myMeshData;
}
