#pragma once
#include <memory>
#include <vector>

class HierarchyObject
{
public:
	HierarchyObject() = default;
	HierarchyObject(const std::string& aName);
	void SetName(const std::string& aName);
	void SetParent(std::shared_ptr<HierarchyObject> aParent);
	void AddChild(std::shared_ptr<HierarchyObject> aChild, std::shared_ptr<HierarchyObject> aNewParent);
	void RemoveChild(std::shared_ptr<HierarchyObject> aChild);

	const std::string& GetName() const { return myName; }
	std::shared_ptr<HierarchyObject> GetParent() const { return myParent; }
	const std::vector<std::shared_ptr<HierarchyObject>>& GetChildren() const { return myChildren; }
	void SetSelected(bool aSelected = true);
	bool GetSelected() const { return mySelected; }

private:
	std::string myName;
	std::shared_ptr<HierarchyObject> myParent;
	std::vector<std::shared_ptr<HierarchyObject>> myChildren;
	bool mySelected = false;
};