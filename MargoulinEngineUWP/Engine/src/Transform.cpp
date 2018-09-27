#include "Transform.hpp"
#include "Node.hpp"
#include "DirectXHelper.hpp"

using namespace DirectX;

Transform::Transform(Node* nd)
{
	attachedNode = nd;
	position = { 0.0f, 0.0f, 0.0f };
	scale = { 1.0f, 1.0f, 1.0f };
	rotation = { 0.0f, 0.0f, 0.0f, 1.0f };
	localMatrix = XMMatrixIdentity();
}

auto	Transform::GetLocalMatrix() -> XMMATRIX
{
	XMMATRIX	translateMat = XMMatrixTranslation(position.x, position.y, position.z);
	XMVECTOR	tempRot = XMLoadFloat4(&rotation);
	XMMATRIX	rotateMat = XMMatrixRotationQuaternion(tempRot);
	rotateMat = XMMatrixTranspose(rotateMat);
	XMMATRIX	scaleMat = XMMatrixScaling(scale.x, scale.y, scale.z);

	XMMATRIX temp = XMMatrixIdentity() * scaleMat * rotateMat * translateMat;
	localMatrix = XMMatrixTranspose(temp);
	return localMatrix;
}

auto	Transform::GetGlobalMatrix() -> XMMATRIX
{
	std::vector<Transform*>	parents;

	auto* parent = attachedNode->GetParent();

	while (parent != nullptr)
	{
		parents.push_back(parent->GetTransform());
		parent = parent->GetParent();
	}

	if (parents.empty())	return GetLocalMatrix();

	XMMATRIX	ret = XMMatrixIdentity();
	for (int pos = 0u; pos < parents.size(); pos++)
	//for (int pos = parents.size() - 1; pos >=0; pos--)
		ret = parents[pos]->GetLocalMatrix() * ret;

	return ret * GetLocalMatrix();
}

#include <imgui.h>
#ifdef _DEBUG

auto	Transform::ImGuiUpdate() -> void
{
	ImGui::DragFloat3("Position", (float*)(&position.x), 0.1f);
	ImGui::DragFloat3("Scale", (float*)(&scale.x), 0.1f);
	ImGui::DragFloat4("Rotation", (float*)(&rotation.x), 0.1f, -1.0f, 1.0f);

	ImGui::DragFloat3("Euler Angles", (float*)(&eulerAngles.x), 0.5f, -360.0f, 360.0f);
	DirectX::XMFLOAT3 tempEuler = Vector3ToRad(eulerAngles);
	XMVECTOR tempRot = DirectX::XMQuaternionRotationRollPitchYaw(tempEuler.x, tempEuler.y, tempEuler.z);
	DirectX::XMStoreFloat4(&rotation, tempRot);
}

#endif // _DEBUG
