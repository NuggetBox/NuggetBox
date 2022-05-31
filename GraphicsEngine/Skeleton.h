#pragma once
#include "FBXStructs.h"

class Skeleton
{
public:
	Skeleton(const SkeletonData& someSkeletonData);

	const std::vector<std::string>& GetBoneNames() const;
	const BoneData* GetRoot() const;
	bool HasBones() const;

private:
	SkeletonData mySkeletonData;
};