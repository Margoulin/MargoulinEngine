#ifndef __NODETRANSFORM_HPP__
#define __NODETRANSFORM_HPP__

#include "Maths/Transform.hpp"

class Node;

class Transform : public MTransform
{
public:
	Transform(Node* nd);
	Transform(const Transform&) = delete;
	Transform(Transform&&) = delete;
	~Transform() = default;

	auto	GetGlobalMatrix() -> Matrix4x4F;

	auto	operator = (const Transform&)->Transform& = delete;
	auto	operator = (Transform&&)->Transform& = delete;

//protected:

private:
#ifdef _DEBUG
	Vector3F	eulerAngles;
#endif
	Node*		attachedNode = nullptr;

#ifdef _DEBUG
public:
	auto	ImGuiUpdate() -> void;
#endif // _DEBUG

};


#endif /*__TRANSFORM_HPP__*/