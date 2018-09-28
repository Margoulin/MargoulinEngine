#ifndef __CAMERA_HPP__
#define __CAMERA_HPP__

#include "Maths/Matrix.hpp"
#include "Maths/Vector.hpp"

class Camera
{
public:
	Camera();
	Camera(const Camera&) = delete;
	Camera(Camera&&) = delete;
	~Camera() = default;

	auto	Translate(Vector3F const& offset) -> void;

	auto	GetPosition() const -> Vector3F const& { return position; }
	auto	GetViewMatrix() -> Matrix4x4F const&;

	auto	operator = (const Camera&)->Camera& = delete;
	auto	operator = (Camera&&)->Camera& = delete;

protected:

private:
	Matrix4x4F	viewMatrix;
	Vector3F	position;
	bool		viewMatrixDirty = true;

#ifdef _DEBUG
public:
	auto	ImGuiUpdate() -> void;
#endif // _DEBUG


};


#endif /*__CAMERA_HPP__*/