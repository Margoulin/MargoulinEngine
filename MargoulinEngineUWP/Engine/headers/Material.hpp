#ifndef __MATERIAL_HPP__
#define __MATERIAL_HPP__

#include "Maths/Vector.hpp"
#include <d3d11.h>
#include <string>

class Shader;

class Material
{
public:
	Material() = default;
	Material(const Material&) = delete;
	Material(Material&&) = delete;
	~Material() = default;

	auto	Bind(ID3D11DeviceContext* context) -> void;

	auto	operator = (const Material&)->Material& = delete;
	auto	operator = (Material&&)->Material& = delete;

	Shader*		attachedShader = nullptr;
	std::string	name = "";
	Vector4F	unlitColor = Vector4F::one;
	Vector3F	ka = Vector3F::one;
	Vector3F	kd = Vector3F::one;
	Vector3F	ks = Vector3F::one;
	float		shininess = 100.0f;

#ifdef _DEBUG
public:
	auto	ImGuiUpdate() -> void;
#endif // _DEBUG

};


#endif /*__MATERIAL_HPP__*/