#ifndef __CAMERACOMPONENT_HPP__
#define __CAMERACOMPONENT_HPP__

#include "Component.hpp"
#include <MUtils/Maths/Matrix.hpp>

struct CameraParameters
{
	float	FOV = 90.0f;
	float	ZNear = 0.01f;
	float	ZFar = 100.0f;
};

class CameraComponent : public Component
{
public:
	CameraComponent() = default;
	CameraComponent(const CameraComponent&) = delete;
	CameraComponent(CameraComponent&&) = delete;
	~CameraComponent() = default;

	virtual auto	GetObjectType() const -> ObjectType const { return ObjectType::CAMERA_COMPONENT; }
	virtual auto	GetComponentType() const -> COMPONENT_TYPE const { return COMPONENT_TYPE::CAMERA; }
	virtual auto	GetObjectTypeName() const -> MString const override { return "Camera Component"; }

	auto	GetProjectionMatrix(Vector2F const& windowSize) const -> Matrix4x4F { return Matrix4x4F::Perspective(parameters.FOV, windowSize.x / windowSize.y, parameters.ZNear, parameters.ZFar); }
	auto	GetViewMatrix() const -> Matrix4x4F;

	auto	operator = (const CameraComponent&)->CameraComponent& = delete;
	auto	operator = (CameraComponent&&)->CameraComponent& = delete;

protected:

private:
	CameraParameters	parameters;

#ifdef _DEBUG
	virtual auto	GetComponentTypeName() -> char* { return "Camera Component"; }
	virtual auto	ImGuiUpdate() -> void;
#endif
};


#endif /*__CAMERACOMPONENT_HPP__*/