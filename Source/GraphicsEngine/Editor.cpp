#include "NuggetBox.pch.h"
#include "Editor.h"

#include "Timer.h"
#include "imgui/imgui.h"
#include "json.hpp"

void Editor::Initialize(Utility::Vector4<float>& aClearColor, bool& aLerpAnimations)
{
	LoadProperties(aLerpAnimations);
	LoadSettings(aClearColor);
}

void Editor::UpdateEditorInterface(Utility::Vector4<float>& aClearColor, bool& aLerpAnimations)
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

		if (ImGui::Button("Save"))
		{
			nlohmann::json fileToWrite;
			fileToWrite["ClearColor"]["r"] = aClearColor.x;
			fileToWrite["ClearColor"]["g"] = aClearColor.y;
			fileToWrite["ClearColor"]["b"] = aClearColor.z;
			fileToWrite["ClearColor"]["a"] = aClearColor.w;
			std::ofstream file("Json/Settings.json");
			file << fileToWrite;
		}

		ImGui::EndMenu();
	}

	if (ImGui::BeginMenu("Properties"))
	{
		ImGui::Checkbox("Lerp Animations", &aLerpAnimations);

		float timeScale = Utility::Timer::GetTimeScale();

		if (ImGui::SliderFloat("Time Scale", &timeScale, 0.01f, 10.0f))
		{
			Utility::Timer::SetTimeScale(timeScale);
		}

		if (ImGui::Button("Save"))
		{
			nlohmann::json fileToWrite;
			fileToWrite["LerpAnimations"] = aLerpAnimations;
			fileToWrite["TimeScale"] = timeScale;
			std::ofstream file("Json/Properties.json");
			file << fileToWrite;
		}

		ImGui::EndMenu();
	}

	ImGui::EndMainMenuBar();
}

void Editor::LoadSettings(Utility::Vector4<float>& aClearColor)
{
	if (std::filesystem::exists("json/settings.json"))
	{
		std::fstream file;
		file.open("json/settings.json");
		nlohmann::json json = nlohmann::json::parse(file);

		if (json.contains("ClearColor"))
		{
			aClearColor.x = json["ClearColor"]["r"];
			aClearColor.y = json["ClearColor"]["g"];
			aClearColor.z = json["ClearColor"]["b"];
			aClearColor.w = json["ClearColor"]["a"];
		}
	}
}

void Editor::LoadProperties(bool& aLerpAnimations)
{
	if (std::filesystem::exists("Json/Properties.json"))
	{
		std::fstream file;
		file.open("Json/Properties.json");
		nlohmann::json json = nlohmann::json::parse(file);

		if (json.contains("LerpAnimations") && json.contains("TimeScale"))
		{
			aLerpAnimations = json["LerpAnimations"];
			Utility::Timer::SetTimeScale(json["TimeScale"]);
		}
	}
}