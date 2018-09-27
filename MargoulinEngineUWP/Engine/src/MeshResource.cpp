#include "MeshResource.hpp"

#include "Mesh.hpp"

auto	MeshResource::Load() -> void
{

}

auto	MeshResource::Unload() -> void
{

}

auto	MeshResource::Shutdown() -> void
{
	meshData->Shutdown();
}

#include <imgui.h>

#ifdef _DEBUG

auto	MeshResource::ImGuiUpdate() -> void
{
	ImGui::Text("Type : Mesh");
	ImGui::Value("Loaded", loaded);
}

#endif