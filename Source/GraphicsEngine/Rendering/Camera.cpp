#include "NuggetBox.pch.h"
#include "Camera.h"

Camera::Camera()
{
	myFov = 90.0f;
	myNearPlane = 1.0f;
	myFarPlane = 15000.f;
}

Camera::Camera(float anFov, float aNearPlane, float aFarPlane)
{
	myFov = anFov;
	myNearPlane = aNearPlane;
	myFarPlane = aFarPlane;
}

Vector4<float> Camera::ConvertToPostProjectionSpace(Vector3<float> aWorldSpacePoint)
{
	Vector4<float> worldPoint;

	worldPoint.x = aWorldSpacePoint.x;
	worldPoint.y = aWorldSpacePoint.y;
	worldPoint.z = aWorldSpacePoint.z;
	worldPoint.w = 1;

	//Put world point in camera space
	Matrix4x4<float> viewMatrix = Matrix4x4<float>::GetFastInverse(myTransform.GetMatrix());
	worldPoint = worldPoint * viewMatrix;

	//Project point onto 2d plane in between far plane and near plane, x,y for coordinates and z should be 0-1 based on how close to near and far plane
	worldPoint = worldPoint * GetProjectionMatrix();

	return worldPoint;
}

float Camera::GetFov()
{
	return DEGTORAD(myFov);
}

void Camera::SetFov(float someDegrees)
{
	myFov = someDegrees;
}

Matrix4x4<float> Camera::GetProjectionMatrix()
{
	return Matrix4f::CreateProjectionMatrixPerspective(1600, 900, myNearPlane, myFarPlane, myFov);
	/*Matrix4x4<float> projectionMatrix;

	const float fovCalc = 1 / tanf(GetFov() * 0.5f);

	projectionMatrix(1, 1) = fovCalc;
	projectionMatrix(2, 2) = 16.0f / 9.0f * fovCalc;
	projectionMatrix(3, 3) = (myFarPlane / (myFarPlane - myNearPlane));
	projectionMatrix(3, 4) = 1.0f;
	projectionMatrix(4, 3) = (-myNearPlane * myFarPlane) / (myFarPlane - myNearPlane);
	projectionMatrix(4, 4) = 0.0f;

	return projectionMatrix;*/
}
