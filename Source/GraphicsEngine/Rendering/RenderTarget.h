#pragma once
#include "DX11.h"

class RenderTarget
{
public:
	RenderTarget() = default;
	void SetAsRenderTarget();
	void RemoveRenderTarget();
	void SetAsResource(unsigned aSlot);
	void RemoveResource(unsigned aSlot);
	void Clear(const Utility::Vector4f& aClearColor);

	static std::shared_ptr<RenderTarget> Create(int aWidth, int aHeight, DXGI_FORMAT aFormat);

private:
	ComPtr<ID3D11Texture2D> myTexture;
	ComPtr<ID3D11ShaderResourceView> mySRV;
	ComPtr<ID3D11RenderTargetView> myRTV;
	D3D11_VIEWPORT myViewport;
};