#ifndef __D3D11_TEXTURE_DATA_HPP__
#define __D3D11_TEXTURE_DATA_HPP__

#include "TextureData.hpp"

#include "CoreMinimal.hpp"

#include "DirectXTex.h"
#include <d3d11.h>
#include <wrl/client.h>

struct D3D11TextureRenderData : public TextureRenderDataBase
{
	ID3D11ShaderResourceView*	shaderView = nullptr;
	ID3D11SamplerState*			samplerState = nullptr;
};


class D3D11TextureData : public TextureData
{
public:
	D3D11TextureData() = default;
	D3D11TextureData(const D3D11TextureData&) = delete;
	D3D11TextureData(D3D11TextureData&&) = delete;
	~D3D11TextureData() = default;

	virtual auto	Shutdown() -> void;

	auto	InitializeD3D11Datas(ID3D11Device* device) -> void;
	auto	CreateTextureRenderData() const -> D3D11TextureRenderData;
	auto	CreateTextureRenderData(Vector4F const& newRect) const -> D3D11TextureRenderData;
	
	auto	SetImageData(DirectX::ScratchImage* dxData) -> void { imageData = dxData; }

	auto	operator = (const D3D11TextureData&)->D3D11TextureData& = delete;
	auto	operator = (D3D11TextureData&&)->D3D11TextureData& = delete;

protected:

private:
	DirectX::ScratchImage*								imageData = nullptr;
	Microsoft::WRL::ComPtr<ID3D11Resource>				d3d11Resource = nullptr;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>	shaderView = nullptr;
	Microsoft::WRL::ComPtr<ID3D11SamplerState>			samplerState = nullptr;
};


#endif /*__D3D11_TEXTURE_DATA_HPP__*/