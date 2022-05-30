#include "NuggetBox.pch.h"
#include <filesystem>
#include <vector>
#include <d3d11.h>

#include "Model.h"
#include "TGAFBXImporter/FBXImporter.h"
#include "FBXStructs.h"
#include "Timer.h"

std::unordered_map<std::string, std::shared_ptr<Model>> Model::ourModelRegistry;
//std::unordered_map<std::string, AnimationData> Model::ourAnimationRegistry;

const std::vector<Mesh>& Model::GetMeshes() const
{
	return myMeshes;
}

void Model::AddSubMesh(const Mesh& aMesh)
{
	myMeshes.push_back(aMesh);
}

std::shared_ptr<Model> Model::Load(const std::filesystem::path& aPath)
{
	std::string path = aPath.string();

	if (ourModelRegistry.contains(path))
	{
		return ourModelRegistry.at(path);
	}

	std::shared_ptr<Model> model = std::make_shared<Model>();

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

		/*cubeVertices.reserve(8);
		cubeVertices.emplace_back(-50.f, -50.f, -50.f, 1, 0, 0, 1, 0, 1);
		cubeVertices.emplace_back(-50.f, 50.f, -50.f, 0, 1, 0, 1, 0, 0);
		cubeVertices.emplace_back(50.f, 50.f, -50.f, 0, 0, 1, 1, 1, 0);
		cubeVertices.emplace_back(50.f, -50.f, -50.f, 1, 0.5f, 1, 1, 1, 1);
		cubeVertices.emplace_back(50.f, -50.f, 50.f, 0.5f, 1, 0, 1, 0, 1);
		cubeVertices.emplace_back(50.f, 50.f, 50.f, 1, 0, 0.5f, 1, 0, 0);
		cubeVertices.emplace_back(-50.f, 50.f, 50.f, 1, 0.5f, 0, 1, 1, 0);
		cubeVertices.emplace_back(-50.f, -50.f, 50.f, 0.5f, 0, 1, 1, 1, 1);

		std::vector<unsigned int> cubeIndices =
		{
			0, 1, 3,
			2, 3, 1,
			3, 2, 4,
			5, 4, 2,
			4, 5, 7,
			6, 7, 5,
			0, 7, 1,
			6, 1, 7,
			1, 6, 2,
			5, 2, 6,
			0, 3, 7,
			4, 7, 3
		};*/

		Mesh cube(cubeVertices, cubeIndices);
		//TODO: Fix default material load
		std::shared_ptr<Material> material = std::make_shared<Material>();
		material->SetAlbedoTexture(Texture::Load("BLABLA"));
		cube.SetMaterial(material);
		model->AddSubMesh(cube);
	}
	else if (path == "Pyramid")
	{
		std::vector<VertexData> pyramidVertices;

		pyramidVertices.reserve(5);
		pyramidVertices.emplace_back(0.0f, 50.f, 0.0f, 1, 0, 0, 1, 1, 1);
		pyramidVertices.emplace_back(-50.f, -50.f, 50.f, 0, 1, 0, 1, 1, 0);
		pyramidVertices.emplace_back(50.f, -50.f, 50.f, 0, 0, 1, 1, 1, 0);
		pyramidVertices.emplace_back(50.f, -50.f, -50.f, 1, 0.5f, 1, 1, 1, 0);
		pyramidVertices.emplace_back(-50.f, -50.f, -50.f, 0.5f, 1, 0, 1, 1, 0);

		std::vector<unsigned int> pyramidIndices =
		{
			0, 3, 4,
			0, 2, 3,
			0, 1, 2,
			0, 4, 1,
			4, 3, 2,
			2, 1, 4
		};

		Mesh pyramid(pyramidVertices, pyramidIndices);
		std::shared_ptr<Material> material = std::make_shared<Material>();
		material->SetAlbedoTexture(Texture::Load("BLABLA"));
		pyramid.SetMaterial(material);
		model->AddSubMesh(pyramid);
	}
	else
	{
		TGA::FBXModel tgaModel;
		assert(TGA::FBXImporter::LoadModel(path, tgaModel));

		//std::vector<Mesh> meshes;
		//meshes.reserve(tgaModel.Meshes.size());

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
				memcpy_s(&vertices[i], sizeof(Vector4<float>), &loadedMesh.Vertices[i].Position, sizeof(float[4]));

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

				// TODO: Remove random vertex colors if needed
				for (int col = 0; col < 4; col++)
				{
					vertices[i].VertexColors[0][col] = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
				}
			}

			//Copy vertex indices
			indices = loadedMesh.Indices;

			//TODO: String tools
			std::string path = aPath.string();
			size_t slash = path.rfind('/');
			size_t dot = path.rfind('.');
			const std::string baseFileName = path.substr(slash + 1, dot - slash - 1);
			const std::string albedoFileName = "T_" + baseFileName + "_C.dds";

			//Load Material
			std::shared_ptr<Material> meshMaterial = std::make_shared<Material>();
			meshMaterial->SetAlbedoTexture(Texture::Load("Textures/" + albedoFileName));

			Mesh mesh(vertices, indices);
			mesh.SetMaterial(meshMaterial);
			model->AddSubMesh(mesh);
		}

		//Load skeleton
		if (tgaModel.Skeleton.GetRoot() != nullptr)
		{
			SkeletonData skeleton;
			skeleton.Name = tgaModel.Skeleton.Name;
			skeleton.BoneNameToIndex.reserve(tgaModel.Skeleton.Joints.size());

			skeleton.Bones.resize(tgaModel.Skeleton.Joints.size());
			for (size_t i = 0; i < tgaModel.Skeleton.Joints.size(); ++i)
			{
				skeleton.Bones[i].Name = tgaModel.Skeleton.Joints[i].Name;

#ifdef _DEBUG
				for (int j = 0; j < i; ++j)
				{
					if (skeleton.Bones[j].Name == tgaModel.Skeleton.Joints[i].Name)
					{
						//TODO: The bone that was just added has the same name as a bone that has already been added
					}
				}
#endif

				skeleton.BoneNameToIndex.insert(std::pair(tgaModel.Skeleton.Joints[i].Name, i));

				skeleton.Bones[i].Parent = tgaModel.Skeleton.Joints[i].Parent;
				memcpy_s(&skeleton.Bones[i].BindPoseInverse, sizeof(Matrix4f), &tgaModel.Skeleton.Joints[i].BindPoseInverse, sizeof(TGA::Matrix));

				/*skeleton.Bones[i].Children.resize(tgaModel.Skeleton.Joints[i].Children.size());
				for (size_t c = 0; c < tgaModel.Skeleton.Joints[i].Children.size(); ++c)
				{
					skeleton.Bones[i].Children[c] = tgaModel.Skeleton.Joints[i].Children[c];
				}*/
				skeleton.Bones[i].Children = tgaModel.Skeleton.Joints[i].Children;
			}

			model->SetSkeleton(skeleton);
		}
	}

	ourModelRegistry.insert(std::pair(path, model));
	return model;
}

void Model::LoadAnimation(const std::filesystem::path& aFilepath, const std::string& aNewAnimationName)
{
	/*if (ourAnimationRegistry.contains(aFilepath.string()))
	{
		return ourAnimationRegistry.at(aFilepath.string());
	}*/

	if (HasSkeleton())
	{
		TGA::FBXAnimation loadedAnimation;
		assert(TGA::FBXImporter::LoadAnimation(aFilepath.string(), mySkeleton->GetBoneNames(), loadedAnimation));

		AnimationData animation;
		animation.Name = loadedAnimation.Name;
		animation.Length = loadedAnimation.Length;
		animation.Duration = static_cast<float>(loadedAnimation.Duration);
		animation.FramesPerSecond = loadedAnimation.FramesPerSecond;

		animation.Frames.resize(loadedAnimation.Frames.size());
		for (size_t f = 0; f < loadedAnimation.Frames.size(); ++f)
		{
			animation.Frames[f].LocalTransforms.resize(loadedAnimation.Frames[f].LocalTransforms.size());
			for (size_t t = 0; t < loadedAnimation.Frames[f].LocalTransforms.size(); ++t)
			{
				memcpy_s(&animation.Frames[f].LocalTransforms[t], sizeof(Matrix4f), &loadedAnimation.Frames[f].LocalTransforms[t], sizeof(TGA::Matrix));
				animation.Frames[f].LocalTransforms[t] = Matrix4f::Transpose(animation.Frames[f].LocalTransforms[t]);
			}
		}

		mySkeleton->Animations.insert(std::pair(aNewAnimationName, animation));
	}
}

void Model::PlayAnimation(const std::string& anAnimationName)
{
	myCurrentAnim = anAnimationName;
}

void Model::SetSkeleton(const SkeletonData& aSkeleton)
{
	mySkeleton = std::make_shared<SkeletonData>(aSkeleton);
}

bool Model::HasSkeleton() const
{
	return mySkeleton != nullptr;
}

void Model::Update()
{
	//TODO: Animation state check == Playing

	if (HasSkeleton())
	{
		//TODO: Refactor animation stepping code
		myAnimationTimer += Timer::GetDeltaTime();

		const AnimationData& currentAnim = mySkeleton->Animations[myCurrentAnim];

		while (myAnimationTimer > currentAnim.Duration)
		{
			myAnimationTimer -= currentAnim.Duration;
		}

		size_t currentFrame = myAnimationTimer * currentAnim.FramesPerSecond;

		const size_t nextFrame = 0;
		UpdateAnimationHierarchy(currentFrame, nextFrame, 0, Matrix4f(), &myBoneTransforms[0]);
	}
}

bool Model::HasBones() const
{
	return HasSkeleton() && mySkeleton->GetRoot() != nullptr;
}

void Model::UpdateAnimationHierarchy(unsigned aCurrentFrame, unsigned aNextFrame, unsigned aBoneIndex, const Matrix4f& aParentTransform, Matrix4f* outBoneTransforms)
{
	const BoneData& currentBone = mySkeleton->Bones[aBoneIndex];
	const Matrix4f currentBoneLocalTransform = mySkeleton->Animations[myCurrentAnim].Frames[aCurrentFrame].LocalTransforms[aBoneIndex];
	const Matrix4f currentBoneGlobalTransform = aParentTransform * currentBoneLocalTransform;

	Matrix4f finalBoneTransform;
	finalBoneTransform *= currentBoneGlobalTransform; // Cursed: finalBoneTransform *= currentBoneLocalTransform;
	finalBoneTransform *= currentBone.BindPoseInverse;

	outBoneTransforms[aBoneIndex] = finalBoneTransform;

	for (size_t c = 0; c < currentBone.Children.size(); ++c)
	{
		UpdateAnimationHierarchy(aCurrentFrame, aNextFrame, currentBone.Children[c], currentBoneGlobalTransform, outBoneTransforms);
	}
}