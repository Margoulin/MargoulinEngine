#ifndef __OBJLOADER_HPP__
#define __OBJLOADER_HPP__

#include "CoreMinimal.hpp"

class Mesh;

class ObjLoader
{
public:
	ObjLoader() = default;
	ObjLoader(const ObjLoader&) = delete;
	ObjLoader(ObjLoader&&) = delete;
	~ObjLoader() = default;

	static	auto	LoadObjFromFile(MString const& filename, bool absolutePath = false) -> bool;
	static	auto	LoadObjFromBinaryFile(MString const& filepath, bool absolutePath = false) -> bool;
	static	auto	SaveObjInBinaryFile(Mesh* meshToSerialize, MString const& filename, bool absolutePath = false) -> bool;

	static	MString	LocalFolderName;

	auto	operator = (const ObjLoader&)->ObjLoader& = delete;
	auto	operator = (ObjLoader&&)->ObjLoader& = delete;

protected:

private:

};


#endif /*__OBJLOADER_HPP__*/