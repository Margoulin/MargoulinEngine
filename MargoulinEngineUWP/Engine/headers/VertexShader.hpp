#ifndef __VERTEX_SHADER_HPP__
#define __VERTEX_SHADER_HPP__

#include "Shader.hpp"

class VertexShader : public Shader
{
public:
	VertexShader() { type = ShaderType::VERTEX; }
	VertexShader(const VertexShader&) = delete;
	VertexShader(VertexShader&&) = delete;
	~VertexShader() = default;

	auto	operator = (const VertexShader&)->VertexShader& = delete;
	auto	operator = (VertexShader&&)->VertexShader& = delete;

protected:

private:

};


#endif /*__VERTEX_SHADER_HPP__*/