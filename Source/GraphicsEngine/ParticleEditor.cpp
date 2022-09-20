#include "NuggetBox.pch.h"
#include "ParticleEditor.h"

#include <imgui/imgui.h>

void ParticleEditor::Initialize()
{
}

void ParticleEditor::Update(std::vector<ParticleSystem>& outSystems)
{
	EmitterSettings& settings = myCurrentTemplate.EmitterSettings;

	ImGui::Begin("Editor");
	ImGui::ShowDemoWindow();

	//Emitter Type
	ImGui::Text("Emitter Type");
	ToolTip("Changes the emission type of the emitter");
	ImGui::SameLine();
	if (ImGui::RadioButton("Cone", settings.EmitterType == EmitterType::Cone))
	{
		settings.EmitterType = EmitterType::Cone;
	}
	ToolTip("Particles emit in a cone pattern");
	ImGui::SameLine();
	if (ImGui::RadioButton("Sphere", settings.EmitterType == EmitterType::Sphere))
	{
		settings.EmitterType = EmitterType::Sphere;
	}
	ToolTip("Particles emit in a sphere pattern");
	//

	//TODO: ADD TOOLTIPS

	//Cone settings
	if (settings.EmitterType == EmitterType::Cone)
	{
		if (ImGui::TreeNode("Cone Emitter Settings"))
		{
			ImGui::Text("Inner Radius");
			ToolTip("The inner radius of the cone-shaped emission");
			ImGui::SameLine();
			ImGui::DragFloat("##InnerRadius", &settings.InnerRadius, 1.0f, 0.0f, settings.OuterRadius);

			ImGui::Text("Outer Radius");
			ToolTip("The outer radius of the cone-shaped emission, located 1 meter above the inner radius");
			ImGui::SameLine();
			ImGui::DragFloat("##OuterRadius", &settings.OuterRadius, 1.0f, settings.InnerRadius, FLT_MAX);

			ImGui::Text("Spawn Origin");
			ToolTip("Should the particles spawn on the edge of the inner circle of the cone, or inside the circle");
			ImGui::SameLine();
			if (ImGui::RadioButton("Spawn On Edge", settings.SpawnOnEdge))
			{
				ToolTip("The particles spawn on the edge of the inner circle");
				settings.SpawnOnEdge = true;
			}
			ImGui::SameLine();
			if (ImGui::RadioButton("Spawn On Circle", !settings.SpawnOnEdge))
			{
				ToolTip("The particles spawn inside the inner circle");
				settings.SpawnOnEdge = false;
			}

			ImGui::TreePop();
		}
	}
	//

	//Sphere Settings
	if (settings.EmitterType == EmitterType::Sphere)
	{
		if (ImGui::TreeNode("Sphere Emitter Settings"))
		{
			ImGui::Text("Sphere Radius");
			ImGui::SameLine();
			ImGui::DragFloat("##SphereRadius", &settings.Radius, 1.0f, 0.0f, FLT_MAX);

			ImGui::Text("Spawn Origin");
			ImGui::SameLine();
			if (ImGui::RadioButton("Spawn On Surface", settings.SpawnOnSurface))
			{
				settings.SpawnOnSurface = true;
			}
			ImGui::SameLine();
			if (ImGui::RadioButton("Spawn In Volume", !settings.SpawnOnSurface))
			{
				settings.SpawnOnSurface = false;
			}

			ImGui::TreePop();
		}
	}
	//

	//Spawn rate
	ImGui::Text("Spawn Rate");
	ToolTip("How many particles spawn per second");
	ImGui::SameLine();
	ImGui::DragFloat("##SpawnRate", &settings.SpawnRate, 1.0f, 0.0f, FLT_MAX);
	//

	//Life Time
	if (ImGui::TreeNode("LifeTime"))
	{
		ImGui::Text("Min");
		ImGui::SameLine();
		ImGui::DragFloat("##Min", &settings.MinLifeTime, 0.1f, 0, settings.MaxLifeTime);
		ImGui::Text("Max");
		ImGui::SameLine();
		ImGui::DragFloat("##Max", &settings.MaxLifeTime, 0.1f, settings.MinLifeTime, FLT_MAX);

		ImGui::TreePop();
	}
	//

	//Scale
	if (ImGui::TreeNode("Scale"))
	{
		ImGui::Text("Start");
		ImGui::SameLine();
		ImGui::DragFloat3("##StartScale", &settings.StartScale.x, 0.1f, 0.0f, FLT_MAX);

		ImGui::Text("End");
		ImGui::SameLine();
		ImGui::DragFloat3("##EndScale", &settings.EndScale.x, 0.1f, 0.0f, FLT_MAX);

		ImGui::Text("Scale Multiplier");
		ImGui::SameLine();
		ImGui::DragFloat("##ScaleMultiplier", &settings.ScaleMultiplier, 0.05f, 0.0f, FLT_MAX);

		ImGui::TreePop();
	}
	//

	//Color
	if (ImGui::TreeNode("Color"))
	{
		ImGui::ColorPicker4("StartColor", &settings.StartColor.x);
		ImGui::ColorPicker4("EndColor", &settings.EndColor.x);

		ImGui::TreePop();
	}
	//

	//Speed
	if (ImGui::TreeNode("Speed"))
	{
		ImGui::Text("Start");
		ImGui::SameLine();
		ImGui::DragFloat3("##StartSpeed", &settings.StartSpeed.x, 0.5f, -FLT_MAX, FLT_MAX);

		ImGui::Text("End");
		ImGui::SameLine();
		ImGui::DragFloat3("##EndSpeed", &settings.EndSpeed.x, 0.5f, -FLT_MAX, FLT_MAX);

		ImGui::Text("Speed Multiplier");
		ImGui::SameLine();
		ImGui::DragFloat("##SpeedMultiplier", &settings.SpeedMultiplier, 0.05f, 0.0f, FLT_MAX);

		ImGui::TreePop();
	}
	//

	//Acceleration
	if (ImGui::TreeNode("Acceleration"))
	{
		ImGui::Checkbox("Use Acceleration", &settings.UseAcceleration);

		if (settings.UseAcceleration)
		{
			ImGui::Text("Acceleration");
			ImGui::SameLine();
			ImGui::DragFloat3("##Acceleration", &settings.Acceleration.x, 0.5f, -FLT_MAX, FLT_MAX);

			ImGui::Text("Maximum Speed");
			ImGui::SameLine();
			ImGui::DragFloat("##MaxSpeed", &settings.MaxSpeed, 0.5f, 0.0f, FLT_MAX);
		}

		ImGui::TreePop();
	}
	//

	//Blend mode
	//TODO: Add blend mode setting
	//

	//Looping
	ImGui::Checkbox("Looping", &settings.Looping);
	//

	//Scaled Delta Time
	ImGui::Checkbox("Use Scaled DeltaTime", &settings.ScaledDeltaTime);
	//

	ImGui::End();
}

void ParticleEditor::ToolTip(const char* aMessage)
{
	if (ImGui::IsItemHovered())
	{
		ImGui::BeginTooltip();
		ImGui::Text(aMessage);
		ImGui::EndTooltip();
	}
}