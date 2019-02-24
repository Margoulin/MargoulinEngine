#ifndef __TEXTURE_RESOURCE_HPP__
#define __TEXTURE_RESOURCE_HPP__

#include "CoreMinimal.hpp"

#include "Resource.hpp"

#include "DirectXTex.h"
#include <d3d11.h>
#include <wrl/client.h>

struct TextureRenderData
{
	ID3D11ShaderResourceView*	shaderView = nullptr;
	ID3D11SamplerState*			samplerState = nullptr;
	Vector4F					textureRect;
};

class TextureResource : public Resource
{
public:
	TextureResource() { type = ResourceType::RESOURCE_TEXTURE; }
	TextureResource(const TextureResource&) = delete;
	TextureResource(TextureResource&&) = delete;
	~TextureResource() = default;

	virtual auto	Load() -> void {}
	virtual auto	Unload() -> void {}
	virtual auto	Shutdown() -> void;

	auto	InitializeD3D11Datas(ID3D11Device* device) -> void;
	auto	CreateTextureRenderData() const -> TextureRenderData;
	auto	CreateTextureRenderData(Vector4F const& newRect) const -> TextureRenderData;

	auto	SetImageData(DirectX::ScratchImage* dxData) -> void { imageData = dxData; }

	auto	GetD3D11Resource() const -> ID3D11Resource* const { return d3d11Resource.Get(); }
	auto	GetShaderView() const -> ID3D11ShaderResourceView* const { return shaderView.Get(); }
	auto	GetSamplerState() const -> ID3D11SamplerState* const { return samplerState.Get(); }

	auto	operator = (const TextureResource&)->TextureResource& = delete;
	auto	operator = (TextureResource&&)->TextureResource& = delete;

protected:

private:
	DirectX::ScratchImage*		imageData = nullptr;
	Microsoft::WRL::ComPtr<ID3D11Resource>				d3d11Resource = nullptr;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>	shaderView = nullptr;
	Microsoft::WRL::ComPtr<ID3D11SamplerState>			samplerState = nullptr;
	unsigned int				width = 0;
	unsigned int				height = 0;

#ifdef _DEBUG

public:
	virtual auto	ImGuiUpdate() -> void {}

#endif // _DEBU
};


#endif /*__TEXTURE_RESOURCE_HPP__*/