#include "Scene.hpp"

#include "Node.hpp"
#include "MeshComponent.hpp"
#include "Renderer2DComponent.hpp"
#include "CameraComponent.hpp"

#include "Engine.hpp"
#include "ObjectManager.hpp"

#include <imgui/imgui.h>

#include "ObjectHandle.inl"

Scene::Scene()
{
	root = NEW Node();
	root->SetName("Name");

	ObjectManager* objMgr = Engine::GetInstance()->GetService<ObjectManager>("Object Manager");

	Node* node = objMgr->Create<Node>();
	node->SetName("Cube");
	node->Initialize();
	node->GetTransform()->SetPosition(Vector3F(0.0f, 0.0f, 0.0f));
	auto* cp1 = objMgr->Create<MeshComponent>();
	cp1->SetMeshType(MeshComponent::CUBE);
	cp1->SetCustomMesh(0);
	cp1->SetMaterial(0);
	node->AddComponent(cp1);
	AddNode(node);
	/*

	node = objMgr->Create<Node>();
	node->SetName("Rectangle");
	node->Initialize();
	auto* comp = objMgr->Create<Renderer2DComponent>();
	node->AddComponent(comp);
	AddNode(node);

	Node* camNode = objMgr->Create<Node>();
	camNode->SetName("CameraNode");
	camNode->Initialize();
	camNode->AddComponent(objMgr->Create<CameraComponent>());
	AddNode(camNode);
	*/
}

auto	Scene::AddNode(Node* value) -> void
{
	value->SetScene(this);
	root->AddChild(value);
}

auto	Scene::FindNode(unsigned int const& value) -> Node*
{
	if (value == 0)
		return root;
	return nullptr;// root->FindChild(value);
}

auto	Scene::Shutdown() -> void
{
	root->DestroyNode(false);
	DEL(root);
}

#ifdef _DEBUG

auto	Scene::ImGuiUpdate() -> void
{
	for (auto& child : root->GetChildrens())
	{
		if (ImGui::TreeNode(child->GetName().Str()))
		{
			child->ImGuiUpdate();
			ImGui::TreePop();
		}
	}
}

#endif // _DEBUG
