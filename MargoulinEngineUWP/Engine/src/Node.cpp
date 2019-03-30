#include "Node.hpp"

#include "Engine.hpp"
#include "Scene.hpp"
#include "Component.hpp"
#include "GraphicComponent.hpp"
#include "Engine.hpp"
#include "ObjectManager.hpp"
#include <algorithm>

Node::Node() :transformation(this) {}

auto	Node::DestroyNode(bool removeFromParent) -> void
{
	if (removeFromParent && parent)
		parent->DetachChild(this);

	for (auto& comp : components)
		comp->DestroyComponent(false);
	for (auto& child : childrens)
		child->DestroyNode(false);

	ObjectManager* objMgr = Engine::GetInstance()->GetService<ObjectManager>("Object Manager");
	objMgr->DeleteObject(this);
}

auto	Node::AddComponent(Component* value) -> void
{
	components.push_back(value);
	value->SetNode(this);
}

auto	Node::AddChild(Node* value) -> void
{
	ObjectManager* objMgr = Engine::GetInstance()->GetService<ObjectManager>("Object Manager");
	if (value != this)
	{
		childrens.push_back(value);
		if (value->parent)
		{
			value->parent->DetachChild(value);
			objMgr->RemoveRelation(value, value->parent);
		}
		value->SetParent(this);
		value->SetScene(this->scene);
		objMgr->AddRelation(this, value);
	}
}

auto	Node::DetachChild(Node* value) -> void
{
	auto it = std::find(childrens.begin(), childrens.end(), value);
	childrens.erase(it);
}

auto	Node::RemoveChild(Node* value) -> void
{
	auto it = std::find(childrens.begin(), childrens.end(), value);
	childrens.erase(it);
	ObjectManager* objMgr = Engine::GetInstance()->GetService<ObjectManager>("Object Manager");
	objMgr->DeleteObject(value);
}

auto	Node::DetachComponent(Component* value) -> void
{
	auto it = std::find(components.begin(), components.end(), value);
	components.erase(it);
}

#include <imgui/imgui.h>
#ifdef _DEBUG

auto	Node::ImGuiUpdate() -> void
{
	MString	idText = "ID : " + MString::FromInt(ID);
	ImGui::Text(idText.Str());
	if (ImGui::SmallButton("Delete node"))
	{
		DestroyNode();
		return;
	}
	if (ImGui::TreeNode("Transform"))
	{
		transformation.ImGuiUpdate();
		ImGui::TreePop();
	}

	if (childrens.size() != 0 && ImGui::TreeNode("Childs"))
	{
		for (auto& child : childrens)
		{
			if (ImGui::TreeNode(child->GetName().Str()))
			{
				child->ImGuiUpdate();
				ImGui::TreePop();
			}
		}
		ImGui::TreePop();
	}
	if (components.size() != 0 && ImGui::TreeNode("Components"))
	{
		for (auto& comp : components)
		{
			if (ImGui::TreeNode(comp->GetComponentTypeName()))
			{
				comp->ImGuiUpdate();
				ImGui::TreePop();
			}
		}
		ImGui::TreePop();
	}
}

#endif // _DEBUG
