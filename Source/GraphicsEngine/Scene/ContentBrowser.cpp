#include "NuggetBox.pch.h"
#include "ContentBrowser.h"

#include <imgui/imgui.h>
#include <algorithm>
#include <cctype>
#include <string>

void ContentBrowser::Initialize()
{
	myRoot = "Assets";
	myCurrentPath = myRoot;
	myDirectoryIcon = Texture::Load("Assets/Textures/Icons/icon_directory.dds");
	myFileIcon = Texture::Load("Assets/Textures/Icons/icon_file.dds");
	myBackButton = Texture::Load("Assets/Textures/Icons/icon_back.dds");
}

void ContentBrowser::Update()
{
	ImGui::Begin("Content Browser");

	if (ImGui::ImageButton(myBackButton->GetSRV().Get(), { 50, 20 }))
	{
		if (myCurrentPath != myRoot)
		{
			myCurrentPath = myCurrentPath.string().erase(myCurrentPath.string().find_last_of('\\'));
		}
	}

	if (ImGui::BeginTable("##ContentBrowserStuff", static_cast<int>(ImGui::GetContentRegionAvail().x / 90.0f)))
	{
		for (auto& entry : std::filesystem::directory_iterator(myCurrentPath))
		{
			ImGui::TableNextColumn();

			//File entries
			if (entry.is_regular_file())
			{
				ImGui::Image(myFileIcon->GetSRV().Get(), { 90, 90 });

				std::string extension = entry.path().extension().string();
				//To lower
				std::transform(extension.begin(), extension.end(), extension.begin(), [](unsigned char c) { return static_cast<char>(std::tolower(static_cast<int>(c))); });

				if (extension == ".dds")
				{
					if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceAllowNullID))
					{
						ImGui::SetDragDropPayload("TEXTURE", entry.path().string().c_str(), entry.path().string().size() + 1);
						ImGui::EndDragDropSource();
					}
				}
			}
			//Directory entries
			else if (entry.is_directory())
			{
				ImGui::Image(myDirectoryIcon->GetSRV().Get(), { 90, 90 });

				if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
				{
					myCurrentPath = entry;
				}
			}

			//Name
			ImGui::Text(entry.path().filename().string().c_str());
		}

		ImGui::EndTable();
	}
	
	ImGui::End();
}

void ContentBrowser::HandleDragDrop(const std::filesystem::path& aDroppedFile)
{
	std::string pathToCheck = myCurrentPath.string() + '\\' + aDroppedFile.filename().string();

	//Insert a 0
	int count = 0;
	if (std::filesystem::exists(pathToCheck))
	{
		pathToCheck.insert(pathToCheck.find_last_of('.'), std::to_string(count));
	}

	while (std::filesystem::exists(pathToCheck))
	{
		count++;
		pathToCheck.replace(pathToCheck.find_last_of('.') - 1, 1, std::to_string(count));
	}

	std::filesystem::copy(aDroppedFile, pathToCheck);
}
