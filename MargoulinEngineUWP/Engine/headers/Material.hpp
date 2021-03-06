#ifndef __MATERIAL_HPP__
#define __MATERIAL_HPP__

#include "CoreMinimal.hpp"

class FragmentShader;
class GPUBuffer;
class Context;

class Material
{
public:
	Material() = default;
	Material(const Material&) = delete;
	Material(Material&&) = delete;
	~Material() = default;

	auto	Bind(Context* context) -> void;
	auto	Shutdown() -> void;

	auto	SetConstantBuffer(GPUBuffer* value) -> void { unlitColorBuffer = value; }
	auto	GetConstantBuffer() const -> GPUBuffer* { return unlitColorBuffer; }

	auto	operator = (const Material&)->Material& = delete;
	auto	operator = (Material&&)->Material& = delete;

	FragmentShader*		attachedShader = nullptr;
	MString				name = "";
	Vector4F	unlitColor = Vector4F::one;
	Vector3F	ka = Vector3F::one;
	Vector3F	kd = Vector3F::one;
	Vector3F	ks = Vector3F::one;
	float		shininess = 100.0f;

private:

	GPUBuffer*	unlitColorBuffer;

#ifdef _DEBUG
public:
	auto	ImGuiUpdate() -> void;
#endif // _DEBUG

};


#endif /*__MATERIAL_HPP__*/