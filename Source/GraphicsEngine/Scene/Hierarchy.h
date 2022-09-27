#pragma once
#include "HierarchyObject.h"

class Hierarchy
{
public:
	Hierarchy() = default;

	static void Initialize();
	static void Update();

	static void DrawObject(std::shared_ptr<HierarchyObject> aObject);

private:
	static inline std::shared_ptr<HierarchyObject> myRoot;

	static inline std::vector<std::shared_ptr<HierarchyObject>> mySelectedObjects;
};