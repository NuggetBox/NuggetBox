#include "NuggetBox.pch.h"
#include "HierarchyObject.h"

HierarchyObject::HierarchyObject(const std::string& aName)
{
	SetName(aName);
}

void HierarchyObject::SetName(const std::string& aName)
{
	myName = aName;
}

void HierarchyObject::SetParent(std::shared_ptr<HierarchyObject> aParent)
{
	myParent = aParent;
}

void HierarchyObject::AddChild(std::shared_ptr<HierarchyObject> aChild, std::shared_ptr<HierarchyObject> aNewParent)
{
	myChildren.push_back(aChild);
	aChild->SetParent(aNewParent);
}

void HierarchyObject::RemoveChild(std::shared_ptr<HierarchyObject> aChild)
{
	if (auto iterator = std::find(myChildren.begin(), myChildren.end(), aChild); iterator != myChildren.end())
	{
		myChildren.erase(iterator);
	}
}

void HierarchyObject::SetSelected(bool aSelected)
{
	mySelected = aSelected;
}
