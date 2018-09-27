#ifndef __OBJLOADER_HPP__
#define __OBJLOADER_HPP__

#include <string>

class Mesh;

class ObjLoader
{
public:
	ObjLoader() = default;
	ObjLoader(const ObjLoader&) = delete;
	ObjLoader(ObjLoader&&) = delete;
	~ObjLoader() = default;

	static	auto	LoadObjFromFile(std::string const& filename, bool absolutePath = false) -> bool;
	static	auto	LoadObjFromBinaryFile(std::string const& filepath, bool absolutePath = false) -> bool;
	static	auto	SaveObjInBinaryFile(Mesh* meshToSerialize, std::string const& filename, bool absolutePath = false) -> bool;

	static	std::string	LocalFolderName;

	auto	operator = (const ObjLoader&)->ObjLoader& = delete;
	auto	operator = (ObjLoader&&)->ObjLoader& = delete;

protected:

private:

};


#endif /*__OBJLOADER_HPP__*/