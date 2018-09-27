#include "Camera.hpp"

#include "DirectXHelper.hpp"

using namespace DirectX;

Camera::Camera()
{
	position = {0.0f, 0.0f, 1.5f};
}

auto	Camera::ProcessKeyboard(DIRECTION const dir) -> void
{
	switch (dir)
	{
	case UP:
	{
		position += Vector3F(0.0f, 0.2f, 0.0f);
		break;
	}
	case DOWN:
	{
		position -= Vector3F(0.0f, 0.2f, 0.0f);
		break;
	}
	case LEFT:
	{
		position -= Vector3F(0.2f, 0.0f, 0.0f);
		break;
	}
	case RIGHT:
	{
		position += Vector3F(0.2f, 0.0f, 0.0f);
		break;
	}
	case BACK:
	{
		position += Vector3F(0.0f, 0.0f, 0.2f);
		break;
	}
	default:
	{
		position -= Vector3F(0.0f, 0.0f, 0.2f);
		break;
	}
	}
}

auto	Camera::GetLocalMatrix() const -> DirectX::XMMATRIX const
{
	XMFLOAT3 tempPos(position.x, position.y, position.z);
	XMVECTORF32	at = {position.x, position.y, position.z - 1.0f, 0.0f};
	XMVECTORF32	up = { 0.0f, 1.0f, 0.0f, 0.0f };
	return DirectX::XMMatrixTranspose(DirectX::XMMatrixLookAtRH(XMLoadFloat3(&tempPos), at, up));
}

#include <imgui.h>

#ifdef _DEBUG

auto	Camera::ImGuiUpdate() -> void
{
	ImGui::DragFloat3("Camera Position", (float*)(&position.x), 0.1f);
}

#endif // _DEBUG
