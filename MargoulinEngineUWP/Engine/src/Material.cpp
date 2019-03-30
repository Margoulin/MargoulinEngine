#include "Material.hpp"

#include "Context.hpp"
#include "GPUBuffer.hpp"

auto	Material::Bind(Context* context) -> void
{
	unlitColorBuffer->UpdateBufferData(context, &unlitColor.x);
	unlitColorBuffer->BindBuffer(context);
}

auto	Material::Shutdown() -> void
{
	unlitColorBuffer->Shutdown();
	DEL(unlitColorBuffer);
}

#include <imgui/imgui.h>

#ifdef _DEBUG

auto	Material::ImGuiUpdate() -> void
{
	ImGui::ColorEdit3("Unlit Color", (float*)&unlitColor.x);
}

#endif // _DEBUG
