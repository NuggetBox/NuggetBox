#pragma once
#include "ParticleEmitter.h"
#include "SceneObject.h"

struct ParticleSystemTemplate
{
	std::vector<std::filesystem::path> EmitterPaths;
};

//struct ParticleSystemEmitter
//{
//	ParticleEmitter Emitter;
//	EmitterSettings Settings;
//	Transform transform;
//};

class ParticleSystem : public SceneObject
{
public:
	//TODO: Temp constructor, fix load func
	ParticleSystem() = default;

	void Initialize(const ParticleSystemTemplate& aTemplate);
	void Initialize(const std::filesystem::path& aTemplatePath);

	void Update(float aDeltaTime);

	static ParticleSystemTemplate Load(const std::filesystem::path& aTemplatePath);

	const std::vector<ParticleEmitter>& GetEmitters() const { return myEmitters; }

private:
	//TODO: Assethandling/registry for emitters + systems. Slide 27
	inline static std::unordered_map<std::string, ParticleSystemTemplate> ourSystemTemplateRegistry;

	std::vector<ParticleEmitter> myEmitters;
};