#include "CameraComponent.hpp"

#include "Logger.hpp"
#include "Node.hpp"
#include "NodeTransform.hpp"

auto	CameraComponent::GetViewMatrix() const -> Matrix4x4F
{
	if (!attachedNode)
	{
		Logger::Log("Cannot create view matrix from camera component because attached node is nullptr");
		return Matrix4x4F::identity;
	}

	Vector3F up = attachedNode->GetTransform()->GetRotation() * Vector3F::up;
	Vector3F forward = attachedNode->GetTransform()->GetRotation() * Vector3F::back;
	Vector3F position = attachedNode->GetTransform()->GetPosition();

	return Matrix4x4F::LookAt(position, up, position + forward);
}

#include <imgui/imgui.h>

#ifdef _DEBUG

auto	CameraComponent::ImGuiUpdate() -> void
{
	ImGui::DragFloat("FOV", &parameters.FOV, 1.0f, 0.0f, 120.0f);
	ImGui::InputFloat("Near Clipping Plane", &parameters.ZNear);
	ImGui::InputFloat("Far Clipping Plane", &parameters.ZFar);
}

#endif // _DEBUG