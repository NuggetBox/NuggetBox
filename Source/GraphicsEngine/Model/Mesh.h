#pragma once
#include <wrl.h>
#include <d3d11.h>

#include "Model/FBXStructs.h"
#include "Material.h"
#include "ShaderClasses/VertexShader.h"
#include "ShaderClasses/PixelShader.h"

using namespace Microsoft::WRL;

struct MeshData
{
	ComPtr<ID3D11Buffer> myVertexBuffer;
	ComPtr<ID3D11Buffer> myIndexBuffer;

	UINT myPrimitiveTopology;
	UINT myNumberOfVertices;
	UINT myNumberOfIndices;
	UINT myStride;
	UINT myOffset;

	//ComPtr<ID3D11InputLayout> myInputLayout;
	//ComPtr<ID3D11VertexShader> myVertexShader;
	//ComPtr<ID3D11PixelShader> myPixelShader;
	std::shared_ptr<VertexShader> myVertexShader;
	std::shared_ptr<PixelShader> myPixelShader;

	std::shared_ptr<Material> myMaterial;
};

class Mesh
{
public:
	Mesh(std::vector<VertexData>& someVertices, std::vector<unsigned int>& someIndices);
	void SetMaterial(std::shared_ptr<Material> aMaterial);
	MeshData GetMeshData() const;

private:
	MeshData myMeshData;
};