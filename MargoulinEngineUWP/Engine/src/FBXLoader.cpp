#include "FBXLoader.hpp"

#include "../OpenFBX/ofbx.h"
#include <iostream>

auto	FBXLoader::LoadFBXFromFile(MString const& filename) -> void
{
	FILE* fp;
	fopen_s(&fp, filename.Str(), "rb");
	if (!fp) 
		return;

	fseek(fp, 0, SEEK_END);
	long file_size = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	auto* content = new ofbx::u8[file_size];
	fread(content, 1, file_size, fp);
	auto* g_scene = ofbx::load((ofbx::u8*)content, file_size);
	
	for (unsigned int pos = 0; pos < g_scene->getMeshCount(); pos++)
	{
		auto* mesh = g_scene->getMesh(pos);
		auto* geo = mesh->getGeometry();
		mesh->getScene();
	}
}