#ifndef __FRAGMENT_SHADER_HPP__
#define __FRAGMENT_SHADER_HPP__

#include "Shader.hpp"

class FragmentShader : public Shader
{
public:
	FragmentShader() { type = ShaderType::FRAGMENT; }
	FragmentShader(const FragmentShader&) = delete;
	FragmentShader(FragmentShader&&) = delete;
	~FragmentShader() = default;

	auto	operator = (const FragmentShader&)->FragmentShader& = delete;
	auto	operator = (FragmentShader&&)->FragmentShader& = delete;

protected:

private:

};


#endif /*__FRAGMENT_SHADER_HPP__*/