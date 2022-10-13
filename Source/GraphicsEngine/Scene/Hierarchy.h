#pragma once
#include "HierarchyObject.h"
#include "json.hpp"

class Hierarchy
{
public:
	Hierarchy() = default;

	static void Initialize();
	static void Update();

	static void DrawObject(std::shared_ptr<HierarchyObject> aObject);

	static void SaveHierarchy();
	static void SaveObject(const std::shared_ptr<HierarchyObject>& aObject, nlohmann::basic_json<>& aJson);
	static void LoadHierarchy();
	static void LoadObject(std::shared_ptr<HierarchyObject>& aObject, const nlohmann::basic_json<>& aJson);

private:
	static inline std::shared_ptr<HierarchyObject> myRoot;
	static inline std::vector<std::shared_ptr<HierarchyObject>> mySelectedObjects;

	static inline std::string myInputBuffer;
};
