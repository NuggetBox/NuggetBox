#include "NuggetBox.pch.h"
#include "Skeleton.h"

Skeleton::Skeleton(const SkeletonData& someSkeletonData)
{
	mySkeletonData = someSkeletonData;
}

//TODO: Cache bone names
const std::vector<std::string>& Skeleton::GetBoneNames() const
{
	std::vector<std::string> boneNames;
	boneNames.reserve(mySkeletonData.Bones.size());

	for (auto& bone : mySkeletonData.Bones)
	{
		boneNames.push_back(bone.Name);
	}

	return boneNames;
}

const BoneData* Skeleton::GetRoot() const
{
	if (!mySkeletonData.Bones.empty())
	{
		return &mySkeletonData.Bones[0];
	}

	return nullptr;
}

bool Skeleton::HasBones() const
{
	return !mySkeletonData.Bones.empty();
}