#include "NuggetBox.pch.h"
#include "RenderTarget.h"

void RenderTarget::SetAsRenderTarget()
{
	DX11::Context->OMSetRenderTargets(1, myRTV.GetAddressOf(), DX11::DepthBuffer.Get());
	DX11::Context->RSSetViewports(1, &myViewport);
}

void RenderTarget::SetAsResource(unsigned aSlot)
{
	DX11::Context->PSSetShaderResources(aSlot, 1, mySRV.GetAddressOf());
}

void RenderTarget::ClearResource(unsigned aSlot)
{
	ID3D11ShaderResourceView* impostorSRV = nullptr;
	DX11::Context->PSSetShaderResources(aSlot, 1, &impostorSRV);
}

void RenderTarget::Clear(const Utility::Vector4f& aClearColor)
{
	DX11::Context->ClearRenderTargetView(myRTV.Get(), &aClearColor.x);
}

std::shared_ptr<RenderTarget> RenderTarget::Create(int aWidth, int aHeight)
{
	RenderTarget renderTarget;

	//Create Texture2D
	D3D11_TEXTURE2D_DESC textureDesc;
	textureDesc.Width = aWidth;
	textureDesc.Height = aHeight;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = 0;
	AssertIfFailed(DX11::Device->CreateTexture2D(&textureDesc, nullptr, renderTarget.myTexture.GetAddressOf()));
	//

	//Create RenderTargetView
	DX11::Device->CreateRenderTargetView(renderTarget.myTexture.Get(), nullptr, renderTarget.myRTV.GetAddressOf());
	//

	//Create ShaderResourceView
	AssertIfFailed(DX11::Device->CreateShaderResourceView(renderTarget.myTexture.Get(), nullptr, renderTarget.mySRV.GetAddressOf()));
	//

	//Create Viewport
	renderTarget.myViewport = D3D11_VIEWPORT(0.0f, 0.0f, aWidth, aHeight, 0.0f, 1.0f);
	//

	return std::make_shared<RenderTarget>(renderTarget);
}