#ifndef __D3D11_PIXEL_SHADER_HPP__
#define __D3D11_PIXEL_SHADER_HPP__

#include "FragmentShader.hpp"

#include <d3d11.h>
#include <wrl/client.h>
#include "DirectXHelper.hpp"

class D3D11VertexShader;

class D3D11PixelShader : public FragmentShader
{
public:
	D3D11PixelShader() = default;
	D3D11PixelShader(const D3D11PixelShader&) = delete;
	D3D11PixelShader(D3D11PixelShader&&) = delete;
	~D3D11PixelShader() = default;

	auto	InitializeUnlitColorPixelShader(ID3D11Device* device, D3D11VertexShader* vtxShader) -> void;
	auto	InitializeTexturePixelShader(ID3D11Device* device, D3D11VertexShader* vtxShader) -> void;

	auto	GetShader() const -> ID3D11PixelShader* const { return pxShader.Get(); }
	auto	GetInputLayout() const -> ID3D11InputLayout* const { return inLayout.Get(); }

	auto	Shutdown() -> void;

	auto	Bind(ID3D11DeviceContext* context) const -> void;

	auto	operator = (const D3D11PixelShader&)->D3D11PixelShader& = delete;
	auto	operator = (D3D11PixelShader&&)->D3D11PixelShader& = delete;

protected:

private:
	Microsoft::WRL::ComPtr<ID3D11InputLayout>	inLayout = nullptr;
	Microsoft::WRL::ComPtr<ID3D11PixelShader>	pxShader = nullptr;
};


#endif /*__D3D11_PIXEL_SHADER_HPP__*/