#include "ResourcesManager.hpp"

#include "ObjLoader.hpp"
#include "Mesh.hpp"
#include "SubMeshData.hpp"
#include "MaterialResource.hpp"
#include "SkeletalMeshResource.hpp"

auto	ResourcesManager::Initialize() -> void
{
	defaultMeshes = NEW MeshResource[2];
#pragma region CUBE
	Mesh* cubeMesh = NEW Mesh();
	defaultMeshes[0].SetMeshData(cubeMesh);
	SubMeshData* cubeData = NEW SubMeshData();
	cubeData->AddVertice(Vector3F(-0.5f, -0.5f, -0.5f));
	cubeData->AddVertice(Vector3F(-0.5f, -0.5f, 0.5f));
	cubeData->AddVertice(Vector3F(-0.5f, 0.5f, -0.5f));
	cubeData->AddVertice(Vector3F(-0.5f, 0.5f, 0.5f));
	cubeData->AddVertice(Vector3F(0.5f, -0.5f, -0.5f));
	cubeData->AddVertice(Vector3F(0.5f, -0.5f, 0.5f));
	cubeData->AddVertice(Vector3F(0.5f, 0.5f, -0.5f));
	cubeData->AddVertice(Vector3F(0.5f, 0.5f, 0.5f));

	cubeData->AddIndice(0);
	cubeData->AddIndice(2);
	cubeData->AddIndice(1);
	cubeData->AddIndice(1);
	cubeData->AddIndice(2);
	cubeData->AddIndice(3);

	cubeData->AddIndice(4);
	cubeData->AddIndice(5);
	cubeData->AddIndice(6);
	cubeData->AddIndice(5);
	cubeData->AddIndice(7);
	cubeData->AddIndice(6);

	cubeData->AddIndice(0);
	cubeData->AddIndice(1);
	cubeData->AddIndice(5);
	cubeData->AddIndice(0);
	cubeData->AddIndice(5);
	cubeData->AddIndice(4);

	cubeData->AddIndice(2);
	cubeData->AddIndice(6);
	cubeData->AddIndice(7);
	cubeData->AddIndice(2);
	cubeData->AddIndice(7);
	cubeData->AddIndice(3);

	cubeData->AddIndice(0);
	cubeData->AddIndice(4);
	cubeData->AddIndice(6);
	cubeData->AddIndice(0);
	cubeData->AddIndice(6);
	cubeData->AddIndice(2);

	cubeData->AddIndice(1);
	cubeData->AddIndice(3);
	cubeData->AddIndice(7);
	cubeData->AddIndice(1);
	cubeData->AddIndice(7);
	cubeData->AddIndice(5);
	cubeMesh->AddMesh(cubeData);
#pragma endregion

#pragma region TEXTURE
	Mesh* textureMesh = NEW Mesh();
	defaultMeshes[1].SetMeshData(textureMesh);
	SubMeshData* textureData = NEW SubMeshData();
	textureData->AddVertice(Vector3F(0.0f, 1.0f, 0.0f));
	textureData->AddVertice(Vector3F(1.0f, 1.0f, 0.0f));
	textureData->AddVertice(Vector3F(1.0f, 0.0f, 0.0f));
	textureData->AddVertice(Vector3F(0.0f, 0.0f, 0.0f));
	textureData->AddTexCoords(Vector2F(0.0f, 0.0f));
	textureData->AddTexCoords(Vector2F(1.0f, 0.0f));
	textureData->AddTexCoords(Vector2F(1.0f, 1.0f));
	textureData->AddTexCoords(Vector2F(0.0f, 1.0f));
	textureData->AddIndice(0);
	textureData->AddIndice(1);
	textureData->AddIndice(2);
	textureData->AddIndice(0);
	textureData->AddIndice(2);
	textureData->AddIndice(3);
	textureMesh->AddMesh(textureData);
#pragma endregion

	Service::Initialize();
}

auto	ResourcesManager::Shutdown() -> void
{
	for (auto&& resource : resources)
	{
		resource.second->Shutdown();
		DEL(resource.second);
	}

	for (unsigned int pos = 0; pos < 2; pos++)
		defaultMeshes[pos].Shutdown();
	DELARRAY(defaultMeshes);
}

auto	ResourcesManager::CreateMeshResource() -> unsigned int
{
	MeshResource* meshResource = NEW MeshResource();
	return addResource((Resource*)meshResource);
}

auto	ResourcesManager::CreateSkeletalMeshResource() -> unsigned int
{
	SkeletalMeshResource* skelResource = NEW SkeletalMeshResource();
	return addResource((Resource*)skelResource);
}

auto	ResourcesManager::CreateMaterialResource() -> unsigned int
{
	MaterialResource* matRes = NEW MaterialResource();
	return addResource((Resource*)matRes);
}

auto	ResourcesManager::GetResource(unsigned int const& resourceID) const -> Resource*
{
	auto it = resources.find(resourceID);
	if (it != resources.end())
		return (*it).second;
	return nullptr;
}

auto	ResourcesManager::addResource(Resource* res) -> unsigned int
{
	resources.insert(std::make_pair(totID, res));
	totID++;
	return totID - 1;
}

#include <imgui.h>

#ifdef _DEBUG

auto	ResourcesManager::ImGuiUpdate() -> void
{
	ImGui::Value("Resources Number", (unsigned int)resources.size());
	if (ImGui::TreeNode("Resources"))
	{
		for (auto& resource : resources)
		{
			if (ImGui::TreeNode(resource.second->GetName().Str()))
			{
				ImGui::Value("Resource ID", resource.first);
				resource.second->ImGuiUpdate();
				ImGui::TreePop();
			}
		}
		ImGui::TreePop();
	}
	if (ImGui::TreeNode("Obj Loader"))
	{
		ImGui::InputText("Obj file path", objFilepath, 128, ImGuiInputTextFlags_CharsNoBlank);
		if (ImGui::Button("Load object"))
			ObjLoader::LoadObjFromFile(MString(objFilepath));
		ImGui::TreePop();
	}
}

#endif // _DEBUG