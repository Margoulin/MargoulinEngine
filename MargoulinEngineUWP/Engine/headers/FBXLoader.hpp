#ifndef __FBXLOADER_HPP__
#define __FBXLOADER_HPP__

#include "../MUtils/MUtils/String.hpp"

class FBXLoader
{
public:
	FBXLoader() = delete;
	FBXLoader(const FBXLoader&) = delete;
	FBXLoader(FBXLoader&&) = delete;
	~FBXLoader() = default;

	static auto	LoadFBXFromFile(MString const& filename) -> void;

	auto	operator = (const FBXLoader&)->FBXLoader& = delete;
	auto	operator = (FBXLoader&&)->FBXLoader& = delete;

protected:

private:

};


#endif /*__FBXLOADER_HPP__*/