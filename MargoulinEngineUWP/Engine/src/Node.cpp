#include "Node.hpp"

#include "Engine.hpp"
#include "Scene.hpp"
#include "Component.hpp"
#include "GraphicComponent.hpp"
#include "Engine.hpp"
#include "ObjectManager.hpp"

Node::Node() :transformation(this) {}

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
			value->parent->RemoveChildFromList(value);
			objMgr->RemoveRelation(value, value->parent);
		}
		value->SetParent(this);
		value->SetScene(this->scene);
		objMgr->AddRelation(this, value);
	}
}

auto	Node::RemoveChildFromList(Node* value) -> void
{
	auto it = std::find(childrens.begin(), childrens.end(), value);
	childrens.erase(it);
}

auto	Node::RemoveChild(Node* value) -> void
{
//	ObjectManager* objMgr = Engine::GetInstance()->GetService<ObjectManager>("Object Manager");
	auto it = std::find(childrens.begin(), childrens.end(), value);
	childrens.erase(it);
	value->Shutdown();
	delete value;
}

#include <imgui.h>
#ifdef _DEBUG

auto	Node::ImGuiUpdate() -> void
{
	std::string	idText = "ID : " + std::to_string(ID);
	ImGui::Text(idText.c_str());
	if (ImGui::TreeNode("Transform"))
	{
		transformation.ImGuiUpdate();
		ImGui::TreePop();
	}

	if (childrens.size() != 0 && ImGui::TreeNode("Childs"))
	{
		for (auto& child : childrens)
		{
			if (ImGui::TreeNode(child->GetName().c_str()))
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
