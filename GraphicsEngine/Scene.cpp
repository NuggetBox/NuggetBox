#include "NuggetBox.pch.h"
#include "Scene.h"

void Scene::AddGameObject(std::shared_ptr<Model> aModel)
{
	myModels.push_back(std::move(aModel));
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

std::shared_ptr<Camera>& Scene::GetCamera()
{
	return myCamera;
}

std::vector<std::shared_ptr<Model>> const& Scene::GetModels() const
{
	return myModels;
}
