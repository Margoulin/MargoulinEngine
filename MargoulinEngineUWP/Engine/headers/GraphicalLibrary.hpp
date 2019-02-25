#ifndef __GRAPHICALLIBRARY_HPP__
#define __GRAPHICALLIBRARY_HPP__

#include "Service.hpp"
#include "Maths/Vector.hpp"
#include <vector>
#include "Maths/Matrix.hpp"
#include "TextureResource.hpp"

struct IUnknown;
class Context;
class Window;
class Shader;
class RendererPipeline;
class ShaderFactory;
class MeshResource;
class MaterialResource;

class GraphicalLibrary : Service
{
public:
	GraphicalLibrary() = default;
	GraphicalLibrary(const GraphicalLibrary&) = delete;
	GraphicalLibrary(GraphicalLibrary&&) = delete;
	~GraphicalLibrary() = default;

	virtual auto	Initialize() -> void { Service::Initialize(); }
	virtual auto	Initialize(Window* window) -> void;
	virtual auto	Shutdown() -> void;
	virtual auto	Update() -> void {}

	auto	DrawMesh(Matrix4x4F const& modelMat, MeshResource* meshRes, MaterialResource* matRes) -> void;
	auto	DrawTexture(Vector4F const& screenRect, TextureRenderData const& renderData) -> void;

#ifdef _DEBUG
	virtual auto	ImGuiUpdate() -> void;
#endif // _DEBUG

#ifdef UWP
	auto	UpdateWindowHandle(IUnknown* value, Vector2F const& size, float dpi) -> void;
#endif

	auto	GetWindow() const -> Window* { return window; }
	auto	GetContext() const -> Context* { return context; }
	auto	GetRenderPipeline() const -> RendererPipeline* { return pipeline; }
	auto	GetShaderFactory() const -> ShaderFactory* { return shaderFactory; }
	auto	GetShader(unsigned int const& value) const -> Shader const* { return shaders[value]; }

	auto	operator = (const GraphicalLibrary&)->GraphicalLibrary& = delete;
	auto	operator = (GraphicalLibrary&&)->GraphicalLibrary& = delete;

protected:
	std::vector<Shader*>	shaders;
	RendererPipeline*		pipeline = nullptr;
	ShaderFactory*			shaderFactory = nullptr;
	Context*				context = nullptr;
	Window*					window = nullptr;

private:

};


#endif /*__GRAPHICALLIBRARY_HPP__*/