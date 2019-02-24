#include "FBXLoader.hpp"

#include "../OpenFBX/ofbx.h"
#include <iostream>

#include "Mesh.hpp"
#include "MeshResource.hpp"
#include "Engine.hpp"
#include "ResourcesManager.hpp"

#include "SkeletalMesh.hpp"
#include "SkeletalMeshResource.hpp"

auto	FBXLoader::LoadFBXFromFile(MString const& filename) -> void
{
	FILE* fp;
	fopen_s(&fp, filename.Str(), "rb");
	if (!fp) 
		return;

	fseek(fp, 0, SEEK_END);
	long file_size = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	auto* content = NEW ofbx::u8[file_size];
	fread(content, 1, file_size, fp);
	auto* g_scene = ofbx::load((ofbx::u8*)content, file_size);

	ResourcesManager* rsMgr = Engine::GetInstance()->GetService<ResourcesManager>("Resources Manager");
	unsigned int meshID = rsMgr->CreateSkeletalMeshResource();
	SkeletalMeshResource*	meshRsc = (SkeletalMeshResource*)rsMgr->GetResource(meshID);
	
	SkeletalMesh&	skelMesh = meshRsc->GetSkeletalMeshData();
	meshRsc->SetName(filename.Str());

	for (int pos = 0; pos < g_scene->getMeshCount(); pos++)
	{
		auto* fbxMesh = g_scene->getMesh(pos);
		auto* geo = fbxMesh->getGeometry();

		SubMeshData* subMeshData = skelMesh.AddSubMesh();

		ofbx::Vec3 const* const vertices = geo->getVertices();
		ofbx::Vec3 const* const normals = geo->getNormals();
		ofbx::Vec2 const* const uvs = geo->getUVs();
		for (int pos = 0; pos < geo->getVertexCount(); pos++)
		{
			subMeshData->AddVertice(Vector3F((float)vertices[pos].x, (float)vertices[pos].y, (float)vertices[pos].z));
			subMeshData->AddNormal(Vector3F((float)normals[pos].x, (float)normals[pos].y, (float)normals[pos].z));
			subMeshData->AddTexCoords(Vector2F((float)uvs[pos].x, (float)uvs[pos].y));
			subMeshData->AddIndice(pos);
		}
	}
}