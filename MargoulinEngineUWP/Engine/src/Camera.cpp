#include "Camera.hpp"

Camera::Camera()
{
	position = {0.0f, 0.0f, 1.5f};
}

auto	Camera::Translate(Vector3F const& offset) -> void
{
	position += offset;
	viewMatrixDirty = true;
}

auto	Camera::GetViewMatrix() -> Matrix4x4F const&
{
	if (viewMatrixDirty)
	{
		viewMatrix = Matrix4x4F::Transpose(Matrix4x4F::LookAt(position, Vector3F::up, position + Vector3F::back));
		viewMatrixDirty = false;
	}
	return viewMatrix;
}

#include <imgui.h>

#ifdef _DEBUG

auto	Camera::ImGuiUpdate() -> void
{
	ImGui::DragFloat3("Camera Position", (float*)(&position.x), 0.1f);
}

#endif // _DEBUG
