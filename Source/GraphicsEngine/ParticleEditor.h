#pragma once
#include <ParticleSystem/ParticleSystem.h>

class ParticleEditor
{
public:
	ParticleEditor() = default;
	void Initialize();
	void Update(std::vector<ParticleSystem>& outSystems);

private:
	void ToolTip(const char* aMessage);

	ParticleEmitterTemplate myCurrentTemplate;
};
