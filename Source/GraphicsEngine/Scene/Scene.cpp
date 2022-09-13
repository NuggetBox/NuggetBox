#include "NuggetBox.pch.h"
#include "Scene.h"

void Scene::AddModel(std::shared_ptr<Model> aModel)
{
	myModels.push_back(std::move(aModel));
}

void Scene::AddParticleSystem(std::shared_ptr<ParticleSystem> aParticleSystem)
{
	myParticleSystems.push_back(std::move(aParticleSystem));
}

void Scene::SetCamera(std::shared_ptr<Camera> aCamera)
{
	myCamera = std::move(aCamera);
}

void Scene::SetDirectionalLight(std::shared_ptr<DirectionalLight> aDirectionalLight)
{
	myDirectionalLight = aDirectionalLight;
}

void Scene::SetAmbientLight(std::shared_ptr<AmbientLight> anAmbientLight)
{
	myAmbientLight = anAmbientLight;
}

void Scene::AddPointLight(std::shared_ptr<PointLight> aPointLight)
{
	myLights.push_back(aPointLight);
}

void Scene::AddSpotLight(std::shared_ptr<SpotLight> aSpotLight)
{
	myLights.push_back(aSpotLight);
}
