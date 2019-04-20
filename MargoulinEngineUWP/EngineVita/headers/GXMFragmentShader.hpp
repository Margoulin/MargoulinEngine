#ifndef __GXM_FRAGMENT_SHADER_HPP__
#define __GXM_FRAGMENT_SHADER_HPP__

#include "FragmentShader.hpp"

#include <psp2/gxm.h>

#include "GXMContext.hpp"

class GXMFragmentShader : public FragmentShader
{
public:
	GXMFragmentShader(SceGxmFragmentProgram* inProg, SceGxmShaderPatcherId inID)
	{
		program = inProg;
		patcherId = inID;
	}
	GXMFragmentShader(const GXMFragmentShader&) = delete;
	GXMFragmentShader(GXMFragmentShader&&) = delete;
	~GXMFragmentShader()
	{
		for (auto&& param : parameters)
			DEL(param);
	}

	void	BindShader(GXMContext* context) { sceGxmSetFragmentProgram(context->GetContext(), program); }

	void	AddParameter(GXMShaderParameter* param) { parameters.push_back(param); }
	GXMShaderParameter const*	GetParameter(unsigned int idx) const { return parameters[idx]; }

	SceGxmFragmentProgram*	GetProgram() const { return program; }
	SceGxmShaderPatcherId	GetProgramID() const { return patcherId; }

	auto	operator = (const GXMFragmentShader&)->GXMFragmentShader& = delete;
	auto	operator = (GXMFragmentShader&&)->GXMFragmentShader& = delete;

protected:

private:
	std::vector<GXMShaderParameter*>	parameters;
	SceGxmFragmentProgram*	program = nullptr;
	SceGxmShaderPatcherId	patcherId;
};


#endif /*__GXM_FRAGMENT_SHADER_HPP__*/