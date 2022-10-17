#pragma once
#include "Math/Matrix4x4.hpp"
#include "Math/Vector.h"
#include "Scene/SceneObject.h"

class Camera : public SceneObject
{
public:
	Camera();
	Camera(float anFov, float aNearPlane, float aFarPlane);

	Utility::Matrix4x4<float> GetProjectionMatrix();

	//Gets full fov in radians
	float GetFov();
	void SetFov(float someDegrees);

	float GetNearPlane() { return myNearPlane; }
	float GetFarPlane() { return myFarPlane; }

private:
	float myFov;
	float myNearPlane;
	float myFarPlane;
};
