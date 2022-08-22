#include "NuggetBox.pch.h"
#include "VertexShader.h"

#include "DebugLogger.h"

//std::unordered_map<std::string, std::shared_ptr<VertexShader>> VertexShader::ourVertexShaderRegistry;
//ComPtr<ID3D11InputLayout> VertexShader::ourInputLayout;
//bool VertexShader::ourInputLayoutIsSet;

void VertexShader::Bind() const
{
	if (myInputLayout != nullptr)
	{
		//TODO: Should IA be set in Bind function? I think it works
		DX11::Context->IASetInputLayout(myInputLayout.Get());
	}
	else
	{
		DEBUGERROR("No input layout found for vertex shader " + myName);
	}

	DX11::Context->VSSetShader(myVertexShader.Get(), nullptr, 0);
}

std::shared_ptr<VertexShader> VertexShader::Load(const std::filesystem::path& aPath, InputLayoutType anInputLayoutType)
{
	if (ourVertexShaderRegistry.contains(aPath.string()))
	{
		DEBUGLOG("Loaded Vertex Shader " + aPath.filename().string() + " from registry successfully");
		return ourVertexShaderRegistry.at(aPath.string());
	}

	//TODO: Maybe not assert later, right now I want it to crash and not default to a standard vertex shader
	assert(std::filesystem::exists(aPath));

	std::ifstream vsFile;
	vsFile.open(aPath.string(), std::ios::binary);
	const std::string vsData = { std::istreambuf_iterator<char>(vsFile), std::istreambuf_iterator<char>() };
	vsFile.close();

	ComPtr<ID3D11VertexShader> vertexShader;
	AssertIfFailed(DX11::Device->CreateVertexShader(vsData.data(), vsData.size(), nullptr, vertexShader.GetAddressOf()));

	std::string name = aPath.filename().string();
	DEBUGLOG("Loaded Vertex Shader " + name);

	/*if (!ourInputLayoutIsSet)
	{
		SetInputLayout(vsData);
		ourInputLayoutIsSet = true;
	}*/

	std::shared_ptr<VertexShader> loadedVertexShader = std::make_shared<VertexShader>();
	loadedVertexShader->myName = name;
	loadedVertexShader->myVertexShader = vertexShader;
	loadedVertexShader->CreateInputLayout(vsData, anInputLayoutType);
	ourVertexShaderRegistry.insert(std::pair(aPath.string(), loadedVertexShader));
	return loadedVertexShader;
}

void VertexShader::CreateInputLayout(const std::string& someVertexShaderData, InputLayoutType anInputLayoutType)
{
	switch (anInputLayoutType)
	{
	case InputLayoutType::MeshVertex:
		{
			D3D11_INPUT_ELEMENT_DESC layout[] =
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
			};

			AssertIfFailed(DX11::Device->CreateInputLayout(layout, sizeof(layout) / sizeof(D3D11_INPUT_ELEMENT_DESC), someVertexShaderData.data(), someVertexShaderData.size(), myInputLayout.GetAddressOf()));
			break;
		}
	case InputLayoutType::ParticleVertex:
		{
			//TODO: Input layout for particles
			D3D11_INPUT_ELEMENT_DESC layout[] =
			{
				{"POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
				{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
				{"VELOCITY", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
				{"SCALE", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
				{"LIFETIME", 0, DXGI_FORMAT_R32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0}
			};

			AssertIfFailed(DX11::Device->CreateInputLayout(layout, sizeof(layout) / sizeof(D3D11_INPUT_ELEMENT_DESC), someVertexShaderData.data(), someVertexShaderData.size(), myInputLayout.GetAddressOf()));
			break;
		}
	default:
		{
			DEBUGERROR("Trying to create unknown Input Layout type");
			return;
		}
	}

	DEBUGLOG("Created Input Layout for Vertex Shader " + myName);
	//DX11::Context->IASetInputLayout(ourInputLayout.Get());
}