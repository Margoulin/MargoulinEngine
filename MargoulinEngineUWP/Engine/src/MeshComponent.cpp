#include "MeshComponent.hpp"

#include "Engine.hpp"
#include "Node.hpp"
#include "NodeTransform.hpp"
#include "GraphicalLibrary.hpp"
#include "RendererPipeline.hpp"

#include <imgui.h>

#ifdef _DEBUG

auto	MeshComponent::ImGuiUpdate() -> void
{
	Component::ImGuiUpdate();
	ImGui::Text("Component Type : Mesh Component");
	ImGui::Combo("Mesh Type", (int*)&meshType, "Cube\0Pyramid\0Sphere\0Custom\0");
	if (meshType == MESH_TYPE::CUSTOM)
		ImGui::InputInt("Resource Mesh ID", (int*)&meshID);
}

#endif // _DEBUG