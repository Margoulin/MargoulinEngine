#include "Component.hpp"

#include "Engine.hpp"
#include "ObjectManager.hpp"

#include "Node.hpp"
#include <imgui.h>

auto	Component::DestroyComponent(bool removeFromNode) -> void
{
	if (removeFromNode && attachedNode)
		attachedNode->DetachComponent(this);
	ObjectManager* objMgr = Engine::GetInstance()->GetService<ObjectManager>("Object Manager");
	objMgr->DeleteObject(this);
}

#ifdef _DEBUG

auto	Component::ImGuiUpdate() -> void
{
	if (ImGui::SmallButton("Delete component"))
	{
		DestroyComponent();
	}
}

#endif