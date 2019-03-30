#include "TextureResource.hpp"

#include "TextureData.hpp"

auto	TextureResource::Shutdown() -> void
{
	if (textureData)
	{
		textureData->Shutdown();
		DEL(textureData);
	}
}