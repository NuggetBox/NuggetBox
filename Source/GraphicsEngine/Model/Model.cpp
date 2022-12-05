#include "NuggetBox.pch.h"
#include "Model.h"

#include <filesystem>
#include <vector>
#include <d3d11.h>

#include "Core/DebugLogger.h"
#include "TGAFBXImporter/include/FBXImporter.h"
#include "FBXStructs.h"
#include "Timer.h"

//Fixed with static inline
//std::unordered_map<std::string, std::shared_ptr<Model>> Model::ourModelRegistry;
//std::unordered_map<std::string, AnimationData> Model::ourAnimationRegistry;

Model::Model(const ModelData& someModelData)
{
	myModelData = someModelData;
}

const std::vector<Mesh>& Model::GetMeshes() const
{
	return myModelData.myMeshes;
}

void Model::SetMaterial(std::shared_ptr<Material> aMaterial)
{
	for (auto& mesh : myModelData.myMeshes)
	{
		mesh.SetMaterial(aMaterial);
	}
}

void Model::AddRenderedInstance(const Utility::Matrix4f& aTransform)
{
	myRenderedInstances.push_back({aTransform});
}

void Model::UpdateInstanceBuffer()
{
	D3D11_BUFFER_DESC instanceVertexBufferDesc;
	instanceVertexBufferDesc.ByteWidth = static_cast<UINT>(sizeof(InstanceData) * myRenderedInstances.size());
	instanceVertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	instanceVertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	instanceVertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	instanceVertexBufferDesc.MiscFlags = 0;
	instanceVertexBufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA subresourceData;
	subresourceData.pSysMem = myRenderedInstances.data();

	AssertIfFailed(DX11::Device->CreateBuffer(&instanceVertexBufferDesc, &subresourceData, myInstanceBuffer.GetAddressOf()));
}

void Model::AddSubMesh(const Mesh& aMesh)
{
	myModelData.myMeshes.push_back(aMesh);
}

std::shared_ptr<Model> Model::Load(const std::filesystem::path& aPath)
{
	std::string path = aPath.string();
	ModelData modelData;

	if (ourModelRegistry.contains(path))
	{
		DEBUGLOG("Loaded Model " + aPath.filename().string() + " from registry successfully");
		modelData = ourModelRegistry.at(path);
		return std::make_shared<Model>(modelData);
	}

	if (path == "Cube")
	{
		std::vector<VertexData> cubeVertices =
		{
			//Front face
			{ -50.0f, 50.0f, -50.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f },
			{ 50.0f, 50.0f, -50.0f, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f },
			{ 50.0f, -50.0f, -50.0f, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f },
			{ -50.0f, -50.0f, -50.0f, 0.0f, 1.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f },
			//Right face
			{ 50.0f, 50.0f, -50.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, -1.0f, 0.0f },
			{ 50.0f, 50.0f, 50.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, -1.0f, 0.0f },
			{ 50.0f, -50.0f, 50.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, -1.0f, 0.0f },
			{ 50.0f, -50.0f, -50.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, -1.0f, 0.0f },
			//Back face
			{ 50.0f, 50.0f, 50.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f },
			{ -50.0f, 50.0f, 50.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f },
			{ -50.0f, -50.0f, 50.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f },
			{ 50.0f, -50.0f, 50.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f },
			//Left face
			{ -50.0f, 50.0f, 50.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f },
			{ -50.0f, 50.0f, -50.0f, 1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f },
			{ -50.0f, -50.0f, -50.0f, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f },
			{ -50.0f, -50.0f, 50.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f },
			//Top face
			{ -50.0f, 50.0f, 50.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f },
			{ 50.0f, 50.0f, 50.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f },
			{ 50.0f, 50.0f, -50.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f },
			{ -50.0f, 50.0f, -50.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f },
			//Bottom face
			{ -50.0f, -50.0f, -50.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f },
			{ 50.0f, -50.0f, -50.0f, 1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f },
			{ 50.0f, -50.0f, 50.0f, 1.0f, 1.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f },
			{ -50.0f, -50.0f, 50.0f, 0.0f, 1.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f }
		};

		std::vector<UINT> cubeIndices =
		{
			//Front face
			0, 1, 3,
			3, 1, 2,
			//Right face
			4, 5, 7,
			7, 5, 6,
			//Back face
			8, 9, 11,
			11, 9, 10,
			//Left face
			12, 13, 15,
			15, 13, 14,
			//Top face
			16, 17, 19,
			19, 17, 18,
			//Bottom face
			20, 21, 23,
			23, 21, 22
		};

		Mesh cube(cubeVertices, cubeIndices);

		//TODO: Fix default material load
		std::shared_ptr<Material> meshMaterial = std::make_shared<Material>();
		meshMaterial->SetAlbedoTexture(Texture::LoadDefaultAlbedo());
		meshMaterial->SetNormalMap(Texture::LoadDefaultNormal());
		meshMaterial->SetSurfaceTexture(Texture::LoadDefaultSurface());

		cube.SetMaterial(meshMaterial);
		modelData.myMeshes.push_back(cube);
		DEBUGLOG("Created Primitive Cube Mesh");
	}
	else if (path == "Pyramid")
	{
		// X and Y component of a normalized 2d vector with angle 30degrees to X-axis
		const float xN = 0.894427180f;
		const float yN = 0.447213590f;

		std::vector<VertexData> pyramidVertices =
		{
			//Front face
			{0.0f, 50.0f, 0.0f, 0.5f, 0.5f, 0.0f, yN, -xN, 1.0f, 0.0f, 0.0f, 0.0f, -xN, -yN},
			{-50.0f, -50.0f, -50.0f, 0.0f, 1.0f, 0.0f, yN, -xN, 1.0f, 0.0f, 0.0f, 0.0f, -xN, -yN},
			{50.0f, -50.0f, -50.0f, 1.0f, 1.0f, 0.0f, yN, -xN, 1.0f, 0.0f, 0.0f, 0.0f, -xN, -yN},
			//Right face
			{0.0f, 50.0f, 0.0f, 0.5f, 0.5f, xN, yN, 0.0f, 0.0f, 0.0f, 1.0f, yN, -xN, 0.0f},
			{50.0f, -50.0f, -50.0f, 0.0f, 1.0f, xN, yN, 0.0f, 0.0f, 0.0f, 1.0f, yN, -xN, 0.0f},
			{50.0f, -50.0f, 50.0f, 1.0f, 1.0f, xN, yN, 0.0f, 0.0f, 0.0f, 1.0f, yN, -xN, 0.0f},
			//Back face
			{0.0f, 50.0f, 0.0f, 0.5f, 0.5f, 0.0f, yN, xN, -1.0f, 0.0f, 0.0f, 0.0f, -xN, yN},
			{50.0f, -50.0f, 50.0f, 0.0f, 1.0f, 0.0f, yN, xN, -1.0f, 0.0f, 0.0f, 0.0f, -xN, yN},
			{-50.0f, -50.0f, 50.0f, 1.0f, 1.0f, 0.0f, yN, xN, -1.0f, 0.0f, 0.0f, 0.0f, -xN, yN},
			//Left face
			{0.0f, 50.0f, 0.0f, 0.5f, 0.5f, -xN, yN, 0.0f, 0.0f, 0.0f, -1.0f, -yN, -xN, 0.0f},
			{-50.0f, -50.0f, 50.0f, 0.0f, 1.0f, -xN, yN, 0.0f, 0.0f, 0.0f, -1.0f, -yN, -xN, 0.0f},
			{-50.0f, -50.0f, -50.0f, 1.0f, 1.0f, -xN, yN, 0.0f, 0.0f, 0.0f, -1.0f, -yN, -xN, 0.0f},
			//Bottom face
			{-50.0f, -50.0f, -50.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f},
			{50.0f, -50.0f, -50.0f, 1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f},
			{50.0f, -50.0f, 50.0f, 1.0f, 1.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f},
			{-50.0f, -50.0f, 50.0f, 0.0f, 1.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f}
		};

		std::vector<unsigned int> pyramidIndices =
		{
			0, 2, 1,
			3, 5, 4,
			6, 8, 7,
			9, 11, 10,
			12, 13, 14,
			12, 14, 15
		};

		Mesh pyramid(pyramidVertices, pyramidIndices);

		//TODO: Fix default material load
		std::shared_ptr<Material> meshMaterial = std::make_shared<Material>();
		meshMaterial->SetAlbedoTexture(Texture::LoadDefaultAlbedo());
		meshMaterial->SetNormalMap(Texture::LoadDefaultNormal());
		meshMaterial->SetSurfaceTexture(Texture::LoadDefaultSurface());

		pyramid.SetMaterial(meshMaterial);
		modelData.myMeshes.push_back(pyramid);
		DEBUGLOG("Created Primitive Pyramid Mesh");
	}
	else if (path == "Plane")
	{
		std::vector<VertexData> quadVertices =
		{
			//Top face
			{-50.0f, 0.0f, 50.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f },
			{50.0f, 0.0f, 50.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f },
			{-50.0f, 0.0f, -50.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f },
			{50.0f, 0.0f, -50.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f }
		};

		std::vector<unsigned int> quadIndices =
		{
			0, 1, 2,
			1, 3, 2
		};

		Mesh plane(quadVertices, quadIndices);

		//TODO: Fix default material load
		std::shared_ptr<Material> meshMaterial = std::make_shared<Material>();
		meshMaterial->SetAlbedoTexture(Texture::LoadDefaultAlbedo());
		meshMaterial->SetNormalMap(Texture::LoadDefaultNormal());
		meshMaterial->SetSurfaceTexture(Texture::LoadDefaultSurface());

		plane.SetMaterial(meshMaterial);
		modelData.myMeshes.push_back(plane);
		DEBUGLOG("Created Primitive Plane Mesh");
	}
	else if (path == "Cylinder")
	{
		constexpr float height = 100.0f;
		constexpr float radius = 50.0f;
		constexpr int resolution = 80;

		constexpr float anglePerVertex = PI * 2 / resolution;
		constexpr int vertexCount = 6 * resolution + 2;

		std::vector<VertexData> cylinderVertices;
		cylinderVertices.reserve(vertexCount);

		//Middle Vertex of Top Face, INDEX = 0
		cylinderVertices.emplace_back(0.0f, height, 0.0f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f);

		//Add top outer vertices, count = res, INDEX = 1 -> resolution
		//First vertex is at -z towards camera
		//Added counterclockwise seen from above
		for (int i = 0; i < resolution; ++i)
		{
			float angle = anglePerVertex * i;
			float relativeAngle = angle - PI / 2.0f;
			float x = cos(relativeAngle);
			float z = sin(relativeAngle);
			float u = (x + 1.0f) / 2.0f;
			float v = (z * -1.0f + 1.0f) / 2.0f;

			cylinderVertices.emplace_back(x * radius, height, z * radius, u, v, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f);
		}

		//Middle Vertex of Bottom Face, INDEX = resolution + 1
		cylinderVertices.emplace_back(0.0f, 0.0f, 0.0f, 0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f);

		//Add bottom outer vertices, count = res, INDEX = resolution + 2 -> 2 x resolution + 1
		//First vertex is at -z towards camera
		//Added counterclockwise seen from above
		for (int i = 0; i < resolution; ++i)
		{
			float angle = anglePerVertex * i;
			float relativeAngle = angle - PI / 2.0f;
			float x = cos(relativeAngle);
			float z = sin(relativeAngle);
			float u = (x + 1.0f) / 2.0f;
			float v = (z + 1.0f) / 2.0f;

			cylinderVertices.emplace_back(x * radius, 0.0f, z * radius, u, v, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f);
		}

		//Add middle part vertices, count = 4 x res, INDEX = 2 x resolution + 2: i < resolution, (i * 4) + 0 -> 3
		//Create one box for every loop
		for (int i = 0; i < resolution; ++i)
		{
			float angle1 = anglePerVertex * i;
			float relativeAngle1 = angle1 - PI / 2.0f;
			float x1 = cos(relativeAngle1);
			float z1 = sin(relativeAngle1);

			float angle2 = anglePerVertex * (i + 1);
			float relativeAngle2 = angle2 - PI / 2.0f;
			float x2 = cos(relativeAngle2);
			float z2 = sin(relativeAngle2);

			float u1 = (static_cast<float>(i) / static_cast<float>(resolution));
			float u2 = (static_cast<float>(i + 1) / static_cast<float>(resolution));

			float normalAngle = (relativeAngle1 + relativeAngle2) / 2.0f;
			Utility::Vector2f normal(cos(normalAngle), sin(normalAngle));
			normal.Normalize();
			Utility::Vector2f tangent(cos(normalAngle + PI / 2.0f), sin(normalAngle + PI / 2.0f));
			tangent.Normalize();

			cylinderVertices.emplace_back(x1 * radius, height, z1 * radius, u1, 0.0f, normal.x, 0.0f, normal.y, tangent.x, 0.0f, tangent.y, 0.0f, -1.0f, 0.0f);
			cylinderVertices.emplace_back(x2 * radius, height, z2 * radius, u2, 0.0f, normal.x, 0.0f, normal.y, tangent.x, 0.0f, tangent.y, 0.0f, -1.0f, 0.0f);
			cylinderVertices.emplace_back(x2 * radius, 0.0f, z2 * radius, u2, 1.0f, normal.x, 0.0f, normal.y, tangent.x, 0.0f, tangent.y, 0.0f, -1.0f, 0.0f);
			cylinderVertices.emplace_back(x1 * radius, 0.0f, z1 * radius, u1, 1.0f, normal.x, 0.0f, normal.y, tangent.x, 0.0f, tangent.y, 0.0f, -1.0f, 0.0f);
		}

		std::vector<UINT> cylinderIndices;
		cylinderIndices.reserve(vertexCount);

		//Top vertices
		for (int i = 0; i < resolution - 1; ++i)
		{
			cylinderIndices.push_back(i + 1);
			cylinderIndices.push_back(0);
			cylinderIndices.push_back(i + 2);
		}

		//Add the last triangle that connects to the first outer vertex
		cylinderIndices.push_back(resolution);
		cylinderIndices.push_back(0);
		cylinderIndices.push_back(1);

		//Bottom vertices, reverse winding order because upside down
		for (int i = resolution + 1; i < 2 * resolution; ++i)
		{
			cylinderIndices.push_back(i + 1);
			cylinderIndices.push_back(i + 2);
			cylinderIndices.push_back(resolution + 1);
		}

		//Add the last triangle that connects to the first outer vertex
		cylinderIndices.push_back(2 * resolution + 1);
		cylinderIndices.push_back(resolution + 2);
		cylinderIndices.push_back(resolution + 1);

		//Middle vertices
		for (int i = 0; i < resolution; ++i)
		{
			int index = 2 * resolution + 2 + i * 4;

			cylinderIndices.push_back(index);
			cylinderIndices.push_back(index + 1);
			cylinderIndices.push_back(index + 2);

			cylinderIndices.push_back(index);
			cylinderIndices.push_back(index + 2);
			cylinderIndices.push_back(index + 3);
		}

		Mesh cylinder(cylinderVertices, cylinderIndices);

		//TODO: Fix default material load
		std::shared_ptr<Material> meshMaterial = std::make_shared<Material>();
		meshMaterial->SetAlbedoTexture(Texture::LoadDefaultAlbedo());
		meshMaterial->SetNormalMap(Texture::LoadDefaultNormal());
		meshMaterial->SetSurfaceTexture(Texture::LoadDefaultSurface());

		cylinder.SetMaterial(meshMaterial);
		modelData.myMeshes.push_back(cylinder);
		DEBUGLOG("Created Primitive Cylinder Mesh");
	}
	else
	{
		TGA::FBXModel tgaModel;
		assert(TGA::FBXImporter::LoadModel(path, tgaModel));

		//Load meshes
		for (auto& loadedMesh : tgaModel.Meshes)
		{
			std::vector<VertexData> vertices;
			vertices.resize(loadedMesh.Vertices.size());

			std::vector<unsigned int> indices;
			indices.reserve(loadedMesh.Indices.size());

			//Load vertices
			for (int i = 0; i < loadedMesh.Vertices.size(); ++i)
			{
				//Copy vertex positions
				memcpy_s(&vertices[i], sizeof(Utility::Vector4<float>), &loadedMesh.Vertices[i].Position, sizeof(float[4]));

				//Copy vertex colors
				for (int verCol = 0; verCol < 4; ++verCol)
				{
					memcpy_s(&vertices[i].VertexColors[verCol], sizeof(float[4]), &loadedMesh.Vertices[i].VertexColors[verCol], sizeof(float[4]));
				}

				//Copy UVs
				memcpy_s(&vertices[i].UVs, sizeof(float[4][2]), &loadedMesh.Vertices[i].UVs, sizeof(float[4][2]));

				//Copy Normal
				memcpy_s(&vertices[i].Normal, sizeof(float[3]), &loadedMesh.Vertices[i].Normal, sizeof(float[3]));

				//Copy Tangent
				memcpy_s(&vertices[i].Tangent, sizeof(float[3]), &loadedMesh.Vertices[i].Tangent, sizeof(float[3]));

				//Copy Binormal
				memcpy_s(&vertices[i].Binormal, sizeof(float[3]), &loadedMesh.Vertices[i].Binormal, sizeof(float[3]));

				//Copy BoneIDs
				memcpy_s(&vertices[i].BoneIDs, sizeof(unsigned int[4]), &loadedMesh.Vertices[i].BoneIDs, sizeof(unsigned int[4]));

				//Copy bone weights
				memcpy_s(&vertices[i].BoneWeights, sizeof(float[4]), &loadedMesh.Vertices[i].BoneWeights, sizeof(float[4]));

				// TODO: Remove random vertex colors when they will be used
				for (int col = 0; col < 4; col++)
				{
					vertices[i].VertexColors[0][col] = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
				}
			}

			//Copy vertex indices
			indices = loadedMesh.Indices;

			//TODO: Refactor material loading, if not found, use default
			//size_t slash = path.rfind('/');
			//size_t dot = path.rfind('.');
			//const std::string baseFileName = path.substr(slash + 1, dot - slash - 1);

			const std::string baseFileName = aPath.stem().string();
			const std::string albedoFileName = "T_" + baseFileName + "_C.dds";
			const std::string normalFileName = "T_" + baseFileName + "_N.dds";
			const std::string surfaceFileName = "T_" + baseFileName + "_M.dds";

			//TODO: Get material name from .fbx file
			//loadedMesh.MaterialName

			//Load Material
			std::shared_ptr<Material> meshMaterial = std::make_shared<Material>();
			meshMaterial->SetAlbedoTexture(Texture::Load("assets/Textures/" + albedoFileName));
			meshMaterial->SetNormalMap(Texture::Load("assets/Textures/" + normalFileName));
			meshMaterial->SetSurfaceTexture(Texture::Load("assets/Textures/" + surfaceFileName));

			Mesh mesh(vertices, indices);
			mesh.SetMaterial(meshMaterial);
			modelData.myMeshes.push_back(mesh);
		}

		//Load skeleton
		if (tgaModel.Skeleton.GetRoot() != nullptr)
		{
			SkeletonData skeletonData;
			//skeleton.Name = tgaModel.Skeleton.Name;
			skeletonData.BoneNameToIndex.reserve(tgaModel.Skeleton.Joints.size());

			skeletonData.Bones.resize(tgaModel.Skeleton.Joints.size());
			for (size_t i = 0; i < tgaModel.Skeleton.Joints.size(); ++i)
			{
				skeletonData.Bones[i].Name = tgaModel.Skeleton.Joints[i].Name;

#ifdef _DEBUG
				for (int j = 0; j < i; ++j)
				{
					if (skeletonData.Bones[j].Name == tgaModel.Skeleton.Joints[i].Name)
					{
						DEBUGWARNING("Bone with name " + skeletonData.Bones[j].Name + " was just added but it has the same name as a bone that was already added!");
					}
				}
#endif

				skeletonData.BoneNameToIndex.insert(std::pair(tgaModel.Skeleton.Joints[i].Name, i));

				skeletonData.Bones[i].Parent = tgaModel.Skeleton.Joints[i].Parent;
				memcpy_s(&skeletonData.Bones[i].BindPoseInverse, sizeof(Utility::Matrix4f), &tgaModel.Skeleton.Joints[i].BindPoseInverse, sizeof(TGA::Matrix));

				/*skeleton.Bones[i].Children.resize(tgaModel.Skeleton.Joints[i].Children.size());
				for (size_t c = 0; c < tgaModel.Skeleton.Joints[i].Children.size(); ++c)
				{
					skeleton.Bones[i].Children[c] = tgaModel.Skeleton.Joints[i].Children[c];
				}*/
				skeletonData.Bones[i].Children = tgaModel.Skeleton.Joints[i].Children;
			}

			modelData.mySkeleton = std::make_shared<Skeleton>(skeletonData);
			DEBUGLOG("Loaded Skeleton");
		}
	}

	DEBUGLOG("Loaded Model " + aPath.filename().string());
	ourModelRegistry.insert(std::pair(path, modelData));
	return std::make_shared<Model>(modelData);
}

void Model::LoadAnimation(const std::filesystem::path& aPath, const std::string& aNewAnimationName)
{
	assert(HasSkeleton());
	std::shared_ptr<Animation> animation = Animation::Load(aPath, aNewAnimationName, myModelData.mySkeleton);
	myAnimations.insert(std::pair(aNewAnimationName, animation));
}

void Model::PlayAnimation(const std::string& anAnimationName)
{
	myCurrentAnim = anAnimationName;
}

void Model::Update(bool aLerpAnimations)
{
	//TODO: Animation state check == Playing

	if (HasAnimations())
	{
		const std::shared_ptr<Animation>& currentAnim = myAnimations[myCurrentAnim];

		myAnimationTimer += Utility::Timer::GetDeltaTime();

		if (myAnimationTimer > currentAnim->GetDuration())
		{
			myAnimationTimer = 0;
		}

		float wholePart;
		float decimalPart = modf(myAnimationTimer * currentAnim->GetFPS(), &wholePart);

		size_t currentFrame = static_cast<size_t>(wholePart);
		size_t nextFrame = currentFrame == currentAnim->GetFrames().size() - 1 ? 1 : currentFrame + 1;
		float lerpFactor = decimalPart;

		if (aLerpAnimations)
		{
			LerpAnimationHierarchy(currentFrame, nextFrame, 0, Utility::Matrix4f(), &myBoneTransforms[0], lerpFactor);
		}
		else
		{
			UpdateAnimationHierarchy(currentFrame, nextFrame, 0, Utility::Matrix4f(), &myBoneTransforms[0]);
		}
	}
}

void Model::SetSkeleton(const SkeletonData& aSkeleton)
{
	myModelData.mySkeleton = std::make_shared<Skeleton>(aSkeleton);
}

bool Model::HasAnimations() const
{
	return HasSkeleton() && !myAnimations.empty();
}

bool Model::HasSkeleton() const
{
	return myModelData.mySkeleton != nullptr;
}

bool Model::HasBones() const
{
	return HasSkeleton() && myModelData.mySkeleton->GetRoot() != nullptr;
}

void Model::UpdateAnimationHierarchy(unsigned aCurrentFrame, unsigned aNextFrame, unsigned aBoneIndex, const Utility::Matrix4f& aParentTransform, Utility::Matrix4f* outBoneTransforms)
{
	const BoneData& currentBone = myModelData.mySkeleton->GetBones()[aBoneIndex];
	const Utility::Matrix4f currentBoneLocalTransform = myAnimations[myCurrentAnim]->GetFrames()[aCurrentFrame].LocalTransforms[aBoneIndex];
	const Utility::Matrix4f currentBoneGlobalTransform = aParentTransform * currentBoneLocalTransform;

	Utility::Matrix4f finalBoneTransform;
	finalBoneTransform *= currentBoneGlobalTransform; // Cursed: finalBoneTransform *= currentBoneLocalTransform;
	finalBoneTransform *= currentBone.BindPoseInverse;

	outBoneTransforms[aBoneIndex] = finalBoneTransform;

	for (unsigned child : currentBone.Children)
	{
		UpdateAnimationHierarchy(aCurrentFrame, aNextFrame, child, currentBoneGlobalTransform, outBoneTransforms);
	}
}

void Model::LerpAnimationHierarchy(unsigned aCurrentFrame, unsigned aNextFrame, unsigned aBoneIndex,
	const Utility::Matrix4f& aParentTransform, Utility::Matrix4f* outBoneTransforms, float aLerpFactor)
{
	const BoneData& currentBone = myModelData.mySkeleton->GetBones()[aBoneIndex];
	const Utility::Matrix4f currentBoneLocalTransform = myAnimations[myCurrentAnim]->GetFrames()[aCurrentFrame].LocalTransforms[aBoneIndex];
	const Utility::Matrix4f currentBoneGlobalTransform = aParentTransform * currentBoneLocalTransform;

	const Utility::Matrix4f currentBoneLocalTransformNextFrame = myAnimations[myCurrentAnim]->GetFrames()[aNextFrame].LocalTransforms[aBoneIndex];
	const Utility::Matrix4f currentBoneGlobalTransformNextFrame = aParentTransform * currentBoneLocalTransformNextFrame;

	Utility::Matrix4f finalBoneTransform = Utility::Matrix4f::Lerp(currentBoneGlobalTransform, currentBoneGlobalTransformNextFrame, aLerpFactor);

	for (unsigned child : currentBone.Children)
	{
		LerpAnimationHierarchy(aCurrentFrame, aNextFrame, child, finalBoneTransform, outBoneTransforms, aLerpFactor);
	}

	finalBoneTransform *= currentBone.BindPoseInverse;
	outBoneTransforms[aBoneIndex] = finalBoneTransform;
}