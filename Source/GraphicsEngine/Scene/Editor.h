#pragma once
#include <Math/Vector4.hpp>

class Editor
{
public:
	void Initialize(Utility::Vector4<float>& aClearColor, bool& aLerpAnimations);
	void UpdateEditorInterface(Utility::Vector4<float>& aClearColor, bool& aLerpAnimations, std::filesystem::path& aTexturePathToLoad);

private:
	void LoadProperties(bool& aLerpAnimations);
	void LoadClearColor(Utility::Vector4<float>& aClearColor, const std::filesystem::path& aPath);

	void LoadColorPopup(Utility::Vector4f& aColorToLoad);
	void SaveColorPopup(const Utility::Vector4f& aColorToSave);

	void LoadBlendPopup();
	void SaveBlendPopup();

	std::string myInputBuffer;

	Utility::Vector4<float> myPresetInput1;
	Utility::Vector4<float> myPresetInput2;
	float myBlendFactor = 0;
	bool myUseBlend = false;
};
