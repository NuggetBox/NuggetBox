#include "NuggetBox.pch.h"
#include "SceneObject.h"

void SceneObject::SetTransform(const Transform& aTransform)
{
    myTransform = aTransform;
}

void SceneObject::SetPosition(const Vector3<float>& aPosition)
{
    myTransform.SetPosition(aPosition);
}

void SceneObject::SetPosition(float anX, float aY, float aZ)
{
    myTransform.SetPosition(anX, aY, aZ);
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

void SceneObject::AddPosition(float anX, float aY, float aZ)
{
    myTransform.AddPosition(anX, aY, aZ);
}

void SceneObject::AddRotation(float anX, float aY, float aZ)
{
    myTransform.AddRotation(anX, aY, aZ);
}

Transform& SceneObject::GetTransform()
{
    return myTransform;
}