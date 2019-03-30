#include "D3D11TextureData.hpp"

#include <MUtils/Maths/Math.hpp>

auto	D3D11TextureData::Shutdown() -> void
{
	DEL(imageData);

	*d3d11Resource.ReleaseAndGetAddressOf() = nullptr;
	*shaderView.ReleaseAndGetAddressOf() = nullptr;
	*samplerState.ReleaseAndGetAddressOf() = nullptr;
}

auto	D3D11TextureData::InitializeD3D11Datas(ID3D11Device* device) -> void
{
	if (!imageData)
		return;

	DirectX::CreateTexture(device, imageData->GetImages(), imageData->GetImageCount(), imageData->GetMetadata(), d3d11Resource.GetAddressOf());

	D3D11_SHADER_RESOURCE_VIEW_DESC	viewDesc;
	ZeroMemory(&viewDesc, sizeof(viewDesc));

	D3D11_TEXTURE2D_DESC texDesc;
	ID3D11Texture2D* d3d11Tex = nullptr;
	d3d11Resource.Get()->QueryInterface(__uuidof(ID3D11Texture2D), (void**)(&d3d11Tex));
	d3d11Tex->GetDesc(&texDesc);
	viewDesc.Format = texDesc.Format;
	viewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	viewDesc.Texture2D.MipLevels = 1;
	device->CreateShaderResourceView(d3d11Resource.Get(), &viewDesc, &shaderView);

	D3D11_SAMPLER_DESC sampDesc;
	ZeroMemory(&sampDesc, sizeof(sampDesc));
	sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sampDesc.MinLOD = 0;
	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
	device->CreateSamplerState(&sampDesc, &samplerState);

	width = texDesc.Width;
	height = texDesc.Height;
}

auto	D3D11TextureData::CreateTextureRenderData() const -> D3D11TextureRenderData
{
	D3D11TextureRenderData ret;
	ret.samplerState = samplerState.Get();
	ret.shaderView = shaderView.Get();
	ret.textureRect.x = 0.0f;
	ret.textureRect.y = 0.0f;
	ret.textureRect.z = 1.0f;
	ret.textureRect.w = 1.0f;
	return ret;
}

auto	D3D11TextureData::CreateTextureRenderData(Vector4F const& newRect) const -> D3D11TextureRenderData
{
	D3D11TextureRenderData ret;
	ret.samplerState = samplerState.Get();
	ret.shaderView = shaderView.Get();

	ret.textureRect.x = Clamp01(newRect.x / width);
	ret.textureRect.y = Clamp01(newRect.y / height);
	ret.textureRect.w = Clamp01((newRect.x + newRect.w) / width);
	ret.textureRect.z = Clamp01((newRect.y + newRect.z) / height);
	return ret;
}