#include "NuggetBox.pch.h"
#include "SceneObject.h"

void SceneObject::ShouldSpin()
{
    mySpin = true;
}

void SceneObject::SetTransform(const Transform& aTransform)
{
    myTransform = aTransform;
}

void SceneObject::SetPosition(const Vector3<float>& aPosition)
{
    myTransform.SetPosition(aPosition);
}

void SceneObject::SetPosition(float aX, float aY, float aZ)
{
    myTransform.SetPosition(aX, aY, aZ);
}

void SceneObject::SetRotation(const Vector3<float>& aRotation)
{
    myTransform.SetRotation(aRotation);
}

void SceneObject::SetScale(const Vector3<float>& aScale)
{
    myTransform.SetScale(aScale);
}

void SceneObject::AddPosition(const Vector3<float>& aPosition)
{
    myTransform.AddPosition(aPosition);
}

void SceneObject::AddPosition(float aX, float aY, float aZ)
{
    myTransform.AddPosition(aX, aY, aZ);
}

void SceneObject::AddRotation(float aX, float aY, float aZ)
{
    myTransform.AddRotation(aX, aY, aZ);
}

Transform& SceneObject::GetTransform()
{
    return myTransform;
}