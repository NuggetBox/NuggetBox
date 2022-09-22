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

	myPresetInput1 = aClearColor;
	myPresetInput2 = aClearColor;

	if (std::filesystem::exists("Json/ClearColorSettings/Blends/Default.json"))
	{
		std::ifstream file("Json/ClearColorSettings/Blends/Default.json");
		nlohmann::json json = nlohmann::json::parse(file);
		if (json.contains("PreviousBlend"))
		{
			if (std::filesystem::exists(json["PreviousBlend"]))
			{
				std::ifstream defaultFile;
				defaultFile.open(json["PreviousBlend"].get<std::string>());
				nlohmann::json defaultJson = nlohmann::json::parse(defaultFile);

				myUseBlend = true;

				myPresetInput1.x = defaultJson["Preset1"]["x"];
				myPresetInput1.y = defaultJson["Preset1"]["y"];
				myPresetInput1.z = defaultJson["Preset1"]["z"];
				myPresetInput1.w = defaultJson["Preset1"]["w"];

				myPresetInput2.x = defaultJson["Preset2"]["x"];
				myPresetInput2.y = defaultJson["Preset2"]["y"];
				myPresetInput2.z = defaultJson["Preset2"]["z"];
				myPresetInput2.w = defaultJson["Preset2"]["w"];

				myBlendFactor = defaultJson["BlendFactor"];

				aClearColor = Vector4f::Lerp(myPresetInput1, myPresetInput2, myBlendFactor);
			}
		}
	}
}

void Editor::UpdateEditorInterface(Utility::Vector4<float>& aClearColor, bool& aLerpAnimations, std::filesystem::path& aTexturePathToLoad)
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

		if (ImGui::Checkbox("Blend", &myUseBlend))
		{
			myPresetInput1 = aClearColor;
			myPresetInput2 = aClearColor;
		}

		if (myUseBlend)
		{
			ImGui::ColorEdit4("Preset 1", &myPresetInput1.x);
			ImGui::ColorEdit4("Preset 2", &myPresetInput2.x);

			ImGui::SliderFloat("Blend Factor", &myBlendFactor, 0, 1);

			aClearColor = Vector4f::Lerp(myPresetInput1, myPresetInput2, myBlendFactor);

			if (ImGui::Button("Load Preset 1"))
			{
				ImGui::OpenPopup("LoadPreset1");
			}

			ImGui::SameLine();

			if (ImGui::Button("Save Preset 1"))
			{
				ImGui::OpenPopup("SavePreset1");
				myInputBuffer = "Preset1";
			}

			if (ImGui::Button("Load Preset 2"))
			{
				ImGui::OpenPopup("LoadPreset2");
			}

			ImGui::SameLine();

			if (ImGui::Button("Save Preset 2"))
			{
				ImGui::OpenPopup("SavePreset2");
				myInputBuffer = "Preset2";
			}

			if (ImGui::Button("Save Blend as Preset"))
			{
				ImGui::OpenPopup("SaveBlendPreset");
				myInputBuffer = "BlendedPreset1";
			}

			if (ImGui::Button("Load Blend"))
			{
				ImGui::OpenPopup("LoadBlend");
			}

			ImGui::SameLine();

			if (ImGui::Button("Save Blend"))
			{
				ImGui::OpenPopup("SaveBlend");
				myInputBuffer = "Blend1";
			}

			if (ImGui::BeginPopup("LoadPreset1"))
			{
				LoadColorPopup(myPresetInput1);
				ImGui::EndPopup();
			}

			if (ImGui::BeginPopup("SavePreset1"))
			{
				SaveColorPopup(myPresetInput1);
				ImGui::EndPopup();
			}

			if (ImGui::BeginPopup("LoadPreset2"))
			{
				LoadColorPopup(myPresetInput2);
				ImGui::EndPopup();
			}

			if (ImGui::BeginPopup("SavePreset2"))
			{
				SaveColorPopup(myPresetInput2);
				ImGui::EndPopup();
			}

			if (ImGui::BeginPopup("SaveBlendPreset"))
			{
				SaveColorPopup(Vector4f::Lerp(myPresetInput1, myPresetInput2, myBlendFactor));
				ImGui::EndPopup();
			}

			if (ImGui::BeginPopup("LoadBlend"))
			{
				LoadBlendPopup();
				ImGui::EndPopup();
			}

			ImGui::SameLine();

			if (ImGui::BeginPopup("SaveBlend"))
			{
				SaveBlendPopup();
				ImGui::EndPopup();
			}
		}
		else
		{
			ImGui::ColorEdit4("Clear color", &aClearColor.x);

			if (ImGui::Button("Load"))
			{
				ImGui::OpenPopup("LoadPreset");
			}

			if (ImGui::BeginPopup("LoadPreset"))
			{
				LoadColorPopup(aClearColor);
				ImGui::EndPopup();
			}

			if (ImGui::Button("Save"))
			{
				ImGui::OpenPopup("SavePreset");
				myInputBuffer = "ClearColor1";
			}

			if (ImGui::BeginPopup("SavePreset"))
			{
				SaveColorPopup(aClearColor);
				ImGui::EndPopup();
			}
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

	if (ImGui::BeginMenu("Texture Manager"))
	{
		if (ImGui::TreeNode("Textures"))
		{
			for (auto& file : std::filesystem::recursive_directory_iterator("textures/"))
			{
				if (file.is_regular_file())
				{
					if (ImGui::TreeNode(file.path().filename().string().c_str()))
					{
						if (ImGui::Button("Load"))
						{
							aTexturePathToLoad = file.path();
						}
						ImGui::TreePop();
					}
				}
			}
			ImGui::TreePop();
		}

		ImGui::EndMenu();
	}

	ImGui::EndMainMenuBar();
}

void Editor::LoadClearColor(Utility::Vector4<float>& aClearColor, const std::filesystem::path& aPath)
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

void Editor::LoadColorPopup(Utility::Vector4f& aColorToLoad)
{
	if (ImGui::TreeNode("Saved Presets")) 
	{
		for (auto& file : std::filesystem::recursive_directory_iterator("json/clearcolorsettings/presets/"))
		{
			if (file.is_regular_file())
			{
				if (ImGui::TreeNode(file.path().filename().string().c_str()))
				{
					if (ImGui::Button("Load"))
					{
						LoadClearColor(aColorToLoad, file.path());
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
}

void Editor::SaveColorPopup(const Utility::Vector4f& aColorToSave)
{
	ImGui::InputText("Filename", &myInputBuffer);

	if (ImGui::Button("Save Preset"))
	{
		nlohmann::json fileToWrite;
		fileToWrite["ClearColor"]["r"] = aColorToSave.x;
		fileToWrite["ClearColor"]["g"] = aColorToSave.y;
		fileToWrite["ClearColor"]["b"] = aColorToSave.z;
		fileToWrite["ClearColor"]["a"] = aColorToSave.w;
		std::ofstream file("Json/ClearColorSettings/Presets/" + myInputBuffer + ".json");
		file << fileToWrite;
	}
}

void Editor::LoadProperties(bool& aLerpAnimations)
{
	if (std::filesystem::exists("Json/Properties.json"))
	{
		std::ifstream file;
		file.open("Json/Properties.json");
		nlohmann::json json = nlohmann::json::parse(file);

		if (json.contains("LerpAnimations") && json.contains("TimeScale"))
		{
			aLerpAnimations = json["LerpAnimations"];
			Utility::Timer::SetTimeScale(json["TimeScale"]);
		}
	}
}

void Editor::LoadBlendPopup()
{
	if (ImGui::TreeNode("Saved Blends"))
	{
		for (auto& file : std::filesystem::recursive_directory_iterator("json/clearcolorsettings/blends/"))
		{
			if (file.is_regular_file())
			{
				if (ImGui::TreeNode(file.path().filename().string().c_str()))
				{
					if (ImGui::Button("Load"))
					{
						std::ifstream fStream;
						fStream.open(file);
						nlohmann::json json = nlohmann::json::parse(fStream);

						if (json.contains("Preset1") && json.contains("Preset2") && json.contains("BlendFactor"))
						{
							myPresetInput1.x = json["Preset1"]["x"];
							myPresetInput1.y = json["Preset1"]["y"];
							myPresetInput1.z = json["Preset1"]["z"];
							myPresetInput1.w = json["Preset1"]["w"];

							myPresetInput2.x = json["Preset2"]["x"];
							myPresetInput2.y = json["Preset2"]["y"];
							myPresetInput2.z = json["Preset2"]["z"];
							myPresetInput2.w = json["Preset2"]["w"];

							myBlendFactor = json["BlendFactor"];
						}
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
}

void Editor::SaveBlendPopup()
{
	ImGui::InputText("Filename", &myInputBuffer);

	if (ImGui::Button("Save Blend"))
	{
		nlohmann::json fileToWrite;

		fileToWrite["Preset1"]["x"] = myPresetInput1.x;
		fileToWrite["Preset1"]["y"] = myPresetInput1.y;
		fileToWrite["Preset1"]["z"] = myPresetInput1.z;
		fileToWrite["Preset1"]["w"] = myPresetInput1.w;

		fileToWrite["Preset2"]["x"] = myPresetInput2.x;
		fileToWrite["Preset2"]["y"] = myPresetInput2.y;
		fileToWrite["Preset2"]["z"] = myPresetInput2.z;
		fileToWrite["Preset2"]["w"] = myPresetInput2.w;

		fileToWrite["BlendFactor"] = myBlendFactor;

		std::ofstream file("Json/ClearColorSettings/Blends/" + myInputBuffer + ".json");
		file << fileToWrite;

		//Save previous saved blend
		fileToWrite.clear();
		file.close();
		fileToWrite["PreviousBlend"] = "Json/ClearColorSettings/Blends/" + myInputBuffer + ".json";
		file.open("Json/ClearColorSettings/Blends/Default.json");
		file << fileToWrite;
	}
}