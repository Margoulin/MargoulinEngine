#include "SkeletalMeshComponent.hpp"

#include "Engine.hpp"
#include "GraphicalLibrary.hpp"

auto	SkeletalMeshComponent::Draw() -> void
{
	auto* lib = Engine::GetInstance()->GetService<GraphicalLibrary>("Renderer");
	//lib->GetRenderPipeline()->DrawCustomMesh(meshID, mat, attachedNode->GetTransform()->GetGlobalMatrix());
}