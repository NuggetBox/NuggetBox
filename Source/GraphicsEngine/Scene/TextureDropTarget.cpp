#include "NuggetBox.pch.h"
#include "TextureDropTarget.h"

#include "imgui/imgui.h"

void TextureDropTarget::Initialize()
{
	mySpiderCatIcon = Texture::Load("Assets/Textures/Icons/icon_directory.dds");
}

void TextureDropTarget::Update(std::filesystem::path& aTextureToLoad)
{
	ImGui::Begin("Spider Cat Texture Dropping Window");

	ImGui::Image(mySpiderCatIcon->GetSRV().Get(), { 100, 100 });

	if (ImGui::BeginDragDropTarget())
	{
		if (auto payload = ImGui::AcceptDragDropPayload("TEXTURE"))
		{
			const char* data = (const char*)payload->Data;
			aTextureToLoad = data;
			ImGui::EndDragDropTarget();
		}
	}

	ImGui::Text("Spider Cat Texture Dropping Area");

	ImGui::End();
}