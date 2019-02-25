#ifndef __D3D11_SHADER_FACTORY_HPP__
#define __D3D11_SHADER_FACTORY_HPP__

#include "ShaderFactory.hpp"
#include <d3d11.h>

class D3D11Context;

class D3D11ShaderFactory : public ShaderFactory
{
public:
	D3D11ShaderFactory() = default;
	D3D11ShaderFactory(const D3D11ShaderFactory&) = delete;
	D3D11ShaderFactory(D3D11ShaderFactory&&) = delete;
	~D3D11ShaderFactory() = default;

	virtual auto	CreateVertexTextureShader() -> Shader* override;
	virtual auto	CreatePixelTextureShader() -> Shader* override;
	virtual auto	CreateBasicVertexShader() -> Shader* override;
	virtual auto	CreateUnlitColorShader() -> Shader* override;
	
	virtual auto	SetContext(Context* value) -> void { ShaderFactory::SetContext(value); d3d11Context = (D3D11Context*)value; }

	virtual auto	BindShader(Shader const* shader) -> void;
	virtual auto	DeleteShader(Shader* shader) -> void;

	auto	operator = (const D3D11ShaderFactory&)->D3D11ShaderFactory& = delete;
	auto	operator = (D3D11ShaderFactory&&)->D3D11ShaderFactory& = delete;

protected:
	virtual auto	createShader(Shader::ShaderType const& type, MString const& shaderText) -> Shader* override;

private:
	auto	createVertexShader(MString const& shaderText) -> Shader*;
	auto	createVertexShader(MString const& shaderText, D3D11_INPUT_ELEMENT_DESC layoutDesc[], unsigned int layoutCount) -> Shader*;
	auto	createPixelShader(MString const& shaderText) -> Shader*;

	D3D11Context* d3d11Context = nullptr;
};


#endif /*__D3D11_SHADER_FACTORY_HPP__*/