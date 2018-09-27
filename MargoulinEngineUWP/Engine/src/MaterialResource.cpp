#include "MaterialResource.hpp"

#include "Material.hpp"

auto	MaterialResource::Shutdown() -> void
{
	delete materialData;
}

#include <imgui.h>
#ifdef _DEBUG

auto	MaterialResource::ImGuiUpdate() -> void
{
	ImGui::Text("Type : Material");
	ImGui::Value("Loaded", loaded);
}

#endif _DEBUG