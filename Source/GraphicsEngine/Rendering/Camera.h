#pragma once
#include "Math/Matrix4x4.hpp"
#include "Math/Vector.h"
#include "Scene/SceneObject.h"

using namespace Utility;

class Camera : public SceneObject
{
public:
	Camera();
	Camera(float anFov, float aNearPlane, float aFarPlane);

	Matrix4x4<float> GetProjectionMatrix();

	Vector4<float> ConvertToPostProjectionSpace(Vector3<float> aWorldSpacePoint);

	//Gets full fov in radians
	float GetFov();
	void SetFov(float someDegrees);

private:
	float myFov;
	float myNearPlane;
	float myFarPlane;
};
