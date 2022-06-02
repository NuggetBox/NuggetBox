#pragma once
#include <memory>
#include <vector>

#include "Camera.h"
#include "Model.h"
#include "DirectionalLight.h"
#include "AmbientLight.h"

class Scene
{
public:
	void AddGameObject(std::shared_ptr<Model> aModel);
	void SetCamera(std::shared_ptr<Camera> aCamera);
	void SetDirectionalLight(std::shared_ptr<DirectionalLight> aDirectionalLight);
	void SetAmbientLight(std::shared_ptr<AmbientLight> anAmbientLight);

	std::shared_ptr<Camera>& GetCamera();
	const std::vector<std::shared_ptr<Model>>& GetModels() const;
	const std::shared_ptr<DirectionalLight>& GetDirectionalLight() const { return myDirectionalLight; }
	const std::shared_ptr<AmbientLight>& GetAmbientLight() const { return myAmbientLight; }

private:
	std::shared_ptr<Camera> myCamera;
	std::vector<std::shared_ptr<Model>> myModels;
	std::shared_ptr<DirectionalLight> myDirectionalLight;
	std::shared_ptr<AmbientLight> myAmbientLight;
};