#ifndef __GXM_RENDERER_HPP__
#define __GXM_RENDERER_HPP__

#include "RendererPipeline.hpp"

#include "GPUMemoryManager.hpp"
#include <psp2/gxm.h>

#include <MUtils/Maths/Matrix.hpp>

class GXMContext;
class GXMFragmentShader;
class GXMVertexShader;
class GPUBuffer;
class GXMBuffer;

class GXMRenderer : public RendererPipeline
{
public:
	GXMRenderer();
	GXMRenderer(const GXMRenderer&) = delete;
	GXMRenderer(GXMRenderer&&) = delete;
	~GXMRenderer();

	virtual auto	BeginRender() -> void;
	virtual auto	EndRender() -> void;
	virtual auto	Present() -> void;
	virtual auto	BindCamera(Matrix4x4F const& projectionMatrix, Matrix4x4F const& viewMatrix) -> void {}
	virtual auto	RebindCamera() -> void {}

	auto	SetContext(GXMContext* value) -> void { context = value; }
	auto	SetClearVertexShader(GXMVertexShader* shad) -> void { clearVtxShader = shad; }
	auto	SetClearFragmentShader(GXMFragmentShader* shad) -> void { clearFragShader = shad; }

	virtual auto	DrawRectangle(Vector2F const& screenPosition, Vector2F const& size, Vector4F const& color) -> void {}
	virtual auto	DrawFilledRectangle(Vector2F const& screenPosition, Vector2F const& size, Vector4F const& color) -> void {}
	virtual auto	DrawRoundedRectangle(Vector2F const& screenPosition, Vector2F const& size, Vector2F const& edgesRadius, Vector4F const& color) -> void {}
	virtual auto	DrawFilledRoundedRectangle(Vector2F const& screenPosition, Vector2F const& size, Vector2F const& edgeseRadius, Vector4F const& color) -> void {}
	virtual auto	DrawEllipse(Vector2F const& position, Vector2F const& radius, Vector4F const& colorValue) -> void {}
	virtual auto	DrawFilledEllipse(Vector2F const& position, Vector2F const& radius, Vector4F const& colorValue) -> void {}
	virtual auto	DrawLine(Vector2F const& point, Vector2F const& point2, Vector4F const& colorValue) -> void {}
	virtual auto	DrawGeometry(PolygonRenderResource* polygon, Vector4F const& color) -> void {}
	virtual auto	DrawFilledGeometry(PolygonRenderResource* polygon, Vector4F const& color) -> void {}

	auto	operator = (const GXMRenderer&)->GXMRenderer& = delete;
	auto	operator = (GXMRenderer&&)->GXMRenderer& = delete;

protected:

private:
	virtual auto	draw3DLine(Vector3F const& firstPoint, Vector3F const& secondPoint, Vector3F const& color) -> void {}
	virtual auto	drawData(Mesh* mesh, Material* mat, Matrix4x4F const& modelMat) -> void {}
	virtual auto	drawData(SkeletalMesh* mesh, Material* mat, Matrix4x4F const& modelMat) -> void {}
	virtual auto	drawTexture(Vector4F const& screenRect, SubMeshData* texMesh, TextureRenderDataBase const& renderData) -> void {}

private:
	GXMContext*					context = nullptr;
	GXMVertexShader*			clearVtxShader = nullptr;
	GXMFragmentShader*			clearFragShader = nullptr;
	GPUAdress<Vector2F>*		clearVertices = nullptr;
	GPUAdress<unsigned int>*	clearIndices = nullptr;
	float						clearColor[4]{ 1.0f, 0.0f, 1.0f, 1.0f };
};

#endif /*__GXM_RENDERER_HPP__*/