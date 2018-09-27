#ifndef __CAMERA_HPP__
#define __CAMERA_HPP__

#include <DirectXMath.h>
#include "../MUtils/MUtils/Maths/Vector.hpp"

class Camera
{
public:
	Camera();
	Camera(const Camera&) = delete;
	Camera(Camera&&) = delete;
	~Camera() = default;

	enum DIRECTION
	{
		FRONT, BACK, LEFT, RIGHT, UP, DOWN
	};

	auto	ProcessKeyboard(DIRECTION const dir) -> void;

	auto	GetPosition() const -> Vector3F const& { return position; }
	auto	GetLocalMatrix() const -> DirectX::XMMATRIX const;

	auto	operator = (const Camera&)->Camera& = delete;
	auto	operator = (Camera&&)->Camera& = delete;

protected:

private:
	Vector3F			position;

#ifdef _DEBUG
public:
	auto	ImGuiUpdate() -> void;
#endif // _DEBUG


};


#endif /*__CAMERA_HPP__*/