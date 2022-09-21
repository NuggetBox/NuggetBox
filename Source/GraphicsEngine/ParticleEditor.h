#pragma once
#include <ParticleSystem/ParticleSystem.h>

class ParticleEditor
{
public:
	ParticleEditor() = default;
	void Initialize();
	void Update(std::vector<ParticleSystem>& outSystems);
	void SaveTemplate(const ParticleEmitterTemplate& aTemplate, const std::filesystem::path& aPath);
	ParticleEmitterTemplate LoadTemplate(const std::filesystem::path& aPath);

private:
	void ToolTip(const char* aMessage);

	ParticleEmitterTemplate myCurrentTemplate;
};
