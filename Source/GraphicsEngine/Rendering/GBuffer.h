#pragma once
#include "DX11.h"

class GBuffer
{
public:
	enum GBufferTexture
	{
		GB_ALBEDO,
		GB_NORMAL,
		GB_MATERIAL,
		GB_VERTEXNORMAL,
		GB_POSITION,
		GB_AMBIENTOCCLUSION,
		GB_COUNT
	};

	static std::unique_ptr<GBuffer> CreateGBuffer(RECT aClientRect);

	void SetAsRenderTarget() const;
	void ClearRenderTarget() const;
	void SetAsResource(unsigned aStartSlot) const;
	void RemoveResource(unsigned aStartSlot) const;
	void Clear();

private:
	std::array<ComPtr<ID3D11RenderTargetView>, GBufferTexture::GB_COUNT> myRTVs;
	std::array<ComPtr<ID3D11ShaderResourceView>, GBufferTexture::GB_COUNT> mySRVs;
};