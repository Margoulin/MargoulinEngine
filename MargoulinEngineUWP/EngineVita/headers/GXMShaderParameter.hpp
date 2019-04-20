#ifndef __GXM_SHADER_PARAMETER_HPP__
#define __GXM_SHADER_PARAMETER_HPP__

#include <psp2/gxm.h>
#include <MUtils/String.hpp>

class GXMShaderParameter
{
public:
	GXMShaderParameter(SceGxmProgram const* prog, MString const& paramName) { parameter = sceGxmProgramFindParameterByName(prog, paramName.Str()); }
	GXMShaderParameter(SceGxmProgram const* prog, SceGxmParameterSemantic sem, unsigned int idx) { parameter = sceGxmProgramFindParameterBySemantic(prog, sem, idx); }
	GXMShaderParameter(const GXMShaderParameter&) = delete;
	GXMShaderParameter(GXMShaderParameter&&) = delete;
	~GXMShaderParameter() = default;

	void	UpdateParameterData(void* buffer, unsigned int compCount, float* data) const { sceGxmSetUniformDataF(buffer, parameter, 0, compCount, data); }

	auto	operator = (const GXMShaderParameter&)->GXMShaderParameter& = delete;
	auto	operator = (GXMShaderParameter&&)->GXMShaderParameter& = delete;

protected:

private:
	SceGxmProgramParameter const*	parameter = nullptr;

};


#endif /*__GXM_SHADER_PARAMETER_HPP__*/