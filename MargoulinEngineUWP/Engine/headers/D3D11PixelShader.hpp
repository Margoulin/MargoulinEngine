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
	D3D11PixelShader() {}
	D3D11PixelShader(ID3D11PixelShader* shader) : pxShader(shader) {}
	D3D11PixelShader(const D3D11PixelShader&) = delete;
	D3D11PixelShader(D3D11PixelShader&&) = delete;
	~D3D11PixelShader() = default;

	auto	GetShader() const -> ID3D11PixelShader* const { return pxShader.Get(); }

	auto	Shutdown() -> void { *pxShader.ReleaseAndGetAddressOf() = nullptr; }

	auto	Bind(ID3D11DeviceContext* context) const -> void { context->PSSetShader(pxShader.Get(), nullptr, 0); }

	auto	operator = (const D3D11PixelShader&)->D3D11PixelShader& = delete;
	auto	operator = (D3D11PixelShader&&)->D3D11PixelShader& = delete;

protected:

private:
	Microsoft::WRL::ComPtr<ID3D11PixelShader>	pxShader = nullptr;
};


#endif /*__D3D11_PIXEL_SHADER_HPP__*/