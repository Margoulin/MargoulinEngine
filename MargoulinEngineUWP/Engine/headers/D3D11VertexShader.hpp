#ifndef __D3D11_VERTEX_SHADER_HPP__
#define __D3D11_VERTEX_SHADER_HPP__

#include "VertexShader.hpp"

#include <d3d11.h>
#include <wrl/client.h>
#include "DirectXHelper.hpp"

class D3D11VertexShader : public VertexShader
{
public:
	D3D11VertexShader() = default;
	D3D11VertexShader(const D3D11VertexShader&) = delete;
	D3D11VertexShader(D3D11VertexShader&&) = delete;
	~D3D11VertexShader() = default;

	auto	InitializeDefaultVertexShader(ID3D11Device* d3d11Device) -> void;
	auto	InitializeSkinningVertexShader(ID3D11Device* d3d11Device) -> void {}
	auto	InitializeTextureVertexShader(ID3D11Device* d3d11Device) -> void;

	auto	Shutdown() -> void { *vtxShader.ReleaseAndGetAddressOf() = nullptr; }

	auto	Bind(ID3D11DeviceContext* context) const -> void { context->VSSetShader(vtxShader.Get(), nullptr, 0); }

	auto	GetShaderBlob() const -> ID3D10Blob* { return shaderBlob.Get(); }
	auto	GetShader() const -> ID3D11VertexShader* const { return vtxShader.Get(); }

	auto	operator = (const D3D11VertexShader&)->D3D11VertexShader& = delete;
	auto	operator = (D3D11VertexShader&&)->D3D11VertexShader& = delete;

protected:

private:
	Microsoft::WRL::ComPtr<ID3D11VertexShader>	vtxShader = nullptr;
	Microsoft::WRL::ComPtr<ID3D10Blob>			shaderBlob = nullptr;
};


#endif /*__D3D11_VERTEX_SHADER_HPP__*/