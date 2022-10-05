#pragma once
#include <memory>
#include <vector>

#include "Rendering/Camera.h"
#include "Model/Model.h"
#include "Lights/DirectionalLight.h"
#include "Lights/AmbientLight.h"
#include "Lights/PointLight.h"
#include "Lights/SpotLight.h"
#include "ParticleSystem/ParticleSystem.h"

class Scene
{
public:
	void SetCamera(std::shared_ptr<Camera> aCamera);
	void AddModel(std::shared_ptr<Model> aModel);
	void AddParticleSystem(std::shared_ptr<ParticleSystem> aParticleSystem);
	void SetDirectionalLight(std::shared_ptr<DirectionalLight> aDirectionalLight);
	void SetAmbientLight(std::shared_ptr<AmbientLight> anAmbientLight);
	void AddPointLight(std::shared_ptr<PointLight> aPointLight);
	void AddSpotLight(std::shared_ptr<SpotLight> aSpotLight);
	void BindLightsAsResources();
	void RemoveLightsAsResources();

	std::shared_ptr<Camera>& GetCamera() { return myCamera; }
	std::vector<std::shared_ptr<Model>>& GetModels() { return myModels; }
	const std::vector<std::shared_ptr<ParticleSystem>>& GetParticleSystems() const { return myParticleSystems; }
	const std::shared_ptr<DirectionalLight>& GetDirectionalLight() const { return myDirectionalLight; }
	const std::shared_ptr<AmbientLight>& GetAmbientLight() const { return myAmbientLight; }
	const std::vector<std::shared_ptr<Light>>& GetLights() const { return myLights; }

private:
	std::shared_ptr<Camera> myCamera;
	std::vector<std::shared_ptr<Model>> myModels;
	std::vector<std::shared_ptr<ParticleSystem>> myParticleSystems;
	std::shared_ptr<DirectionalLight> myDirectionalLight;
	std::shared_ptr<AmbientLight> myAmbientLight;
	std::vector<std::shared_ptr<Light>> myLights;
};