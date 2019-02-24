#ifndef __RENDERERPIPELINE_HPP__
#define __RENDERERPIPELINE_HPP__

#include "Maths/Vector.hpp"
#include "Maths/Matrix.hpp"

class Material;
class Mesh;
class PolygonRenderResource;

class RendererPipeline
{
public:
	RendererPipeline() = default;
	RendererPipeline(const RendererPipeline&) = delete;
	RendererPipeline(RendererPipeline&&) = delete;
	~RendererPipeline() = default;

	virtual auto	BeginRender() -> void = 0;
	virtual auto	EndRender() -> void = 0;
	virtual auto	Present() -> void = 0;
	virtual auto	BindCamera(Matrix4x4F const& projectionMatrix, Matrix4x4F const& viewMatrix) -> void = 0;

	auto	DrawCustomMesh(unsigned int const& meshID, unsigned int const& matID, Matrix4x4F const& modelMat) -> void;
	auto	DrawCube(unsigned int const& matID, Matrix4x4F const& modelMat) -> void;
	auto	DrawSkeletalMesh(unsigned int const& meshID) -> void;

	virtual auto	DrawRectangle(Vector2F const& screenPosition, Vector2F const& size, Vector4F const& color) -> void = 0;
	virtual auto	DrawFilledRectangle(Vector2F const& screenPosition, Vector2F const& size, Vector4F const& color) -> void = 0;
	virtual auto	DrawRoundedRectangle(Vector2F const& screenPosition, Vector2F const& size, Vector2F const& edgesRadius, Vector4F const& color) -> void = 0;
	virtual auto	DrawFilledRoundedRectangle(Vector2F const& screenPosition, Vector2F const& size, Vector2F const& edgesRadius, Vector4F const& color) -> void = 0;
	virtual auto	DrawEllipse(Vector2F const& position, Vector2F const& radius, Vector4F const& colorValue) -> void = 0;
	virtual auto	DrawFilledEllipse(Vector2F const& position, Vector2F const& radius, Vector4F const& colorValue) -> void= 0;
	virtual auto	DrawLine(Vector2F const& point, Vector2F const& point2, Vector4F const& colorValue) -> void = 0;
	virtual auto	DrawGeometry(PolygonRenderResource* polygon, Vector4F const& color) -> void = 0;
	virtual auto	DrawFilledGeometry(PolygonRenderResource* polygon, Vector4F const& color) -> void = 0;

	auto	operator = (const RendererPipeline&)->RendererPipeline& = delete;
	auto	operator = (RendererPipeline&&)->RendererPipeline& = delete;

protected:
	virtual auto	drawData(Mesh* mesh, Material* mat, Matrix4x4F const& modelMat) -> void = 0;

	float									clearColor[4] = {0.5f, 0.5f, 0.5f, 1.0f};

private:

#ifdef _DEBUG
public:
	virtual auto	ImGuiUpdate() -> void;
#endif

};


#endif /*__RENDERERPIPELINE_HPP__*/