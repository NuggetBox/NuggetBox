#include "NuggetBox.pch.h"
#include "ParticleSystem.h"

#include <fstream>

#include "json.hpp"
#include "DebugLogger.h"

void ParticleSystem::Initialize(const ParticleSystemTemplate& aTemplate)
{
	for (auto& emitterInfo : aTemplate.EmitterInfos)
	{
		ParticleEmitter emitter;
		emitter.SetTransform(emitterInfo.Transform);
		emitter.LoadAndInitialize(emitterInfo.SettingsPath);
		myEmitters.push_back(emitter);
	}
}

void ParticleSystem::LoadAndInitialize(const std::filesystem::path& aTemplatePath)
{
	if (ourSystemTemplateRegistry.contains(aTemplatePath.string()))
	{
		Initialize(ourSystemTemplateRegistry.at(aTemplatePath.string()));
	}
	else
	{
		Initialize(Load(aTemplatePath.string()));
	}
}

void ParticleSystem::Update()
{
	for (auto& emitter : myEmitters)
	{
		emitter.Update();
	}
}

ParticleSystemTemplate ParticleSystem::Load(const std::filesystem::path& aTemplatePath)
{
	ParticleSystemTemplate loadedTemplate;

	if (ourSystemTemplateRegistry.contains(aTemplatePath.string()))
	{
		DEBUGLOG("Loaded Particle System " + aTemplatePath.filename().string() + " from registry successfully");
		return ourSystemTemplateRegistry.at(aTemplatePath.string());
	}

	if (!std::filesystem::exists(aTemplatePath))
	{
		DEBUGERROR("File " + aTemplatePath.string() + " was trying to be read as a ParticleSystemTemplate file but doesn't exist");
		return loadedTemplate;
	}

	using json = nlohmann::json;
	std::ifstream file(aTemplatePath);
	json data = json::parse(file);

	for (auto& emitterData : data["Emitters"])
	{
		ParticleSystemEmitterInfo emitterInfo;

		emitterInfo.SettingsPath = emitterData["SettingsPath"];
		//TODO: Load Transform json
		emitterInfo.Transform = {}; //emitterInfo["Transform"] etc.

		ParticleEmitter::Load(emitterInfo.SettingsPath);

		loadedTemplate.EmitterInfos.push_back(emitterInfo);
	}

	DEBUGLOG("Loaded Particle System " + aTemplatePath.filename().string() + " successfully");
	return loadedTemplate;
}