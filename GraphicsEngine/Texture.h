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
	static std::unordered_map<std::string, std::shared_ptr<Texture>> ourTextureRegistry;

	//std::string myName;
	
	ComPtr<ID3D11Resource> myTexture;
	// The binding to allow a shader to read from the texture
	ComPtr<ID3D11ShaderResourceView> myShaderResourceView;

	inline static const std::string myDefaultAlbedoPath = "Textures/Defaults/T_Default_C.dds";
	inline static const std::string myDefaultNormalPath = "Textures/Defaults/T_Default_N.dds";
	inline static const std::string myDefaultRoughnessPath = "Textures/Defaults/T_Default_M.dds";
};