#ifndef __TEXTURE_RESOURCE_HPP__
#define __TEXTURE_RESOURCE_HPP__

#include "CoreMinimal.hpp"

#include "Resource.hpp"

class TextureData;

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

	auto	GetTextureData() const -> TextureData* { return textureData; }
	auto	SetTextureData(TextureData* data) -> void { textureData = data; }

	auto	operator = (const TextureResource&)->TextureResource& = delete;
	auto	operator = (TextureResource&&)->TextureResource& = delete;

protected:

private:
	TextureData*	textureData = nullptr;

#ifdef _DEBUG

public:
	virtual auto	ImGuiUpdate() -> void {}

private:

#endif // _DEBU
};


#endif /*__TEXTURE_RESOURCE_HPP__*/