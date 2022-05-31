#pragma once
#include <vector>
#include <unordered_map>

#include "Math/Matrix4x4.hpp"

struct VertexData
{
	VertexData() = default;

	//Pos, VertexColor0
	VertexData(float aX, float aY, float aZ, float aR, float aG, float aB, float aA)
	{
		Position[0] = aX;
		Position[1] = aY;
		Position[2] = aZ;
		//Position[3] = 1;

		VertexColors[0][0] = aR;
		VertexColors[0][1] = aG;
		VertexColors[0][2] = aB;
		VertexColors[0][3] = aA;
	}

	//Pos, VertexColor0, UV0
	VertexData(float aX, float aY, float aZ, float aR, float aG, float aB, float aA, float aU, float aV)
	{
		Position[0] = aX;
		Position[1] = aY;
		Position[2] = aZ;
		//Position[3] = 1;

		VertexColors[0][0] = aR;
		VertexColors[0][1] = aG;
		VertexColors[0][2] = aB;
		VertexColors[0][3] = aA;

		UVs[0][0] = aU;
		UVs[0][1] = aV;
	}

	//Pos, UV0, Normal, Tangent, Binormal
	VertexData(float aX, float aY, float aZ, float aU, float aV, float aNX, float aNY, float aNZ, float aTX, float aTY, float aTZ, float aBX, float aBY, float aBZ)
	{
		Position[0] = aX;
		Position[1] = aY;
		Position[2] = aZ;
		//Position[3] = 1;

		UVs[0][0] = aU;
		UVs[0][1] = aV;

		Normal[0] = aNX;
		Normal[1] = aNY;
		Normal[2] = aNZ;

		Tangent[0] = aTX;
		Tangent[1] = aTY;
		Tangent[2] = aTZ;

		Binormal[0] = aBX;
		Binormal[1] = aBY;
		Binormal[2] = aBZ;
	}

	float Position[4] = { 0, 0, 0, 1 };
	float VertexColors[4][4]
	{
		{0, 0, 0, 0},
		{0, 0, 0, 0},
		{0, 0, 0, 0},
		{0, 0, 0, 0}
	};

	float UVs[4][2]
	{
		{0, 0},
		{0, 0},
		{0, 0},
		{0, 0}
	};

	float Normal[3] = { 0, 0, 0 };
	float Tangent[3] = { 0, 0, 0 };
	float Binormal[3] = { 0, 0, 0 };

	unsigned int BoneIDs[4] = { 0, 0, 0, 0 };
	float BoneWeights[4] = { 0, 0, 0, 0 };
};

struct FrameData
{
	std::vector<Utility::Matrix4f> LocalTransforms;
};

struct AnimationData
{
	std::string Name;
	//In frames
	unsigned int Length;
	//In seconds
	float Duration;
	float FramesPerSecond;
	std::vector<FrameData> Frames;
};

struct BoneData
{
	std::string Name;
	Utility::Matrix4f BindPoseInverse;
	int Parent;
	std::vector<unsigned int> Children;
};

struct SkeletonData
{
	std::string Name;
	std::vector<BoneData> Bones;
	std::unordered_map<std::string, size_t> BoneNameToIndex;
	/*std::unordered_map<std::string, AnimationData> Animations;

	std::vector<std::string> GetBoneNames()
	{
		std::vector<std::string> boneNames;
		boneNames.reserve(Bones.size());

		for (auto& bone : Bones)
		{
			boneNames.push_back(bone.Name);
		}

		return boneNames;
	}

	const BoneData* GetRoot() const
	{
		if (!Bones.empty())
		{
			return &Bones[0];
		}

		return nullptr;
	}*/
};
