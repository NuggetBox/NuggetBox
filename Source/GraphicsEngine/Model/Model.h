#pragma once
#include <unordered_map>

#include "Math/Matrix4x4.hpp"
#include "Scene/SceneObject.h"
#include "Mesh.h"
#include "Core/EngineDefines.h"
#include "Model/Animation/Skeleton.h"
#include "Model/Animation/Animation.h"

//using namespace Microsoft::WRL;
using namespace Utility;

//struct ID3D11Buffer;
//struct ID3D11InputLayout;
//struct ID3D11VertexShader;
//struct ID3D11PixelShader;

struct ModelData
{
	std::vector<Mesh> myMeshes;
	std::shared_ptr<Skeleton> mySkeleton = nullptr;
};

class Model : public SceneObject
{
public:
	Model() = default;
	Model(const ModelData& someModelData);

	const std::vector<Mesh>& GetMeshes() const;

	//TODO: Separate loading and getting? To have more flexibility later when they are called.
	static std::shared_ptr<Model> Load(const std::filesystem::path& aPath);

	void LoadAnimation(const std::filesystem::path& aPath, const std::string& aNewAnimationName);
	void PlayAnimation(const std::string& anAnimationName);
	void Update(bool aLerpAnimations);

	//TODO: Fix something
	void SetSkeleton(const SkeletonData& aSkeleton);
	const std::shared_ptr<Skeleton>& GetSkeleton() const { return myModelData.mySkeleton; }
	bool HasAnimations() const;
	bool HasSkeleton() const;
	bool HasBones() const;

	//TODO: Fix something
	Matrix4f* GetBoneTransforms() { return myBoneTransforms; }

	//:(
	void SetMaterial(std::shared_ptr<Material> aMaterial);

private:
	void AddSubMesh(const Mesh& aMesh);

	//TODO: :eyes: Template registry? :eyes:
	static inline std::unordered_map<std::string, ModelData> ourModelRegistry;
	//TODO: Some sort of animation registry, maybe not AnimationData
	//static std::unordered_map<std::string, AnimationData> ourAnimationRegistry;

	ModelData myModelData;

	/*std::vector<Mesh> myMeshes;
	std::shared_ptr<Skeleton> mySkeleton = nullptr;*/
	//std::shared_ptr<SkeletonData> mySkeleton = nullptr;

	std::unordered_map<std::string, std::shared_ptr<Animation>> myAnimations;
	//std::vector<std::shared_ptr<Animation>> myAnimations;

	//TODO: REFACTOR ANIMATIONS
	Matrix4f myBoneTransforms[MAX_BONES] = { Matrix4f() };
	void UpdateAnimationHierarchy(unsigned aCurrentFrame, unsigned aNextFrame, unsigned aBoneIndex, const Matrix4f& aParentTransform, Matrix4f* outBoneTransforms);
	void LerpAnimationHierarchy(unsigned aCurrentFrame, unsigned aNextFrame, unsigned aBoneIndex, const Matrix4f& aParentTransform, Matrix4f* outBoneTransforms, float aLerpFactor);
	//TODO: Make unique timer for each animation
	float myAnimationTimer = 0;
	//TODO: better solution for current anim
	std::string myCurrentAnim;
};