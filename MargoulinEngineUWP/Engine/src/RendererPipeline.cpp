#include "RendererPipeline.hpp"

#include "Engine.hpp"
#include "ResourcesManager.hpp"
#include "MaterialResource.hpp"
#include "MeshResource.hpp"

auto	RendererPipeline::DrawCube(unsigned int const& materialID, DirectX::XMMATRIX const& modelMatrix) -> void
{
	ResourcesManager* rsmgr = Engine::GetInstance()->GetService<ResourcesManager>("Resources Manager");
	Resource* matRes = rsmgr->GetResource(materialID);
	if (!matRes)
		return;
	MeshResource* cubeRes = rsmgr->GetDefaultMeshResource(0);
	if (!cubeRes)
		return;
	drawData(cubeRes->GetMeshData(), ((MaterialResource*)matRes)->GetMaterialData(), modelMatrix);
}

auto	RendererPipeline::DrawCustomMesh(unsigned int const& modelID, unsigned int const& materialID, DirectX::XMMATRIX const& modelMatrix) -> void
{
	ResourcesManager* rsmgr = Engine::GetInstance()->GetService<ResourcesManager>("Resources Manager");
	Resource* matRes = rsmgr->GetResource(materialID);
	if (!matRes || matRes->GetType() != Resource::ResourceType::RESOURCE_MATERIAL)
		return;
	Resource* meshRes = rsmgr->GetResource(modelID);
	if (!meshRes || meshRes->GetType() != Resource::ResourceType::RESOURCE_MESH)
		return;
	drawData(((MeshResource*)meshRes)->GetMeshData(), ((MaterialResource*)matRes)->GetMaterialData(), modelMatrix);

}

#include <imgui.h>
#ifdef _DEBUG
auto	RendererPipeline::ImGuiUpdate() -> void
{
	ImGui::ColorEdit4("Clear color", clearColor);
	if (ImGui::TreeNode("Camera"))
	{
		editorCamera.ImGuiUpdate();
		ImGui::TreePop();
	}
}
#endif // _DEBUG
