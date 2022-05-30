#pragma once
#include <memory>
#include <vector>

#include "Camera.h"
#include "Model.h"

class Scene
{
public:
	void AddGameObject(std::shared_ptr<Model> aModel);
	void SetCamera(std::shared_ptr<Camera> aCamera);

	std::shared_ptr<Camera>& GetCamera();

	std::vector<std::shared_ptr<Model>> const& GetModels() const;

private:
	std::vector<std::shared_ptr<Model>> myModels;
	std::shared_ptr<Camera> myCamera;
};