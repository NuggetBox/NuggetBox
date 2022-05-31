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
	TGA::FBXAnimation loadedAnimation;
	assert(TGA::FBXImporter::LoadAnimation(aPath.string(), aSkeleton->GetBoneNames(), loadedAnimation));

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
			memcpy_s(&animation.Frames[f].LocalTransforms[t], sizeof(Utility::Matrix4f), &loadedAnimation.Frames[f].LocalTransforms[t], sizeof(TGA::Matrix));
			animation.Frames[f].LocalTransforms[t] = Utility::Matrix4f::Transpose(animation.Frames[f].LocalTransforms[t]);
		}
	}

	ourAnimationRegistry.insert(std::pair(aNewAnimationName, animation));
	return std::make_shared<Animation>(animation);
}