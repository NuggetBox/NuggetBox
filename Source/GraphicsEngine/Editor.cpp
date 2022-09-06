#include "NuggetBox.pch.h"
#include "Editor.h"

#include "Timer.h"
#include "imgui/imgui.h"
#include "imgui/misc/cpp/imgui_stdlib.h"
#include "json.hpp"

#include <iostream>
#include <filesystem>
#include <Windows.h>
#include <shellapi.h>

void Editor::Initialize(Utility::Vector4<float>& aClearColor, bool& aLerpAnimations)
{
	LoadProperties(aLerpAnimations);
	//LoadSettings(aClearColor);
}

void Editor::UpdateEditorInterface(Utility::Vector4<float>& aClearColor, bool& aLerpAnimations)
{
	ImGui::ShowDemoWindow();

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

		if (ImGui::Button("Load")) 
		{
			ImGui::OpenPopup("LoadPreset");
		}

		if (ImGui::BeginPopup("LoadPreset")) 
		{
			if (ImGui::TreeNode("Saved Presets")) {
				for (auto& file : std::filesystem::recursive_directory_iterator("json/settings/"))
				{
					if (file.is_regular_file()) 
					{
						if (ImGui::TreeNode(file.path().filename().string().c_str()))
						{
							if (ImGui::Button("Load"))
							{
								LoadSettings(aClearColor, file.path());
							}
							if (ImGui::Button("Delete"))
							{
								std::wstring path = file.path().wstring() + std::wstring(1, L'\0');

								SHFILEOPSTRUCT fileOp;     
								fileOp.hwnd = NULL;
								fileOp.wFunc = FO_DELETE;
								fileOp.pFrom = path.c_str();
								fileOp.pTo = NULL;
								fileOp.fFlags = FOF_ALLOWUNDO | FOF_NOERRORUI | FOF_NOCONFIRMATION | FOF_SILENT;
								int result = SHFileOperation(&fileOp);
							}
							ImGui::TreePop();
						}
					}
				}
				ImGui::TreePop();
			}
			

			ImGui::EndPopup();
		}

		if (ImGui::Button("Save"))
		{
			ImGui::OpenPopup("FilenameSelect");
			myInputBuffer = "Settings1";
		}

		if (ImGui::BeginPopup("FilenameSelect")) 
		{
			ImGui::InputText("Filename", &myInputBuffer, ImGuiInputTextFlags_ReadOnly);

			if (ImGui::Button("Save Preset"))
			{
				nlohmann::json fileToWrite;
				fileToWrite["ClearColor"]["r"] = aClearColor.x;
				fileToWrite["ClearColor"]["g"] = aClearColor.y;
				fileToWrite["ClearColor"]["b"] = aClearColor.z;
				fileToWrite["ClearColor"]["a"] = aClearColor.w;
				std::ofstream file("Json/Settings/" + myInputBuffer + ".json");
				file << fileToWrite;
			}

			ImGui::EndPopup();
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

void Editor::LoadSettings(Utility::Vector4<float>& aClearColor, const std::filesystem::path& aPath)
{
	if (std::filesystem::exists(aPath))
	{
		std::fstream file;
		file.open(aPath);
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