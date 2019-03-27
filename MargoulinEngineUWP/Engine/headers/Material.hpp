#ifndef __MATERIAL_HPP__
#define __MATERIAL_HPP__

#include <MUtils/Maths/Vector.hpp>
#include <d3d11.h>
#include <MUtils/String.hpp>
#include <wrl/client.h>

class FragmentShader;

class Material
{
public:
	Material() = default;
	Material(const Material&) = delete;
	Material(Material&&) = delete;
	~Material() = default;

	auto	Initialize(ID3D11Device* device) -> void;
	auto	Shutdown() -> void;

	auto	Bind(ID3D11DeviceContext* context) -> void;

	auto	GetConstantBuffer() const -> ID3D11Buffer* const { return unlitColorConstantBuffer.Get(); }

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
	Microsoft::WRL::ComPtr<ID3D11Buffer>	unlitColorConstantBuffer;

#ifdef _DEBUG
public:
	auto	ImGuiUpdate() -> void;
#endif // _DEBUG

};


#endif /*__MATERIAL_HPP__*/