#include "MeshComponent.hpp"

#include "Engine.hpp"
#include "Node.hpp"
#include "NodeTransform.hpp"
#include "GraphicalLibrary.hpp"
#include "RendererPipeline.hpp"

auto	MeshComponent::Draw() -> void
{
	switch (MType)
	{
		case CUSTOM:
		{
			auto* lib = Engine::GetInstance()->GetService<GraphicalLibrary>("Renderer");
			lib->GetRenderPipeline()->DrawCustomMesh(meshID, mat, attachedNode->GetTransform()->GetGlobalMatrix());
			break;
		}
		case CUBE:
		{
			auto* lib = Engine::GetInstance()->GetService<GraphicalLibrary>("Renderer");
			lib->GetRenderPipeline()->DrawCube(mat, attachedNode->GetTransform()->GetGlobalMatrix());
			break;
		}
		case PYRAMID:
		{
			//auto* rend = static_cast<DirectX11Handler*>(Engine::GetInstance()->GetService("Renderer"));
			//rend->DrawPyramid(AttachedNode->GetTransform()->GetGlobalMatrix(), &Mat);
			break;
		}
		case SPHERE:
		{
			//auto* rend = static_cast<DirectX11Handler*>(Engine::GetInstance()->GetService("Renderer"));
			//rend->DrawSphere(AttachedNode->GetTransform()->GetGlobalMatrix(), &Mat);
			break;
		}
		default:
			break;
	}
}

auto	MeshComponent::Shutdown() -> void
{
}

#include <imgui.h>

#ifdef _DEBUG

auto	MeshComponent::ImGuiUpdate() -> void
{
	ImGui::Text("Component Type : Mesh Component");
	ImGui::Combo("Mesh Type", (int*)&MType, "Cube\0Pyramid\0Sphere\0Custom\0");
	if (MType == MESH_TYPE::CUSTOM)
		ImGui::InputInt("Resource Mesh ID", (int*)&meshID);
}

#endif // _DEBUG