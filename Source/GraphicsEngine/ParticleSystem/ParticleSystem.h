#pragma once
#include "ParticleEmitter.h"
#include "Scene/SceneObject.h"

struct ParticleSystemEmitterInfo
{
	std::string TexturePath;
	std::string SettingsPath;
	Transform Transform;
};

struct ParticleSystemTemplate
{
	std::vector<ParticleSystemEmitterInfo> EmitterInfos;
};

class ParticleSystem : public SceneObject
{
public:
	ParticleSystem() = default;

	void Initialize(const ParticleSystemTemplate& aTemplate);
	void LoadAndInitialize(const std::filesystem::path& aTemplatePath);

	void Update();

	static ParticleSystemTemplate Load(const std::filesystem::path& aTemplatePath);

	const std::vector<ParticleEmitter>& GetEmitters() const { return myEmitters; }

//TODO: ParticleSystemFactory?

private:
	inline static std::unordered_map<std::string, ParticleSystemTemplate> ourSystemTemplateRegistry;

	std::vector<ParticleEmitter> myEmitters;
};