#pragma once
#include "FBXStructs.h"

class Skeleton
{
public:
	Skeleton() = default;
	Skeleton(const SkeletonData& someSkeletonData);
	const std::vector<BoneData>& GetBones() const { return mySkeletonData.Bones; }
	std::vector<std::string> GetBoneNames() const;
	const BoneData* GetRoot() const;
	bool HasBones() const;

private:

	SkeletonData mySkeletonData;
};