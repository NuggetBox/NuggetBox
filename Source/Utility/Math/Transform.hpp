#pragma once
#include "Matrix.h"
#include "Vector.h"

class Transform 
{
public:
	Transform();
	Transform(const Transform& aTransform);
	Transform(const Utility::Vector3f& aPosition, const Utility::Vector3f& aRotation, const Utility::Vector3f& aScale);

	void SetPosition(const Utility::Vector3f& aPosition);
	void SetPosition(float aX, float aY, float aZ);
	void SetXPosition(float aX);
	void SetYPosition(float aY);
	void SetZPosition(float aZ);

	void SetRotation(const Utility::Vector3f& aRotation);
	void SetRotation(float aX, float aY, float aZ);
	void SetXRotation(float aX);
	void SetYRotation(float aY);
	void SetZRotation(float aZ);

	void SetScale(Utility::Matrix4f aScaleMatrix);
	void SetScale(Utility::Vector3f aScaleVector);
	void SetScale(float aX, float aY, float az);
	void SetXScale(float aX);
	void SetYScale(float aY);
	void SetZScale(float aZ);

	void AddPosition(Utility::Vector3f someMovement);
	void AddPosition(float aX, float aY, float aZ);

	void AddRotation(Utility::Vector3f someRotation);
	void AddRotation(float aX, float aY, float aZ);

	Utility::Matrix4f GetMatrix();

	Utility::Vector3<float> GetPosition() const;
	float GetXPosition() const;
	float GetYPosition() const;
	float GetZPosition() const;

	Utility::Vector3<float> GetRotation() const;
	Utility::Vector3<float> GetScale() const;

	Utility::Vector3<float> GetRight();
	Utility::Vector3<float> GetLeft();

	Utility::Vector3<float> GetUp();
	Utility::Vector3<float> GetDown();

	Utility::Vector3<float> GetForward();
	Utility::Vector3<float> GetBackward();

private:
	void NormalizeRotations();

	Utility::Matrix4f myCachedMatrix;
	bool myWasModified;

	Utility::Vector3f myPosition;
	Utility::Vector3f myRotation;
	Utility::Vector3f myScale;
};

inline Transform::Transform() : myScale(1, 1, 1)
{
	myWasModified = false;
}

inline Transform::Transform(const Transform& aTransform)
{
	myPosition = aTransform.GetPosition();
	myRotation = aTransform.GetRotation();
	myScale = aTransform.GetScale();
	myCachedMatrix = aTransform.myCachedMatrix;
	myWasModified = false;
}

inline Transform::Transform(const Utility::Vector3f& aPosition, const Utility::Vector3f& aRotation, const Utility::Vector3f& aScale)
{
	myPosition = aPosition;
	myRotation = aRotation;
	myScale = aScale;
	myWasModified = true;
}

inline void Transform::SetPosition(const Utility::Vector3f& aPosition)
{
	myPosition = aPosition;
	myWasModified = true;
}

inline void Transform::SetPosition(float aX, float aY, float aZ)
{
	myPosition = Utility::Vector3f(aX, aY, aZ);
	myWasModified = true;
}

inline void Transform::SetXPosition(float aX)
{
	myPosition.x = aX;
	myWasModified = true;
}

inline void Transform::SetYPosition(float aY)
{
	myPosition.y = aY;
	myWasModified = true;
}

inline void Transform::SetZPosition(float aZ)
{
	myPosition.z = aZ;
	myWasModified = true;
}

inline void Transform::SetRotation(const Utility::Vector3f& aRotation)
{
	myRotation = aRotation;
	myWasModified = true;
}

inline void Transform::SetRotation(float aX, float aY, float aZ)
{
	myRotation.x = aX;
	myRotation.y = aY;
	myRotation.z = aZ;
	myWasModified = true;
}

inline void Transform::SetXRotation(float aX)
{
	myRotation.x = aX;
	myWasModified = true;
}

inline void Transform::SetYRotation(float aY)
{
	myRotation.y = aY;
	myWasModified = true;
}

inline void Transform::SetZRotation(float aZ)
{
	myRotation.z = aZ;
	myWasModified = true;
}

inline void Transform::SetScale(Utility::Matrix4f aScaleMatrix)
{
	myScale.x = aScaleMatrix(1, 1);
	myScale.y = aScaleMatrix(2, 2);
	myScale.z = aScaleMatrix(3, 3);
	myWasModified = true;
}

inline void Transform::SetScale(Utility::Vector3f aScaleVector)
{
	myScale = aScaleVector;
	myWasModified = true;
}

inline void Transform::SetScale(float aX, float aY, float aZ)
{
	myScale.x = aX;
	myScale.y = aY;
	myScale.z = aZ;
	myWasModified = true;
}

inline void Transform::SetXScale(float aX)
{
	myScale.x = aX;
	myWasModified = true;
}

inline void Transform::SetYScale(float aY)
{
	myScale.y = aY;
	myWasModified = true;
}

inline void Transform::SetZScale(float aZ)
{
	myScale.z = aZ;
	myWasModified = true;
}

inline void Transform::AddPosition(Utility::Vector3f someMovement)
{
	myPosition += someMovement;
	myWasModified = true;
}

inline void Transform::AddPosition(float aX, float aY, float aZ)
{
	myPosition.x += aX;
	myPosition.y += aY;
	myPosition.z += aZ;
	myWasModified = true;
}

inline void Transform::AddRotation(Utility::Vector3f someRotation)
{
	myRotation += someRotation;
	NormalizeRotations();
	myWasModified = true;
}

inline void Transform::AddRotation(float aX, float aY, float aZ)
{
	myRotation.x += aX;
	myRotation.y += aY;
	myRotation.z += aZ;
	NormalizeRotations();
	myWasModified = true;
}

inline Utility::Matrix4f Transform::GetMatrix()
{
	if (myWasModified)
	{
		Utility::Matrix4f result;

		Utility::Matrix4f scaleMatrix;
		scaleMatrix(1, 1) = myScale.x;
		scaleMatrix(2, 2) = myScale.y;
		scaleMatrix(3, 3) = myScale.z;

		Utility::Matrix4f rotationX = Utility::Matrix4f::CreateRotationAroundX(DEGTORAD(myRotation.x));
		Utility::Matrix4f rotationY = Utility::Matrix4f::CreateRotationAroundY(DEGTORAD(myRotation.y));
		Utility::Matrix4f rotationZ = Utility::Matrix4f::CreateRotationAroundZ(DEGTORAD(myRotation.z));

		Utility::Matrix4f translationMatrix;
		translationMatrix(4, 1) = myPosition.x;
		translationMatrix(4, 2) = myPosition.y;
		translationMatrix(4, 3) = myPosition.z;

		result *= scaleMatrix;
		result *= rotationX;
		result *= rotationY;
		result *= rotationZ;
		result *= translationMatrix;
		
		myWasModified = false;
		myCachedMatrix = result;
		return result;
	}

	return myCachedMatrix;
}

inline Utility::Vector3f Transform::GetPosition() const
{
	return myPosition;
}

inline float Transform::GetXPosition() const
{
	return myPosition.x;
}

inline float Transform::GetYPosition() const
{
	return myPosition.y;
}

inline float Transform::GetZPosition() const
{
	return myPosition.z;
}

inline Utility::Vector3f Transform::GetRotation() const
{
	return myRotation;
}

inline Utility::Vector3f Transform::GetScale() const
{
	return myScale;
}

inline Utility::Vector3<float> Transform::GetRight()
{
	Utility::Matrix4f transform = GetMatrix();
	return Utility::Vector3f(transform(1, 1), transform(1, 2), transform(1, 3)).GetNormalized();
}

inline Utility::Vector3<float> Transform::GetLeft()
{
	return GetRight() * -1.0f;
}

inline Utility::Vector3<float> Transform::GetUp()
{
	Utility::Matrix4f transform = GetMatrix();
	return Utility::Vector3f(transform(2, 1), transform(2, 2), transform(2, 3)).GetNormalized();
}

inline Utility::Vector3<float> Transform::GetDown()
{
	return GetUp() * -1.0f;
}

inline Utility::Vector3<float> Transform::GetForward()
{
	Utility::Matrix4f transform = GetMatrix();
	return Utility::Vector3f(transform(3, 1), transform(3, 2), transform(3, 3)).GetNormalized();
}

inline Utility::Vector3<float> Transform::GetBackward()
{
	return GetForward() * -1.0f;
}

inline void Transform::NormalizeRotations()
{
	while (myRotation.x >= 360.0f)
	{
		myRotation.x -= 360.0f;
	}
	while (myRotation.x < 0.0f)
	{
		myRotation.x += 360.0f;
	}

	while (myRotation.y >= 360.0f)
	{
		myRotation.y -= 360.0f;
	}
	while (myRotation.y < 0.0f)
	{
		myRotation.y += 360.0f;
	}

	while (myRotation.z >= 360.0f)
	{
		myRotation.z -= 360.0f;
	}
	while (myRotation.z < 0.0f)
	{
		myRotation.z += 360.0f;
	}
}