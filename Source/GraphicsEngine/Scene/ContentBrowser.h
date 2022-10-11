#pragma once
#include "Model/Texture.h"

class ContentBrowser
{
public:
	ContentBrowser() = default;

	static void Initialize();
	static void Update();
	static void HandleDragDrop(const std::filesystem::path& aDroppedFile);

private:
	static inline std::filesystem::path myRoot;
	static inline std::filesystem::path myCurrentPath;
	static inline std::shared_ptr<Texture> myDirectoryIcon;
	static inline std::shared_ptr<Texture> myFileIcon;
	static inline std::shared_ptr<Texture> myBackButton;
};
