#pragma once
#include "Model/Texture.h"

class TextureDropTarget
{
public:
	TextureDropTarget() = default;

	static void Initialize();
	static void Update(std::filesystem::path& aTextureToLoad);

private:
	static inline std::shared_ptr<Texture> mySpiderCatIcon;
};
