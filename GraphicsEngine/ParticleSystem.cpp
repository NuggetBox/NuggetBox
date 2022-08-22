#include "NuggetBox.pch.h"
#include "ParticleSystem.h"

void ParticleSystem::Initialize(const ParticleSystemTemplate& aTemplate)
{
	for (auto& emitterPath : aTemplate.EmitterPaths)
	{
		ParticleEmitter emitter;
		emitter.Initialize(emitterPath);
		myEmitters.push_back(emitter);
	}
}

void ParticleSystem::Initialize(const std::filesystem::path& aTemplatePath)
{
	Initialize(Load(aTemplatePath.string()));
}

void ParticleSystem::Update(float aDeltaTime)
{
	for (auto& emitter : myEmitters)
	{
		emitter.Update(aDeltaTime);
	}
}

ParticleSystemTemplate ParticleSystem::Load(const std::filesystem::path& aTemplatePath)
{
	//TODO: Read system json, put systemtemplate in registry, should read all emitter json? probably yes
	ParticleSystemTemplate temp;
	temp.EmitterPaths.push_back("yo.json");
	return temp;
}