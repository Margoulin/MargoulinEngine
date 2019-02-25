#include "RendererPipeline.hpp"

#include <imgui.h>
#ifdef _DEBUG
auto	RendererPipeline::ImGuiUpdate() -> void
{
	ImGui::ColorEdit4("Clear color", clearColor);
}
#endif // _DEBUG
