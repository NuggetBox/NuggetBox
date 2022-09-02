#include "NuggetBox.pch.h"
#include "GBuffer.h"

std::unique_ptr<GBuffer> GBuffer::CreateGBuffer(RECT aClientRect)
{
	GBuffer gBuffer;

	for (uint8_t i = 0; i < GBuffer::GBufferTexture::GB_COUNT; ++i)
	{
		ComPtr<ID3D11Texture2D> texture;
		D3D11_TEXTURE2D_DESC textureDesc = { 0 };
		textureDesc.Width = aClientRect.right - aClientRect.left;
		textureDesc.Height = aClientRect.bottom - aClientRect.top;
		textureDesc.ArraySize = 1;
		textureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		textureDesc.SampleDesc.Count = 1;
		textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
		textureDesc.CPUAccessFlags = 0;
		textureDesc.MiscFlags = 0;
		textureDesc.Usage = D3D11_USAGE_DEFAULT;
		textureDesc.MipLevels = 1;

		AssertIfFailed(DX11::Device->CreateTexture2D(&textureDesc, nullptr, texture.GetAddressOf()));
		AssertIfFailed(DX11::Device->CreateRenderTargetView(texture.Get(), nullptr, &gBuffer.myRTVs[i]));
		AssertIfFailed(DX11::Device->CreateShaderResourceView(texture.Get(), nullptr, &gBuffer.mySRVs[i]));
	}

	return std::make_unique<GBuffer>(gBuffer);
}

void GBuffer::SetAsTarget() const
{
	ID3D11RenderTargetView* myRTVList[GBufferTexture::GB_COUNT];
	for (unsigned i = 0; i < GBufferTexture::GB_COUNT; ++i)
	{
		myRTVList[i] = myRTVs[i].Get();
	}

	DX11::Context->OMSetRenderTargets(GBufferTexture::GB_COUNT, &myRTVList[0], DX11::DepthBuffer.Get());
}

void GBuffer::ClearTarget() const
{
	ID3D11RenderTargetView* emptyViews[GBufferTexture::GB_COUNT];
	for (unsigned i = 0; i < GBufferTexture::GB_COUNT; ++i)
	{
		emptyViews[i] = nullptr;
	}

	DX11::Context->OMSetRenderTargets(GBufferTexture::GB_COUNT, &emptyViews[0], DX11::DepthBuffer.Get());
}

void GBuffer::SetAsResource(unsigned aStartSlot) const
{
	ID3D11ShaderResourceView* mySRVList[GBufferTexture::GB_COUNT];
	for (unsigned i = 0; i < GBufferTexture::GB_COUNT; ++i)
	{
		mySRVList[i] = mySRVs[i].Get();
	}

	DX11::Context->PSSetShaderResources(aStartSlot, GBufferTexture::GB_COUNT, &mySRVList[0]);
}

void GBuffer::ClearResource(unsigned aStartSlot) const
{
	ID3D11ShaderResourceView* emptyResources[GBufferTexture::GB_COUNT];
	for (unsigned i = 0; i < GBufferTexture::GB_COUNT; ++i)
	{
		emptyResources[i] = nullptr;
	}

	DX11::Context->PSSetShaderResources(aStartSlot, GBufferTexture::GB_COUNT, &emptyResources[0]);
}

void GBuffer::Clear()
{
	const FLOAT black[4] = { 0, 0, 0, 0 };

	for (unsigned i = 0; i < GBufferTexture::GB_COUNT; ++i)
	{
		DX11::Context->ClearRenderTargetView(myRTVs[i].Get(), black);
	}
}