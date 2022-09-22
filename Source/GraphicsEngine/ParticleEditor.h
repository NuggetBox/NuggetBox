#pragma once
#include <ParticleSystem/ParticleSystem.h>

class ParticleEditor
{
public:
	ParticleEditor() = default;
	void Initialize();
	void Update(EmitterSettings& aSettings, std::string& aTexturePath);
	void SaveTemplate(const ParticleEmitterTemplate& aTemplate, const std::filesystem::path& aPath);
	ParticleEmitterTemplate LoadTemplate(const std::filesystem::path& aPath);

private:
	void ToolTip(const char* aMessage);
};
