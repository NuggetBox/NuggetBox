#include "NuggetBox.pch.h"
#include "Hierarchy.h"

#include "json.hpp"
#include "InputHandler.h"
#include <imgui/imgui.h>

void Hierarchy::Initialize()
{
	myRoot = std::make_shared<HierarchyObject>("Root");
	myRoot->AddChild(std::make_shared<HierarchyObject>("SpinningCube"), myRoot);
	myRoot->AddChild(std::make_shared<HierarchyObject>("Cube"), myRoot);
	myRoot->AddChild(std::make_shared<HierarchyObject>("RunningGoblin"), myRoot);
	myRoot->AddChild(std::make_shared<HierarchyObject>("WalkingGoblin"), myRoot);
	myRoot->AddChild(std::make_shared<HierarchyObject>("Pyramid"), myRoot);
	myRoot->AddChild(std::make_shared<HierarchyObject>("SpiderCat"), myRoot);
	myRoot->AddChild(std::make_shared<HierarchyObject>("Floor"), myRoot);
	myRoot->AddChild(std::make_shared<HierarchyObject>("PointLight"), myRoot);
	myRoot->AddChild(std::make_shared<HierarchyObject>("SpotLight"), myRoot);
	myRoot->AddChild(std::make_shared<HierarchyObject>("DirectionalLight"), myRoot);
}

void Hierarchy::Update(HWND aHandle)
{
	myHandle = aHandle;

	ImGui::Begin("Hierarchy");

	if (ImGui::Button("Save Hierarchy"))
	{
		SaveHierarchy();
	}

	if (ImGui::Button("Load Hierarchy"))
	{

	}

	DrawObject(myRoot);
	ImGui::End();
}

void Hierarchy::DrawObject(std::shared_ptr<HierarchyObject> aObject)
{
	if (aObject != nullptr)
	{
		ImGuiTreeNodeFlags flags = 0;

		if (aObject->GetSelected())
		{
			flags |= ImGuiTreeNodeFlags_Selected;
		}

		if (ImGui::TreeNodeEx(aObject->GetName().c_str(), flags))
		{
			if (aObject->GetName() != "Root")
			{
				if (Utility::InputHandler::GetKeyHeld(VK_CONTROL))
				{
					if (ImGui::IsItemClicked())
					{
						auto iterator = std::find(mySelectedObjects.begin(), mySelectedObjects.end(), aObject);

						if (iterator != mySelectedObjects.end())
						{
							mySelectedObjects.erase(iterator);
							aObject->SetSelected(false);
						}
						else
						{
							mySelectedObjects.push_back(aObject);
							aObject->SetSelected();
						}
					}
				}

				//If this entity is being dragged
				if (ImGui::BeginDragDropSource())
				{
					if (!aObject->GetSelected())
					{
						ImGui::SetDragDropPayload("obj", &aObject, sizeof(std::shared_ptr<HierarchyObject>));
						ImGui::EndDragDropSource();
					}
					else
					{
						ImGui::SetDragDropPayload("manyObj", nullptr, 0);
						ImGui::EndDragDropSource();
					}
				}
			}

			//If this entity is getting dropped on
			if (ImGui::BeginDragDropTarget())
			{
				const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("obj");

				if (payload != nullptr)
				{
					bool executeParentTransfer = true;
					std::shared_ptr<HierarchyObject> droppedObject = *static_cast<std::shared_ptr<HierarchyObject>*>(payload->Data);

					//Check if object dropped will result in being its own parent recursive
					std::shared_ptr<HierarchyObject> checkParent = aObject;
					while (checkParent != nullptr)
					{
						if (checkParent == droppedObject)
						{
							executeParentTransfer = false;
							break;
						}

						checkParent = checkParent->GetParent();
					}

					//Check if object dropped on itself
					if (aObject == droppedObject)
					{
						executeParentTransfer = false;
					}

					//Check if object dropped on its own parent
					if (droppedObject->GetParent() == aObject)
					{
						executeParentTransfer = false;
					}

					//Execute the parent transfer
					if (executeParentTransfer)
					{
						droppedObject->GetParent()->RemoveChild(droppedObject);
						aObject->AddChild(droppedObject, aObject);
					}
				}

				const ImGuiPayload* multiplePayload = ImGui::AcceptDragDropPayload("manyObj");

				if (multiplePayload != nullptr)
				{
					bool executeParentTransfer = true;

					//Check if object dropped will result in being its own parent recursive
					for (auto& object : mySelectedObjects)
					{
						for (auto& otherSelectedObject : mySelectedObjects)
						{
							if (object != otherSelectedObject)
							{
								std::shared_ptr<HierarchyObject> checkParent = otherSelectedObject;
								while (checkParent != nullptr)
								{
									if (checkParent == object)
									{
										executeParentTransfer = false;
										break;
									}

									checkParent = checkParent->GetParent();
								}
							}
						}

						std::shared_ptr<HierarchyObject> checkParent = aObject;
						while (checkParent != nullptr)
						{
							if (checkParent == object)
							{
								executeParentTransfer = false;
								break;
							}

							checkParent = checkParent->GetParent();
						}

						//Check if object dropped on itself
						if (aObject == object)
						{
							executeParentTransfer = false;
						}

						//Check if object dropped on its own parent
						if (object->GetParent() == aObject)
						{
							executeParentTransfer = false;
						}
					}

					//Execute the parent transfer
					if (executeParentTransfer)
					{
						for (auto& object : mySelectedObjects)
						{
							object->GetParent()->RemoveChild(object);
							aObject->AddChild(object, aObject);
						}
					}
				}

				ImGui::EndDragDropTarget();
			}

			for (auto& child : aObject->GetChildren())
			{
				DrawObject(child);
			}

			ImGui::TreePop();
		}
	}
}

void Hierarchy::SaveHierarchy()
{
	nlohmann::json json;
	SaveObject(myRoot, json["Objects"]);
	std::ofstream file("Assets/Json/Scenes/Hierarchy.json");
	file << std::setw(4) << json;
}

void Hierarchy::SaveObject(const std::shared_ptr<HierarchyObject>& aObject, nlohmann::basic_json<>& aJson)
{
	aJson["Name"] = aObject->GetName();

	for (int i = 0; i < aObject->GetChildren().size(); ++i)
	{
		SaveObject(aObject->GetChildren()[i], aJson["Children"][i]);
	}
}

void Hierarchy::LoadHierarchy()
{
}