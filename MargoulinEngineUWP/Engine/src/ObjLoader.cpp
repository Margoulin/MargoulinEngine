#include "ObjLoader.hpp"

#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

#include "MeshResource.hpp"
#include "Mesh.hpp"
#include "Material.hpp"

#include "Engine.hpp"
#include "ResourcesManager.hpp"
#include <fstream>

#include <string>

MString ObjLoader::LocalFolderName = ".";

auto	ObjLoader::LoadObjFromFile(MString const& filename, bool absolutePath) -> bool
{
	ResourcesManager* rsMgr = Engine::GetInstance()->GetService<ResourcesManager>("Resources Manager");

	std::string err;
	tinyobj::attrib_t					attrib;
	std::vector<tinyobj::shape_t>		shapes;
	std::vector<tinyobj::material_t>	materials;

	MString	finalPath = LocalFolderName.Str() + '/';
	finalPath += filename.Str();

	if (tinyobj::LoadObj(&attrib, &shapes, &materials, &err, finalPath.Str(), "./") == false)
		return false;

	unsigned int meshID = rsMgr->CreateMeshResource();
	MeshResource*				meshRsc = (MeshResource*)rsMgr->GetResource(meshID);
	Mesh*						mesh = NEW Mesh();
	meshRsc->SetName(filename);
	
	SubMeshData*				defaultMeshData = nullptr;
	std::vector<SubMeshData*>	subMeshes;

	for (auto&& mat : materials)
	{
		Material*	newMaterial = NEW Material();
		/*
		if (mat.diffuse_texname != "")
		{
			newMaterial->Desc.DiffuseMap = true;
			if (Texture* tex = PNGLoader::LoadPNGFile(("app0:Resources/" + mat.diffuse_texname).c_str()))
				newMaterial->DiffuseMapID = ResourcesManager::Instance->AddTexture(tex)->GetID();
		}
		if (mat.specular_texname != "")
		{
			newMaterial->Desc.SpecularMap = true;
			if (Texture* tex = PNGLoader::LoadPNGFile(("app0:Resources/" + mat.specular_texname).c_str()))
				newMaterial->SpecularMapID = ResourcesManager::Instance->AddTexture(tex)->GetID();
		}
		if (mat.ambient_texname != "")
		{
			newMaterial->Desc.AmbientMap = true;
			if (Texture* tex = PNGLoader::LoadPNGFile(("app0:Resources/" + mat.ambient_texname).c_str()))
				newMaterial->AmbientMapID = ResourcesManager::Instance->AddTexture(tex)->GetID();
		}
		*/
		newMaterial->ka = { mat.ambient[0], mat.ambient[1], mat.ambient[2]};
		newMaterial->kd = { mat.diffuse[0], mat.diffuse[1], mat.diffuse[2]};
		newMaterial->ks = { mat.specular[0], mat.specular[1], mat.specular[2] };
		newMaterial->shininess = mat.shininess;
		newMaterial->name = mat.name.c_str();

		SubMeshData* meshData = NEW SubMeshData();
		subMeshes.push_back(meshData);
		//mesh->MaterialID = ResourcesManager::Instance->AddMaterial(newMaterial)->GetID();
	}

	for (auto&& shape : shapes)
	{
		size_t index_offset = 0;
		for (size_t f = 0; f < shape.mesh.num_face_vertices.size(); f++)
		{
			int currentMaterialID = shape.mesh.material_ids[f];

			SubMeshData* currentMeshData = nullptr;
			if (currentMaterialID < 0 || currentMaterialID >= (int)subMeshes.size())
			{
				if (!defaultMeshData)
					defaultMeshData = NEW SubMeshData();
				currentMeshData = defaultMeshData;
			}
			else
				currentMeshData = subMeshes[currentMaterialID];

			size_t fv = shape.mesh.num_face_vertices[f];
			for (size_t v = 0; v < fv; v++)
			{
				tinyobj::index_t	idx = shape.mesh.indices[index_offset + v];
				float vx = attrib.vertices[3 * idx.vertex_index + 0];
				float vy = attrib.vertices[3 * idx.vertex_index + 1];
				float vz = attrib.vertices[3 * idx.vertex_index + 2];
				float nx = 0.0f, ny = 0.0f, nz = 0.0f;
				if (idx.normal_index != -1)
				{
					nx = attrib.normals[3 * idx.normal_index + 0];
					ny = attrib.normals[3 * idx.normal_index + 1];
					nz = attrib.normals[3 * idx.normal_index + 2];
				}
				float tx = 0.0f, ty = 0.0f;
				if (idx.texcoord_index != -1)
				{
					tx = attrib.texcoords[2 * idx.texcoord_index + 0];
					ty = attrib.texcoords[2 * idx.texcoord_index + 1];
				}

				currentMeshData->AddVertice({vx, vy, vz});
				currentMeshData->AddNormal({nx, ny, nz});
				currentMeshData->AddTexCoords({ tx, 1 - ty });
			}
			index_offset += fv;
		}
	}

	if (defaultMeshData)
		subMeshes.push_back(defaultMeshData);
	
	for (auto& subMesh : subMeshes)
	{
		if (subMesh->GetIndicesCount() == 0)
		{
			for (uint16_t pos = 0u; pos < subMesh->GetVerticesCount(); pos++)
				subMesh->AddIndice(pos);
		}
		mesh->AddMesh(subMesh);
	}
	meshRsc->SetMeshData(mesh);

	return true;
}

auto	ObjLoader::LoadObjFromBinaryFile(MString const& filename, bool absolutePath) -> bool
{
	std::ifstream ifs(filename.Str(), std::ios::in);
	if (!ifs.is_open())
		return false;

	return true;
}

auto	ObjLoader::SaveObjInBinaryFile(Mesh* mesh, MString const& filename, bool absolutePath) -> bool
{
	/*
	SerializeMesh	smesh;

	SubMeshData**	subMeshes = mesh->GetSubMeshTab();
	for (unsigned int pos = 0; pos < mesh->GetSubMeshNbr(); pos++)
		smesh.meshes.push_back(subMeshes[pos]->GetSerializeData());
	std::ofstream ofs(filename, std::ios::out | std::ios::trunc);
	if (ofs.is_open())
	{
		cereal::BinaryOutputArchive oarchive(ofs);
		oarchive(smesh);
		ofs.close();
		return true;
	}
	else
		*/
		return false;
}