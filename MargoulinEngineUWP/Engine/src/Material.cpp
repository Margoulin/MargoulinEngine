#include "Material.hpp"

#include "Shader.hpp"

auto	Material::Bind(ID3D11DeviceContext* context) -> void
{
	attachedShader->SetColor(unlitColor);
	attachedShader->SetCurrent(context);
}

#include <imgui.h>

#ifdef _DEBUG

auto	Material::ImGuiUpdate() -> void
{
	ImGui::ColorEdit3("Unlit Color", (float*)&unlitColor.x);
}

#endif // _DEBUG
