#ifndef __GXM_VERTEX_SHADER_HPP__
#define __GXM_VERTEX_SHADER_HPP__

#include "VertexShader.hpp"

#include "MemoryMacro.hpp"
#include <psp2/gxm.h>
#include <vector>
#include "GXMShaderParameter.hpp"

#include "GXMContext.hpp"

class GXMVertexShader : public VertexShader
{
public:
	GXMVertexShader(SceGxmVertexProgram* inProg, SceGxmShaderPatcherId inID)
	{
		program = inProg;
		patcherId = inID;
	}
	GXMVertexShader(const GXMVertexShader&) = delete;
	GXMVertexShader(GXMVertexShader&&) = delete;
	~GXMVertexShader()
	{
		for (auto&& param : parameters)
			DEL(param);
	}

	void	BindShader(GXMContext* context) { sceGxmSetVertexProgram(context->GetContext(), program); }

	void	AddParameter(GXMShaderParameter* param) { parameters.push_back(param); }
	GXMShaderParameter const*	GetParameter(unsigned int idx) const { return parameters[idx]; }

	SceGxmVertexProgram*	GetProgram() const { return program; }
	SceGxmShaderPatcherId	GetProgramID() const { return patcherId; }

	auto	operator = (const GXMVertexShader&)->GXMVertexShader& = delete;
	auto	operator = (GXMVertexShader&&)->GXMVertexShader& = delete;

protected:

private:
	std::vector<GXMShaderParameter*>	parameters;
	SceGxmVertexProgram*				program = nullptr;
	SceGxmShaderPatcherId				patcherId;
};


#endif /*__GXM_VERTEX_SHADER_HPP__*/