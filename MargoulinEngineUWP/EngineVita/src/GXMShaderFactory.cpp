#include "GXMShaderFactory.hpp"

#include "GXMVertexShader.hpp"
#include "GXMFragmentShader.hpp"

extern SceGxmProgram _binary_shaders_clear_v_gxp_start;
extern SceGxmProgram _binary_shaders_clear_f_gxp_start;

static const SceGxmProgram *const 	clearVertexProgramGxp = &_binary_shaders_clear_v_gxp_start;
static const SceGxmProgram *const	clearFragmentProgramGxp = &_binary_shaders_clear_f_gxp_start;

static void* patcherHostAlloc(void* userData, unsigned int size)
{
	return malloc(size);
}

static void patcherHostFree(void* userData, void* mem)
{
	free(mem);
}

GXMShaderFactory::GXMShaderFactory()
{
	const unsigned int patcherBufferSize = 64 * 1024;
	const unsigned int patcherVertexUsseSize = 64 * 1024;
	const unsigned int patcherFragmentUsseSize = 64 * 1024;
	patcherBuffer = GPUMemoryManager::AllocateCustom(SCE_KERNEL_MEMBLOCK_TYPE_USER_RW_UNCACHE, patcherBufferSize, SCE_GXM_MEMORY_ATTRIB_RW);
	unsigned int patcherVertexUsseOffset;
	patcherVertexUsse = GPUMemoryManager::AllocateUsse(patcherVertexUsseSize, true, &patcherVertexUsseOffset);
	unsigned int patcherFragmentUsseOffset;
	patcherFragmentUsse = GPUMemoryManager::AllocateUsse(patcherFragmentUsseSize, false, &patcherFragmentUsseOffset);

	SceGxmShaderPatcherParams patcherParams;
	memset(&patcherParams, 0, sizeof(SceGxmShaderPatcherParams));
	patcherParams.userData = nullptr;
	patcherParams.hostAllocCallback = &patcherHostAlloc;
	patcherParams.hostFreeCallback = &patcherHostFree;
	patcherParams.bufferAllocCallback = nullptr;
	patcherParams.bufferFreeCallback = nullptr;
	patcherParams.bufferMem = patcherBuffer->GetVoidAdress();
	patcherParams.bufferMemSize = patcherBufferSize;
	patcherParams.vertexUsseAllocCallback = nullptr;
	patcherParams.vertexUsseFreeCallback = nullptr;
	patcherParams.vertexUsseMem = patcherVertexUsse->GetVoidAdress();
	patcherParams.vertexUsseMemSize = patcherVertexUsseSize;
	patcherParams.vertexUsseOffset = patcherVertexUsseOffset;
	patcherParams.fragmentUsseAllocCallback = nullptr;
	patcherParams.fragmentUsseFreeCallback = nullptr;
	patcherParams.fragmentUsseMem = patcherFragmentUsse->GetVoidAdress();
	patcherParams.fragmentUsseMemSize = patcherFragmentUsseSize;
	patcherParams.fragmentUsseOffset = patcherFragmentUsseOffset;

	sceGxmShaderPatcherCreate(&patcherParams, &shaderPatcher);

	blendInfos.colorFunc = SCE_GXM_BLEND_FUNC_ADD;
	blendInfos.alphaFunc = SCE_GXM_BLEND_FUNC_ADD;
	blendInfos.colorSrc = SCE_GXM_BLEND_FACTOR_SRC_ALPHA;
	blendInfos.colorDst = SCE_GXM_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
	blendInfos.alphaSrc = SCE_GXM_BLEND_FACTOR_ONE;
	blendInfos.alphaDst = SCE_GXM_BLEND_FACTOR_ZERO;
	blendInfos.colorMask = SCE_GXM_COLOR_MASK_ALL;

	sceGxmProgramCheck(clearVertexProgramGxp);
	sceGxmProgramCheck(clearFragmentProgramGxp);
}

GXMShaderFactory::~GXMShaderFactory()
{
	sceGxmShaderPatcherDestroy(shaderPatcher);
	DEL(patcherFragmentUsse);
	DEL(patcherVertexUsse);
	DEL(patcherBuffer);
}

auto	GXMShaderFactory::DeleteShader(Shader* shader) -> void
{
	if (shader->GetShaderType() == Shader::ShaderType::VERTEX)
	{
		GXMVertexShader* vtxShad = (GXMVertexShader*)shader;
		sceGxmShaderPatcherReleaseVertexProgram(shaderPatcher, vtxShad->GetProgram());
		sceGxmShaderPatcherUnregisterProgram(shaderPatcher, vtxShad->GetProgramID());
	}
	else if (shader->GetShaderType() == Shader::ShaderType::FRAGMENT)
	{
		GXMFragmentShader* fragShad = (GXMFragmentShader*)shader;
		sceGxmShaderPatcherReleaseFragmentProgram(shaderPatcher, fragShad->GetProgram());
		sceGxmShaderPatcherUnregisterProgram(shaderPatcher, fragShad->GetProgramID());
	}
	DEL(shader);
}

auto	GXMShaderFactory::CreateVertexTextureShader() -> Shader*
{
	return nullptr; 
}

auto	GXMShaderFactory::CreatePixelTextureShader() -> Shader*
{ 
	return nullptr; 
}

auto	GXMShaderFactory::CreateBasicVertexShader() -> Shader*
{
	return nullptr;
}

auto	GXMShaderFactory::CreateUnlitColorShader() -> Shader*
{
	return nullptr;
}

auto	GXMShaderFactory::CreateVertexSkinningShader() -> Shader*
{
	return nullptr;
}

auto	GXMShaderFactory::CreateLineVertexShader() -> Shader* 
{
	return nullptr;
}

auto	GXMShaderFactory::CreateClearVertexShader() -> Shader*
{
	const SceGxmProgramParameter* paramClearPositionAttribute = sceGxmProgramFindParameterByName(clearVertexProgramGxp, "aPosition");

	GXMVertexInputLayout	iL(1, 1);
	iL.attributes[0].streamIndex = 0;
	iL.attributes[0].offset = 0;
	iL.attributes[0].format = SCE_GXM_ATTRIBUTE_FORMAT_F32;
	iL.attributes[0].componentCount = 2;
	iL.attributes[0].regIndex = sceGxmProgramParameterGetResourceIndex(paramClearPositionAttribute);
	iL.streams[0].stride = sizeof(Vector2F);
	iL.streams[0].indexSource = SCE_GXM_INDEX_SOURCE_INDEX_32BIT;

	return createVertexShader(clearVertexProgramGxp, &iL);
}

auto	GXMShaderFactory::CreateClearFragmentShader() -> Shader*
{
	GXMFragmentShader* outShad = (GXMFragmentShader*)createFragmentShader(clearFragmentProgramGxp);
	GXMShaderParameter* unlitColor = NEW GXMShaderParameter(clearFragmentProgramGxp, "uClearColor");
	outShad->AddParameter(unlitColor);
	return outShad;
}

auto	GXMShaderFactory::createVertexShader(SceGxmProgram const* program, GXMVertexInputLayout* iL) -> Shader*
{
	SceGxmShaderPatcherId tempID;
	sceGxmShaderPatcherRegisterProgram(shaderPatcher, program, &tempID);

	SceGxmVertexProgram* outProg = nullptr;
	sceGxmShaderPatcherCreateVertexProgram(shaderPatcher, tempID, iL->attributes, iL->attributesCount, iL->streams, iL->streamsCount, &outProg);
	return NEW GXMVertexShader(outProg, tempID);
}

auto	GXMShaderFactory::createFragmentShader(SceGxmProgram const* program) -> Shader*
{
	SceGxmShaderPatcherId tempID;
	sceGxmShaderPatcherRegisterProgram(shaderPatcher, program, &tempID);

	SceGxmFragmentProgram* outProg = nullptr;
	sceGxmShaderPatcherCreateFragmentProgram(shaderPatcher, tempID, SCE_GXM_OUTPUT_REGISTER_FORMAT_UCHAR4, SCE_GXM_MULTISAMPLE_NONE, &blendInfos, program, &outProg);
	return NEW GXMFragmentShader(outProg, tempID);
}