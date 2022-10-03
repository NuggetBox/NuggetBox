#pragma once
#include "DX11.h"

class DepthStencil
{
public:
	DepthStencil() = default;
	static std::shared_ptr<DepthStencil> Create(int aWidth, int aHeight);
	void Clear();
	void SetAsRenderTarget();
	void SetAsResource(unsigned aSlot);

private:
	ComPtr<ID3D11Resource> myTexture;
	ComPtr<ID3D11ShaderResourceView> mySRV;
	ComPtr<ID3D11DepthStencilView> myDSV;
	D3D11_VIEWPORT myViewport;
};