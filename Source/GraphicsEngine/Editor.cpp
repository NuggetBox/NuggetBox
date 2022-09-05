#include "NuggetBox.pch.h"
#include "Editor.h"

#include "Timer.h"
#include "imgui/imgui.h"
#include "json.hpp"

void Editor::UpdateEditorInterface(Utility::Vector4<float>& aClearColor, float& aTimeScale, bool& aLerpAnimations)
{
	ImGui::BeginMainMenuBar();
	if (ImGui::BeginMenu("File"))
	{
		ImGui::Text("I Do Nothing");
		ImGui::EndMenu();
	}
	if (ImGui::BeginMenu("Settings"))
	{
		ImGui::Text("Clear color selection");
		ImGui::ColorEdit4("Clear color", &aClearColor.x);
		ImGui::Checkbox("Lerp Animations", &aLerpAnimations);
		if (ImGui::SliderFloat("Time Scale", &aTimeScale, 0.01f, 10.0f))
		{
			Utility::Timer::SetTimeScale(aTimeScale);
		}

		if (ImGui::Button("Save"))
		{
			nlohmann::json fileToWrite;
			fileToWrite["ClearColor"]["r"] = aClearColor.x;
			fileToWrite["ClearColor"]["g"] = aClearColor.y;
			fileToWrite["ClearColor"]["b"] = aClearColor.z;
			fileToWrite["ClearColor"]["a"] = aClearColor.w;
			fileToWrite["LerpAnimations"] = aLerpAnimations;
			fileToWrite["TimeScale"] = aTimeScale;
			std::ofstream file("Json/settings.json");
			file << fileToWrite;
		}
		ImGui::EndMenu();
	}
	ImGui::EndMainMenuBar();
}