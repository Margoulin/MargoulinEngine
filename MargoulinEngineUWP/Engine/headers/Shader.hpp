#ifndef __SHADER_HPP__
#define __SHADER_HPP__

#include <d3d11.h>
#include <wrl/client.h>
#include "Maths/Vector.hpp"

class Shader
{
public:
	enum ShaderType
	{
		UNLIT_COLOR,
		UNLIT_TEXTURE
	};

	Shader(ShaderType const typeValue = ShaderType::UNLIT_COLOR) { type = typeValue; }
	Shader(const Shader&) = delete;
	Shader(Shader&&) = delete;
	~Shader() = default;

	auto	Initialize(ID3D11Device* rend) -> void;
	auto	Shutdown() -> void;

	auto	SetColor(Vector4F const& value) -> void { unlitColorData = value; }

	auto	SetCurrent(ID3D11DeviceContext* context) -> void;

	auto	operator = (const Shader&)->Shader& = delete;
	auto	operator = (Shader&&)->Shader& = delete;

protected:

private:
	ID3D11InputLayout*						g_VAO = nullptr;
	ID3D10Blob*								g_ShaderBlob = nullptr;
	ID3D11VertexShader*						g_VertexShader = nullptr;
	ID3D11PixelShader*						g_PixelShader = nullptr;
	Microsoft::WRL::ComPtr<ID3D11Buffer>	unlitColorConstantBuffer;
	Vector4F								unlitColorData = Vector4F::one;
	ShaderType								type = ShaderType::UNLIT_COLOR;

#ifdef _DEBUG
public:
	auto	ImGuiUpdate() -> void;
#endif // _DEBUG

};


#endif /*__SHADER_HPP__*/