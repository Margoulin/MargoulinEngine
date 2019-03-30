#include "MaterialResource.hpp"

#include "Material.hpp"

auto	MaterialResource::Shutdown() -> void
{
	materialData->Shutdown();
	DEL(materialData);
}

#include <imgui/imgui.h>
#ifdef _DEBUG

auto	MaterialResource::ImGuiUpdate() -> void
{
	ImGui::Text("Type : Material");
	ImGui::Value("Loaded", loaded);
}

#endif _DEBUG