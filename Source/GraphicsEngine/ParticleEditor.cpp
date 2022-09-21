#include "NuggetBox.pch.h"
#include "ParticleEditor.h"

#include <imgui/imgui.h>
#include <iostream>
#include "json.hpp"

void ParticleEditor::Initialize()
{
}

void ParticleEditor::Update(std::vector<ParticleSystem>& outSystems)
{
	EmitterSettings& settings = myCurrentTemplate.EmitterSettings;

	ImGui::Begin("Particle Emitter Settings");

	//TODO: Drag&Drop .dds texture file to use for the particle system

	//Emitter Type
	ImGui::Text("Emitter Type");
	ToolTip("Changes the emission type of the emitter");
	ImGui::SameLine();
	if (ImGui::RadioButton("Cone", settings.EmitterType == EmitterType::Cone))
	{
		settings.EmitterType = EmitterType::Cone;
	}
	ToolTip("Particles emit from a cone");
	ImGui::SameLine();
	if (ImGui::RadioButton("Sphere", settings.EmitterType == EmitterType::Sphere))
	{
		settings.EmitterType = EmitterType::Sphere;
	}
	ToolTip("Particles emit from a sphere");
	//

	//TODO: ADD TOOLTIPS

	//Cone settings
	if (settings.EmitterType == EmitterType::Cone)
	{
		if (ImGui::TreeNode("Cone Emitter Settings"))
		{
			ImGui::Text("Inner Radius");
			ToolTip("The inner radius of the cone-shaped emission (centimeters)");
			ImGui::SameLine();
			ImGui::DragFloat("##InnerRadius", &settings.InnerRadius, 1.0f, 0.0f, settings.OuterRadius);

			ImGui::Text("Outer Radius");
			ToolTip("The outer radius of the cone-shaped emission (centimeters), located 1 meter above the inner radius");
			ImGui::SameLine();
			ImGui::DragFloat("##OuterRadius", &settings.OuterRadius, 1.0f, settings.InnerRadius, FLT_MAX);

			ImGui::Text("Spawn Origin");
			ToolTip("Should the particles spawn on the edge of the inner circle of the cone, or inside the circle");
			ImGui::SameLine();
			if (ImGui::RadioButton("Spawn On Edge", settings.SpawnOnEdge))
			{
				settings.SpawnOnEdge = true;
			}
			//ToolTip("The particles spawn on the edge of the inner circle");
			ImGui::SameLine();
			if (ImGui::RadioButton("Spawn In Circle", !settings.SpawnOnEdge))
			{
				settings.SpawnOnEdge = false;
			}
			//ToolTip("The particles spawn inside the inner circle");

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
			ToolTip("The radius of the sphere-shaped emission (centimeters)");
			ImGui::SameLine();
			ImGui::DragFloat("##SphereRadius", &settings.Radius, 1.0f, 0.0f, FLT_MAX);

			ImGui::Text("Spawn Origin");
			ToolTip("Should the particles spawn on the surface of the sphere, or inside the sphere");
			ImGui::SameLine();
			if (ImGui::RadioButton("Spawn On Surface", settings.SpawnOnSurface))
			{
				settings.SpawnOnSurface = true;
			}
			ToolTip("The particles spawn on the surface of the sphere");
			ImGui::SameLine();
			if (ImGui::RadioButton("Spawn In Volume", !settings.SpawnOnSurface))
			{
				settings.SpawnOnSurface = false;
			}
			ToolTip("The particles spawn inside of the sphere");

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
		ToolTip("The total lifetime for any given particle will be randomly generated between Min and Max (seconds)");
		ImGui::Text("Min");
		ToolTip("Minimum randomizable lifetime for any particle in the emitter (seconds)");
		ImGui::SameLine();
		ImGui::DragFloat("##Min", &settings.MinLifeTime, 0.1f, 0, settings.MaxLifeTime);
		ImGui::Text("Max");
		ToolTip("Maximum randomizable lifetime for any particle in the emitter (seconds)");
		ImGui::SameLine();
		ImGui::DragFloat("##Max", &settings.MaxLifeTime, 0.1f, settings.MinLifeTime, FLT_MAX);

		ImGui::TreePop();
	}
	//

	//Scale
	if (ImGui::TreeNode("Scale"))
	{
		ToolTip("The scale of a particle will go from Start to End during its lifetime");
		ImGui::Text("Start");
		ToolTip("The scale of the particle when it spawns");
		ImGui::SameLine();
		ImGui::DragFloat3("##StartScale", &settings.StartScale.x, 0.1f, 0.0f, FLT_MAX);

		ImGui::Text("End");
		ToolTip("The scale of the particle before it disappears");
		ImGui::SameLine();
		ImGui::DragFloat3("##EndScale", &settings.EndScale.x, 0.1f, 0.0f, FLT_MAX);

		ImGui::Text("Scale Multiplier");
		ToolTip("Introduces a variation in scale between the particles. For each particle, a number between 1 and Scale Multiplier will be generated and multiplied by the Start- and Endscale of that particle");
		ImGui::SameLine();
		ImGui::DragFloat("##ScaleMultiplier", &settings.ScaleMultiplier, 0.05f, 0.0f, FLT_MAX);

		ImGui::TreePop();
	}
	//

	//Color
	if (ImGui::TreeNode("Color"))
	{
		ToolTip("The color of a particle will interpolate between StartColor and EndColor during its lifetime");
		ImGui::ColorPicker4("StartColor", &settings.StartColor.x);
		ImGui::ColorPicker4("EndColor", &settings.EndColor.x);

		ImGui::TreePop();
	}
	//

	//Speed
	if (ImGui::TreeNode("Speed"))
	{
		ToolTip("The speed of a particle will go from Start to End during its lifetime (centimeters/second)");
		ImGui::Text("Start");
		ToolTip("The speed of the particle when it spawns (centimeters/second)");
		ImGui::SameLine();
		ImGui::DragFloat3("##StartSpeed", &settings.StartSpeed.x, 0.5f, -FLT_MAX, FLT_MAX);

		ImGui::Text("End");
		ToolTip("The speed of the particle before it disappears (centimeters/second)");
		ImGui::SameLine();
		ImGui::DragFloat3("##EndSpeed", &settings.EndSpeed.x, 0.5f, -FLT_MAX, FLT_MAX);

		ImGui::Text("Speed Multiplier");
		ToolTip("Introduces a variation in speed between the particles. For each particle, a number between 1 and Speed Multiplier will be generated and multiplied by the Start- and Endspeed of that particle");
		ImGui::SameLine();
		ImGui::DragFloat("##SpeedMultiplier", &settings.SpeedMultiplier, 0.05f, 0.0f, FLT_MAX);

		ImGui::TreePop();
	}
	//

	//Acceleration
	if (ImGui::TreeNode("Acceleration"))
	{
		ToolTip("The acceleration of a particle will affect the speed of that particle over time, like gravity (centimeters/second^2)");
		ImGui::Checkbox("Use Acceleration", &settings.UseAcceleration);

		if (settings.UseAcceleration)
		{
			ImGui::Text("Acceleration");
			ToolTip("The acceleration of the particles in each direction (centimeters/second^2)");
			ImGui::SameLine();
			ImGui::DragFloat3("##Acceleration", &settings.Acceleration.x, 0.5f, -FLT_MAX, FLT_MAX);

			ImGui::Text("Maximum Speed");
			ToolTip("The maximum speed allowed for any particle (centimeters/second)");
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
	ToolTip("Should the particle emitter loop or stop after one cycle");
	//

	//Scaled Delta Time
	ImGui::Checkbox("Use Scaled DeltaTime", &settings.ScaledDeltaTime);
	ToolTip("Should the particle emitter use Scaled Delta Time (affected by time manipulation) or Unscaled Delta Time (Even if time is slowed, the particle emitter runs at full speed)");
	//

	if (ImGui::Button("Save"))
	{
		SaveTemplate(myCurrentTemplate, "TEMPLATETEST.json");
	}

	if (ImGui::Button("Load"))
	{
		myCurrentTemplate = LoadTemplate("TEMPLATETEST.json");
	}

	ImGui::End();
}

void ParticleEditor::SaveTemplate(const ParticleEmitterTemplate& aTemplate, const std::filesystem::path& aPath)
{
	EmitterSettings settings = aTemplate.EmitterSettings;

	nlohmann::json json;
	json["TexturePath"] = aTemplate.TexturePath;

	json["EmitterType"] = settings.EmitterType;

	json["SpawnRate"] = settings.SpawnRate;
	json["MinLifeTime"] = settings.MinLifeTime;
	json["MaxLifeTime"] = settings.MaxLifeTime;

	json["StartScale"][0] = settings.StartScale.x;
	json["StartScale"][1] = settings.StartScale.y;
	json["StartScale"][2] = settings.StartScale.z;
	json["EndScale"][0] = settings.EndScale.x;
	json["EndScale"][1] = settings.EndScale.y;
	json["EndScale"][2] = settings.EndScale.z;
	json["ScaleMultiplier"] = settings.ScaleMultiplier;

	json["StartColor"][0] = settings.StartColor.x;
	json["StartColor"][1] = settings.StartColor.y;
	json["StartColor"][2] = settings.StartColor.z;
	json["StartColor"][3] = settings.StartColor.w;
	json["EndColor"][0] = settings.EndColor.x;
	json["EndColor"][1] = settings.EndColor.y;
	json["EndColor"][2] = settings.EndColor.z;
	json["EndColor"][3] = settings.EndColor.w;

	json["StartSpeed"][0] = settings.StartSpeed.x;
	json["StartSpeed"][1] = settings.StartSpeed.y;
	json["StartSpeed"][2] = settings.StartSpeed.z;
	json["EndSpeed"][0] = settings.EndSpeed.x;
	json["EndSpeed"][1] = settings.EndSpeed.y;
	json["EndSpeed"][2] = settings.EndSpeed.z;
	json["SpeedMultiplier"] = settings.SpeedMultiplier;

	json["UseAcceleration"] = settings.UseAcceleration;
	json["Acceleration"][0] = settings.Acceleration.x;
	json["Acceleration"][1] = settings.Acceleration.y;
	json["Acceleration"][2] = settings.Acceleration.z;
	json["MaxSpeed"] = settings.MaxSpeed;

	json["Looping"] = settings.Looping;
	json["ScaledDeltaTime"] = settings.ScaledDeltaTime;

	json["InnerRadius"] = settings.InnerRadius;
	json["OuterRadius"] = settings.OuterRadius;
	json["SpawnOnEdge"] = settings.SpawnOnEdge;
	json["AimForEdge"] = settings.AimForEdge;

	json["Radius"] = settings.Radius;
	json["SpawnOnSurface"] = settings.SpawnOnSurface;

	std::ofstream file(aPath.string());
	file << json;
	file.close();
}

ParticleEmitterTemplate ParticleEditor::LoadTemplate(const std::filesystem::path& aPath)
{
	if (std::filesystem::exists(aPath))
	{
		std::ifstream file(aPath.string());
		nlohmann::json json = nlohmann::json::parse(file);

		ParticleEmitterTemplate emitterTemplate;
		emitterTemplate.TexturePath = json["TexturePath"].get<std::string>();

		EmitterSettings& settings = emitterTemplate.EmitterSettings;
		settings.EmitterType = json["EmitterType"];

		settings.SpawnRate = json["SpawnRate"];
		settings.MinLifeTime = json["MinLifeTime"];
		settings.MaxLifeTime = json["MaxLifeTime"];

		settings.StartScale.x = json["StartScale"][0];
		settings.StartScale.y = json["StartScale"][1];
		settings.StartScale.z = json["StartScale"][2];
		settings.EndScale.x = json["EndScale"][0];
		settings.EndScale.y = json["EndScale"][1];
		settings.EndScale.z = json["EndScale"][2];
		settings.ScaleMultiplier = json["ScaleMultiplier"];

		settings.StartColor.x = json["StartColor"][0];
		settings.StartColor.y = json["StartColor"][1];
		settings.StartColor.z = json["StartColor"][2];
		settings.StartColor.w = json["StartColor"][3];
		settings.EndColor.x = json["EndColor"][0];
		settings.EndColor.y = json["EndColor"][1];
		settings.EndColor.z = json["EndColor"][2];
		settings.EndColor.w = json["EndColor"][3];

		settings.StartSpeed.x = json["StartSpeed"][0];
		settings.StartSpeed.y = json["StartSpeed"][1];
		settings.StartSpeed.z = json["StartSpeed"][2];
		settings.EndSpeed.x = json["EndSpeed"][0];
		settings.EndSpeed.y = json["EndSpeed"][1];
		settings.EndSpeed.z = json["EndSpeed"][2];
		settings.SpeedMultiplier = json["SpeedMultiplier"];

		settings.UseAcceleration = json["UseAcceleration"];
		settings.Acceleration.x = json["Acceleration"][0];
		settings.Acceleration.y = json["Acceleration"][1];
		settings.Acceleration.z = json["Acceleration"][2];
		settings.MaxSpeed = json["MaxSpeed"];

		settings.Looping = json["Looping"];
		settings.ScaledDeltaTime = json["ScaledDeltaTime"];

		settings.InnerRadius = json["InnerRadius"];
		settings.OuterRadius = json["OuterRadius"];
		settings.SpawnOnEdge = json["SpawnOnEdge"];
		settings.AimForEdge = json["AimForEdge"];

		settings.Radius = json["Radius"];
		settings.SpawnOnSurface = json["SpawnOnSurface"];

		return emitterTemplate;
	}

	DEBUGERROR("No Particle Emitter file found");
	return {};
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
