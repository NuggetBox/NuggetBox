#include "NuggetBox.pch.h"
#include "DepthStencil.h"

std::shared_ptr<DepthStencil> DepthStencil::Create(const std::string& aName, int aWidth, int aHeight)
{
	DepthStencil depthStencil;

	depthStencil.myName = aName;

	//Create Resource/Texture2D
	D3D11_TEXTURE2D_DESC textureDesc;
	textureDesc.Width = aWidth;
	textureDesc.Height = aHeight;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_R32_TYPELESS;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_DEPTH_STENCIL;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = 0;
	AssertIfFailed(DX11::Device->CreateTexture2D(&textureDesc, nullptr, reinterpret_cast<ID3D11Texture2D**>(depthStencil.myTexture.GetAddressOf())));
	//

	//Create ShaderResourceView
	D3D11_SHADER_RESOURCE_VIEW_DESC resourceDesc{};
	resourceDesc.Format = DXGI_FORMAT_R32_FLOAT;
	resourceDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	resourceDesc.Texture2D.MipLevels = textureDesc.MipLevels;
	AssertIfFailed(DX11::Device->CreateShaderResourceView(depthStencil.myTexture.Get(), &resourceDesc, depthStencil.mySRV.GetAddressOf()));
	//

	//Create DepthStencilView
	D3D11_DEPTH_STENCIL_VIEW_DESC depthDesc{};
	depthDesc.Format = DXGI_FORMAT_D32_FLOAT;
	depthDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	AssertIfFailed(DX11::Device->CreateDepthStencilView(depthStencil.myTexture.Get(), &depthDesc, depthStencil.myDSV.GetAddressOf()));
	//

	//Create viewport
	depthStencil.myViewport = D3D11_VIEWPORT(0.0f, 0.0f, aWidth, aHeight, 0.0f, 1.0f);
	//

	return std::make_shared<DepthStencil>(depthStencil);
}

void DepthStencil::Clear()
{
	DX11::Context->ClearDepthStencilView(myDSV.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}

void DepthStencil::SetRenderTarget()
{
	ID3D11RenderTargetView* impostorBackBuffer = nullptr;
	DX11::Context->OMSetRenderTargets(1, &impostorBackBuffer, myDSV.Get());
	DX11::Context->RSSetViewports(1, &myViewport);
}

void DepthStencil::BindShadowResource(unsigned aSlot)
{
	DX11::Context->PSSetShaderResources(aSlot, 1, mySRV.GetAddressOf());
}
