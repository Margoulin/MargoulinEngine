#include "ResourcesManager.hpp"

#include "ObjLoader.hpp"
#include "Mesh.hpp"
#include "SubMeshData.hpp"
#include "MaterialResource.hpp"


auto	ResourcesManager::Initialize() -> void
{
	defaultMeshes = new MeshResource[1];
#pragma region CUBE
	Mesh* cubeMesh = new Mesh();
	defaultMeshes[0].SetMeshData(cubeMesh);
	SubMeshData* cubeData = new SubMeshData();
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
}

auto	ResourcesManager::Shutdown() -> void
{
	for (auto&& resource : resources)
	{
		resource.second->Shutdown();
		delete resource.second;
	}

	for (unsigned int pos = 0; pos < 1; pos++)
		defaultMeshes[pos].Shutdown();
}

auto	ResourcesManager::CreateMeshResource() -> unsigned int
{
	MeshResource* meshResource = new MeshResource();
	return addResource((Resource*)meshResource);
}

auto	ResourcesManager::CreateMaterialResource() -> unsigned int
{
	MaterialResource* matRes = new MaterialResource();
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
			if (ImGui::TreeNode(resource.second->GetName().c_str()))
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
			ObjLoader::LoadObjFromFile(std::string(objFilepath));
		ImGui::TreePop();
	}
}

#endif // _DEBUG