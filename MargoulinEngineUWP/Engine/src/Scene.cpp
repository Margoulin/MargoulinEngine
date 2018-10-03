#include "Scene.hpp"

#include "Node.hpp"
#include "MeshComponent.hpp"
#include "Renderer2DComponent.hpp"
#include "CameraComponent.hpp"

#include "Engine.hpp"
#include "ObjectManager.hpp"

#include <imgui.h>

Scene::Scene()
{
	root = new Node();
	root->SetName("Name");

	ObjectManager* objMgr = Engine::GetInstance()->GetService<ObjectManager>("Object Manager");

	Node* camNode = objMgr->Create<Node>();
	camNode->SetName("Camera Node");
	camNode->Initialize();
	camNode->GetTransform()->Translate(Vector3F(0.0f, 0.0f, 1.5f));
	auto* camComp = objMgr->Create<CameraComponent>();
	camComp->Initialize();
	camNode->AddComponent(camComp);
	AddNode(camNode);

	Node* node2 = objMgr->Create<Node>();
	//Node* node2 = new Node();
	node2->SetName("Testing Node 2");
	node2->Initialize();
	auto* cp1 = objMgr->Create<MeshComponent>();
	cp1->SetMeshType(MeshComponent::CUBE);
	cp1->SetCustomMesh(0);
	cp1->SetMaterial(0);
	node2->AddComponent(cp1);
	AddNode(node2);

	Node* node = objMgr->Create<Node>();
	//Node* node = new Node();
	node->SetName("Rectangle");
	node->Initialize();
	auto* comp = objMgr->Create<Renderer2DComponent>();
	node->AddComponent(comp);
	AddNode(node);
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
	root->Shutdown();
	delete root;
}

#ifdef _DEBUG

auto	Scene::ImGuiUpdate() -> void
{
	for (auto& child : root->GetChildrens())
	{
		if (ImGui::TreeNode(child->GetName().c_str()))
		{
			child->ImGuiUpdate();
			ImGui::TreePop();
		}
	}
}

#endif // _DEBUG
