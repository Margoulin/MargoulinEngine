#ifndef __MATERIAL_HPP__
#define __MATERIAL_HPP__

#include <DirectXMath.h>
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

	Shader*				attachedShader = nullptr;
	std::string			name = "";
	DirectX::XMFLOAT4	unlitColor = {1.0f, 1.0f, 1.0f, 1.0f};
	DirectX::XMFLOAT3	ka = {1.0f, 1.0f, 1.0f};
	DirectX::XMFLOAT3	kd = {1.0f, 1.0f, 1.0f};
	DirectX::XMFLOAT3	ks = {1.0f, 1.0f, 1.0f};
	float				shininess = 100.0f;

#ifdef _DEBUG
public:
	auto	ImGuiUpdate() -> void;
#endif // _DEBUG

};


#endif /*__MATERIAL_HPP__*/