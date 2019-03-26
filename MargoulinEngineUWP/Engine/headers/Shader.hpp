#ifndef __SHADER_HPP__
#define __SHADER_HPP__

#include "CoreMinimal.hpp"

class Shader
{
public:

	enum ShaderType
	{
		NONE,
		FRAGMENT,
		VERTEX,
		GEOMETRY,
		COMPUTE
	};

	Shader() = default;
	Shader(const Shader&) = delete;
	Shader(Shader&&) = delete;
	~Shader() = default;

	auto	SetShaderName(MString const& value) -> void { name = value; }

	auto	GetShaderType() const -> ShaderType const& { return type; }

	auto	operator = (const Shader&)->Shader& = delete;
	auto	operator = (Shader&&)->Shader& = delete;

protected:
	ShaderType	type;
	MString		name;

#ifdef _DEBUG
public:
	auto	ImGuiUpdate() -> void {}
#endif // _DEBUG

};

static int			FeatureLevelIndex = 2;
static char const* VsTargets[] = { "vs_5_0", "vs_4_1", "vs_4_0", "vs_3_0" };
static char const* PsTargets[] = { "ps_5_0", "ps_4_1", "ps_4_0", "ps_3_0" };

#endif /*__SHADER_HPP__*/