#ifndef __GXM_SHADER_FACTORY_HPP__
#define __GXM_SHADER_FACTORY_HPP__

#include "ShaderFactory.hpp"
#include "GPUMemoryManager.hpp"
#include <psp2/gxm.h>

struct GXMVertexInputLayout
{
	GXMVertexInputLayout(unsigned int attribute, unsigned int stream)
	{
		attributes = NEW SceGxmVertexAttribute[attribute];
		streams = NEW SceGxmVertexStream[stream];
		attributesCount = attribute;
		streamsCount = stream;
	}

	~GXMVertexInputLayout()
	{
		DEL(attributes);
		DEL(streams);
		attributesCount = 0;
		streamsCount = 0;
	}

	SceGxmVertexAttribute*	attributes;
	SceGxmVertexStream*		streams;
	unsigned int			attributesCount = 0;
	unsigned int			streamsCount = 0;
};

class GXMShaderFactory : public ShaderFactory
{
public:
	GXMShaderFactory();
	GXMShaderFactory(const GXMShaderFactory&) = delete;
	GXMShaderFactory(GXMShaderFactory&&) = delete;
	~GXMShaderFactory();

	virtual auto	CreateVertexTextureShader() -> Shader* override;
	virtual auto	CreatePixelTextureShader() -> Shader* override;
	virtual auto	CreateBasicVertexShader() -> Shader* override;
	virtual auto	CreateUnlitColorShader() -> Shader* override;
	virtual auto	CreateVertexSkinningShader() -> Shader* override;
	virtual auto	CreateLineVertexShader() -> Shader* override;
	auto			CreateClearVertexShader() -> Shader*;
	auto			CreateClearFragmentShader() -> Shader*;

	SceGxmShaderPatcher* GetShaderPatcher() { return shaderPatcher; }

	virtual auto	SetContext(Context* value) -> void {}

	virtual auto	BindShader(Shader const* shader) -> void {}
	virtual auto	DeleteShader(Shader* shader) -> void;

	auto	operator = (const GXMShaderFactory&)->GXMShaderFactory& = delete;
	auto	operator = (GXMShaderFactory&&)->GXMShaderFactory& = delete;

protected:
	virtual auto	createShader(Shader::ShaderType const& type, MString const& shaderText) -> Shader* override { return nullptr; }

private:
	auto	createVertexShader(SceGxmProgram const* program, GXMVertexInputLayout* inputLayout) -> Shader*;
	auto	createFragmentShader(SceGxmProgram const* program) -> Shader*;

private:
	GPUAdressBase*			patcherBuffer = nullptr;
	GPUUsseAdress*			patcherVertexUsse = nullptr;
	GPUUsseAdress*			patcherFragmentUsse = nullptr;
	SceGxmShaderPatcher*	shaderPatcher = nullptr;

	SceGxmBlendInfo			blendInfos;
	SceGxmShaderPatcherId	clearFragId;
	SceGxmShaderPatcherId	clearVertexId;
};


#endif /*__GXM_SHADER_FACTORY_HPP__*/