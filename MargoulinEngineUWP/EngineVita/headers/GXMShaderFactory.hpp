#ifndef __GXM_SHADER_FACTORY_HPP__
#define __GXM_SHADER_FACTORY_HPP__

#include "ShaderFactory.hpp"

class GXMShaderFactory : public ShaderFactory
{
public:
	GXMShaderFactory() = default;
	GXMShaderFactory(const GXMShaderFactory&) = delete;
	GXMShaderFactory(GXMShaderFactory&&) = delete;
	~GXMShaderFactory() = default;

	virtual auto	CreateVertexTextureShader() -> Shader* override { return nullptr; }
	virtual auto	CreatePixelTextureShader() -> Shader* override { return nullptr; }
	virtual auto	CreateBasicVertexShader() -> Shader* override { return nullptr; }
	virtual auto	CreateUnlitColorShader() -> Shader* override { return nullptr; }
	virtual auto	CreateVertexSkinningShader() -> Shader* override { return nullptr; }
	virtual auto	CreateLineVertexShader() -> Shader* override { return nullptr; }

	virtual auto	SetContext(Context* value) -> void {}

	virtual auto	BindShader(Shader const* shader) -> void {}
	virtual auto	DeleteShader(Shader* shader) -> void {}

	auto	operator = (const GXMShaderFactory&)->GXMShaderFactory& = delete;
	auto	operator = (GXMShaderFactory&&)->GXMShaderFactory& = delete;

protected:
	virtual auto	createShader(Shader::ShaderType const& type, MString const& shaderText) -> Shader* override { return nullptr; }

private:
	auto	createVertexShader(MString const& shaderText) -> Shader* { return nullptr; }
	auto	createPixelShader(MString const& shaderText) -> Shader* { return nullptr; }

};


#endif /*__GXM_SHADER_FACTORY_HPP__*/