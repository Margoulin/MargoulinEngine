#ifndef __TEXTURE_DATA_HPP__
#define __TEXTURE_DATA_HPP__

#include <MUtils/Maths/Vector.hpp>

struct TextureRenderDataBase
{
	Vector4F	textureRect;
};

class TextureData
{
public:
	TextureData() = default;
	TextureData(const TextureData&) = delete;
	TextureData(TextureData&&) = delete;
	~TextureData() = default;

	virtual auto	Shutdown() -> void = 0;

	auto	operator = (const TextureData&)->TextureData& = delete;
	auto	operator = (TextureData&&)->TextureData& = delete;

protected:
	unsigned int				width = 0;
	unsigned int				height = 0;

private:

};


#endif /*__TEXTURE_DATA_HPP__*/