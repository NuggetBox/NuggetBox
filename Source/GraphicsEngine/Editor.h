#pragma once
#include <Math/Vector4.hpp>

class Editor
{
public:
	void Initialize(Utility::Vector4<float>& aClearColor, bool& aLerpAnimations);
	void UpdateEditorInterface(Utility::Vector4<float>& aClearColor, bool& aLerpAnimations);

private:
	void LoadProperties(bool& aLerpAnimations);
	void LoadSettings(Utility::Vector4<float>& aClearColor, const std::filesystem::path& aPath);

	std::string myInputBuffer;
};
