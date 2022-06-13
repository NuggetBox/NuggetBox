#include "NuggetBox.pch.h"
#include "Animation.h"

#include <cassert>
#include <filesystem>

#include "TGAFBXImporter/FBXImporter.h"
#include "TGAFBXImporter/FBXImporterStructs.h"
#include "Skeleton.h"

//std::unordered_map<std::string, AnimationData> Animation::ourAnimationRegistry;

Animation::Animation(const AnimationData& someAnimationData)
{
	myAnimationData = someAnimationData;
}

std::shared_ptr<Animation> Animation::Load(const std::filesystem::path& aPath, const std::string& aNewAnimationName, std::shared_ptr<Skeleton> aSkeleton)
{
	if (ourAnimationRegistry.contains(aNewAnimationName))
	{
		return std::make_shared<Animation>(ourAnimationRegistry.at(aNewAnimationName));
	}

	TGA::FBXAnimation loadedAnimation;
	assert(TGA::FBXImporter::LoadAnimation(aPath.string(), aSkeleton->GetBoneNames(), loadedAnimation));

	AnimationData animationData;
	animationData.Name = loadedAnimation.Name;
	animationData.Length = loadedAnimation.Length;
	animationData.Duration = static_cast<float>(loadedAnimation.Duration);
	animationData.FramesPerSecond = loadedAnimation.FramesPerSecond;

	animationData.Frames.resize(loadedAnimation.Frames.size());
	for (size_t f = 0; f < loadedAnimation.Frames.size(); ++f)
	{
		animationData.Frames[f].LocalTransforms.resize(loadedAnimation.Frames[f].LocalTransforms.size());
		for (size_t t = 0; t < loadedAnimation.Frames[f].LocalTransforms.size(); ++t)
		{
			memcpy_s(&animationData.Frames[f].LocalTransforms[t], sizeof(Utility::Matrix4f), &loadedAnimation.Frames[f].LocalTransforms[t], sizeof(TGA::Matrix));
			animationData.Frames[f].LocalTransforms[t] = Utility::Matrix4f::Transpose(animationData.Frames[f].LocalTransforms[t]);
		}
	}

	//TODO: Map with path or newAnimationName as key?
	ourAnimationRegistry.insert(std::pair(aNewAnimationName, animationData));
	return std::make_shared<Animation>(animationData);
}