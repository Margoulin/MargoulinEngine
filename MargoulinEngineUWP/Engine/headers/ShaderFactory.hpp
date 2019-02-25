#ifndef __SHADER_FACTORY_HPP__
#define __SHADER_FACTORY_HPP__

#include "Shader.hpp"

#include "CoreMinimal.hpp"

class Context;

class ShaderFactory
{
public:
	ShaderFactory() = default;
	ShaderFactory(const ShaderFactory&) = delete;
	ShaderFactory(ShaderFactory&&) = delete;
	~ShaderFactory() = default;

	virtual auto	SetContext(Context* value) -> void { context = value; }

	//TO FIX WHEN SHADER PARSING DONE
	virtual auto	CreateVertexTextureShader() -> Shader* = 0;
	virtual auto	CreatePixelTextureShader() -> Shader* = 0;
	virtual auto	CreateBasicVertexShader() -> Shader* = 0;
	virtual auto	CreateUnlitColorShader() -> Shader* = 0;

	virtual auto	BindShader(Shader const* shader) -> void = 0;
	virtual auto	DeleteShader(Shader* shader) -> void = 0;

	auto	operator = (const ShaderFactory&)->ShaderFactory& = delete;
	auto	operator = (ShaderFactory&&)->ShaderFactory& = delete;

protected:
	virtual auto	createShader(Shader::ShaderType const& type, MString const& shaderText) -> Shader* = 0;
	
	Context*	context;

private:
};


#endif /*__SHADER_FACTORY_HPP__*/