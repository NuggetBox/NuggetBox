#pragma once
#include <unordered_map>

#include "Model/FBXStructs.h"

class Skeleton;

class Animation
{
public:
	Animation() = default;
	Animation(const AnimationData& someAnimationData);

	static std::shared_ptr<Animation> Load(const std::filesystem::path& aPath, const std::string& aNewAnimationName, std::shared_ptr<Skeleton> aSkeleton);

	const std::string& GetName() const { return myAnimationData.Name; }
	unsigned int GetLength() const { return myAnimationData.Length; }
	float GetDuration() const { return myAnimationData.Duration; }
	float GetFPS() const { return myAnimationData.FramesPerSecond; }
	const std::vector<FrameData>& GetFrames() const { return myAnimationData.Frames; }

private:
	static inline std::unordered_map<std::string, AnimationData> ourAnimationRegistry;

	AnimationData myAnimationData;
};