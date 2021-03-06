#ifndef __TEXTURE_LOADER_HPP__
#define __TEXTURE_LOADER_HPP__

#include <MUtils/String.hpp>

class TextureLoader
{
public:
	TextureLoader() = delete;
	TextureLoader(const TextureLoader&) = delete;
	TextureLoader(TextureLoader&&) = delete;
	~TextureLoader() = default;

	static auto	LoadTextureFromFile(MString const& filename, bool const absolutePath = false) -> bool;

	static	MString	LocalFolderName;

	auto	operator = (const TextureLoader&)->TextureLoader& = delete;
	auto	operator = (TextureLoader&&)->TextureLoader& = delete;

protected:

private:
};

#endif /*__TEXTURE_LOADER_HPP__*/