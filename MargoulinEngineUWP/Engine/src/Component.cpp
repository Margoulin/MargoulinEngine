#include "Component.hpp"

#include "Engine.hpp"
#include "ObjectManager.hpp"

#include <imgui.h>

#ifdef _DEBUG

auto	Component::ImGuiUpdate() -> void
{
	if (ImGui::SmallButton("Delete component"))
	{
		ObjectManager* objMgr = Engine::GetInstance()->GetService<ObjectManager>("Object Manager");
		objMgr->DeleteObject(this);
	}
}

#endif