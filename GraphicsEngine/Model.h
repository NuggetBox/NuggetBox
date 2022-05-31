#pragma once
#include <unordered_map>

#include "Math/Matrix4x4.hpp"
#include "SceneObject.h"
#include "Mesh.h"
#include "EngineDefines.h"

//using namespace Microsoft::WRL;
using namespace Utility;

//struct ID3D11Buffer;
//struct ID3D11InputLayout;
//struct ID3D11VertexShader;
//struct ID3D11PixelShader;

//TODO: Fix registry to use this struct instead
struct ModelData
{
	std::vector<Mesh> myMeshes;
	//TODO: Make skeletondata just that, only data, animations should be stored per model!
	std::shared_ptr<SkeletonData> mySkeleton = nullptr;
};

class Model : public SceneObject
{
public:
	Model() = default;

	const std::vector<Mesh>& GetMeshes() const;

	void AddSubMesh(const Mesh& aMesh);

	//TODO: Separate loading and getting? To have more flexibility later when they are called.
	static std::shared_ptr<Model> Load(const std::filesystem::path& aFath);

	void LoadAnimation(const std::filesystem::path& aFilepath, const std::string& aNewAnimationName);
	void PlayAnimation(const std::string& anAnimationName);

	//TODO: Fix something
	void SetSkeleton(const SkeletonData& aSkeleton);
	const std::shared_ptr<SkeletonData>& GetSkeleton() const { return mySkeleton; }
	bool HasSkeleton() const;
	void Update();

	//TODO: Fix something
	Matrix4f* GetBoneTransforms() { return myBoneTransforms; }
	bool HasBones() const;

private:
	static std::unordered_map<std::string, std::shared_ptr<Model>> ourModelRegistry;
	//TODO: Some sort of animation registry, maybe not AnimationData
	//static std::unordered_map<std::string, AnimationData> ourAnimationRegistry;

	std::vector<Mesh> myMeshes;
	std::shared_ptr<SkeletonData> mySkeleton = nullptr;

	//TODO: REFACTOR ANIMATIONS
	Utility::Matrix4f myBoneTransforms[MAX_BONES] = { Matrix4f() };
	void UpdateAnimationHierarchy(unsigned aCurrentFrame, unsigned aNextFrame, unsigned aBoneIndex, const Matrix4f& aParentTransform, Matrix4f* outBoneTransforms);
	//TODO: Make unique timer for each animation
	float myAnimationTimer = 0;
	//TODO: better solution for current anim
	std::string myCurrentAnim;
};