#ifndef __TRANSFORM_HPP__
#define __TRANSFORM_HPP__

#include <DirectXMath.h>

class Node;

class Transform
{
public:
	Transform(Node* nd);
	Transform(const Transform&) = delete;
	Transform(Transform&&) = delete;
	~Transform() = default;

	auto	GetLocalMatrix() -> DirectX::XMMATRIX;
	auto	GetGlobalMatrix() -> DirectX::XMMATRIX;

	auto	operator = (const Transform&)->Transform& = delete;
	auto	operator = (Transform&&)->Transform& = delete;

//protected:

private:
	DirectX::XMFLOAT3	position;
	DirectX::XMFLOAT3	scale;
	DirectX::XMFLOAT3	eulerAngles;
	DirectX::XMFLOAT4	rotation;
	DirectX::XMMATRIX	localMatrix;
	Node*				attachedNode = nullptr;

#ifdef _DEBUG
public:
	auto	ImGuiUpdate() -> void;
#endif // _DEBUG

};


#endif /*__TRANSFORM_HPP__*/