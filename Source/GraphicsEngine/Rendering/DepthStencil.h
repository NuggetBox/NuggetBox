#pragma once
#include "DX11.h"

class DepthStencil
{
public:
	DepthStencil() = default;
	static std::shared_ptr<DepthStencil> Create(const std::string& aName, int aWidth, int aHeight);
	void Clear();
	void SetRenderTarget();
	void BindShadowResource(unsigned aSlot);

private:
	std::string myName;
	ComPtr<ID3D11Resource> myTexture;
	ComPtr<ID3D11ShaderResourceView> mySRV;
	ComPtr<ID3D11DepthStencilView> myDSV;
	D3D11_VIEWPORT myViewport;
};