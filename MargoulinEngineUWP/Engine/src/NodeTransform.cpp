#include "NodeTransform.hpp"

#include "Node.hpp"
#include <MUtils/Maths/Math.hpp>

Transform::Transform(Node* nd)
{
	attachedNode = nd;
}

auto	Transform::GetGlobalMatrix() -> Matrix4x4F
{
	std::vector<Transform*>	parents;

	auto* parent = attachedNode->GetParent();

	while (parent != nullptr)
	{
		parents.push_back(parent->GetTransform());
		parent = parent->GetParent();
	}

	if (parents.empty())	return GetLocalMatrix();

	Matrix4x4F	ret = Matrix4x4F::identity;
	for (size_t pos = 0u; pos < parents.size(); pos++)
		ret = parents[pos]->GetLocalMatrix() * ret;

	return ret * GetLocalMatrix();
}

#include <imgui/imgui.h>
#ifdef _DEBUG

auto	Transform::ImGuiUpdate() -> void
{
	ImGui::DragFloat3("Position", (float*)(&position.x), 0.1f);
	ImGui::DragFloat3("Scale", (float*)(&scale.x), 0.1f);

	ImGui::DragFloat3("Euler Angles", (float*)(&eulerAngles.x), 0.5f, -360.0f, 360.0f);
	Vector3F tempEuler = eulerAngles * degToRad;
	rotation = Quaternion::Euler(tempEuler);
}

#endif // _DEBUG
