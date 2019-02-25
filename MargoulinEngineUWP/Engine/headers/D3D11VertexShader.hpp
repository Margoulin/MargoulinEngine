#ifndef __D3D11_VERTEX_SHADER_HPP__
#define __D3D11_VERTEX_SHADER_HPP__

#include "VertexShader.hpp"

#include <d3d11.h>
#include <wrl/client.h>
#include "DirectXHelper.hpp"

class D3D11VertexShader : public VertexShader
{
public:
	D3D11VertexShader() {}
	D3D11VertexShader(ID3D11VertexShader* vtxShader, ID3D10Blob* shadBlob);
	D3D11VertexShader(ID3D11VertexShader* vtxShader, ID3D10Blob* shadBlob, ID3D11InputLayout* inputLayout);
	D3D11VertexShader(const D3D11VertexShader&) = delete;
	D3D11VertexShader(D3D11VertexShader&&) = delete;
	~D3D11VertexShader() = default;

	auto	InitializeDefaultVertexShader(ID3D11Device* d3d11Device) -> void;
	auto	InitializeSkinningVertexShader(ID3D11Device* d3d11Device) -> void {}
	auto	InitializeTextureVertexShader(ID3D11Device* d3d11Device) -> void;

	auto	Shutdown() -> void;

	auto	Bind(ID3D11DeviceContext* context) const -> void;

	auto	GetShaderBlob() const -> ID3D10Blob* { return shaderBlob.Get(); }
	auto	GetShader() const -> ID3D11VertexShader* const { return vtxShader.Get(); }
	auto	GetInputLayout() const -> ID3D11InputLayout* const { return inLayout.Get(); }

	auto	operator = (const D3D11VertexShader&)->D3D11VertexShader& = delete;
	auto	operator = (D3D11VertexShader&&)->D3D11VertexShader& = delete;

protected:

private:
	Microsoft::WRL::ComPtr<ID3D11VertexShader>	vtxShader = nullptr;
	Microsoft::WRL::ComPtr<ID3D10Blob>			shaderBlob = nullptr;
	Microsoft::WRL::ComPtr<ID3D11InputLayout>	inLayout = nullptr;
};


#endif /*__D3D11_VERTEX_SHADER_HPP__*/