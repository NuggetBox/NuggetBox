#pragma once
#include <filesystem>
#include <unordered_map>

#include "DX11.h"

class Texture
{
public:
	static std::shared_ptr<Texture> Load(const std::filesystem::path& aPath);
	void SetAsResource(UINT aSlot);

	virtual ~Texture();

private:
	inline static std::unordered_map<std::string, std::shared_ptr<Texture>> ourTextureRegistry;
	//std::string myName;
	
	ComPtr<ID3D11Resource> myTexture;
	// The binding to allow a shader to read from the texture
	ComPtr<ID3D11ShaderResourceView> myShaderResourceView;

	//TODO: Use
	//inline static const std::string myAlbedoPrefix = "T_";
	//inline static const std::string myAlbedoSuffix = "_C";
	//inline static const std::string myNormalSuffix = "_N";
	//inline static const std::string mySurfaceSuffix = "_M";

	inline static const std::string myDefaultAlbedoPath = "Textures/Defaults/T_Default_C.dds";
	inline static const std::string myDefaultNormalPath = "Textures/Defaults/T_Default_N.dds";
	inline static const std::string myDefaultRoughnessPath = "Textures/Defaults/T_Default_M.dds";
};