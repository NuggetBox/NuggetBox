#pragma once
#include "Math/Transform.hpp"

class SceneObject
{
public:
	SceneObject() = default;
	virtual ~SceneObject() = default;

	void ShouldSpin();
	bool GetShouldSpin() { return mySpin; }

	void SetTransform(const Transform& aTransform);
	void SetPosition(const Vector3<float>& aPosition);
	void SetPosition(float aX, float aY, float aZ);
	void SetRotation(const Vector3<float>& aRotation);
	void SetScale(const Vector3<float>& aScale);

	void AddPosition(const Vector3<float>& aPosition);
	void AddPosition(float aX, float aY, float aZ);
	void AddRotation(float aX, float aY, float aZ);

	Transform& GetTransform();
	
protected:
	Transform myTransform;
	bool mySpin = false;
};