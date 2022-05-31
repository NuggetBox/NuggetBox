#pragma once
#include <unordered_map>

#include "FBXStructs.h"

class Skeleton;

class Animation
{
public:
	Animation() = default;

	static std::shared_ptr<Animation> Load(const std::filesystem::path& aPath, const std::string& aNewAnimationName, const std::shared_ptr<Skeleton>& aSkeleton);

private:
	static inline std::unordered_map<std::string, AnimationData> ourAnimationRegistry;

	AnimationData myAnimationData;
};