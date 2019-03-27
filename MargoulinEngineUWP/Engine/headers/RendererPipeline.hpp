#ifndef __RENDERERPIPELINE_HPP__
#define __RENDERERPIPELINE_HPP__

#include <MUtils/Maths/Vector.hpp>
#include <MUtils/Maths/Matrix.hpp>

#include "TextureResource.hpp"

class Material;
class Mesh;
class SkeletalMesh;
class PolygonRenderResource;
class GPUBuffer;
class SubMeshData;

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
	virtual auto	RebindCamera() -> void = 0;

	auto	SetModelBuffer(GPUBuffer* buffer) -> void { modelBuffer = buffer; }
	auto	SetViewProjBuffer(GPUBuffer* buffer) -> void { viewProjBuffer = buffer; }
	auto	SetSkeletalMeshBuffer(GPUBuffer* buffer) -> void { skelMeshBuffer = buffer; }
	auto	SetLineBuffer(GPUBuffer* buffer) -> void { lineBuffer = buffer; }

	auto	GetLineVerticesBuffer() -> float* { return lineVerticesBuffer; }

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

	virtual auto	draw3DLine(Vector3F const& firstPoint, Vector3F const& secondPoint, Vector3F const& color) -> void = 0;
	virtual auto	drawData(Mesh* mesh, Material* mat, Matrix4x4F const& modelMat) -> void = 0;
	virtual auto	drawData(SkeletalMesh* mesh, Material* mat, Matrix4x4F const& modelMat) -> void = 0;
	virtual auto	drawTexture(Vector4F const& screenRect, SubMeshData* texMesh, TextureRenderData const& renderData) -> void = 0;
protected:

	GPUBuffer*	modelBuffer = nullptr;
	GPUBuffer*	viewProjBuffer = nullptr;
	GPUBuffer*	skelMeshBuffer = nullptr;
	GPUBuffer*	lineBuffer = nullptr;
	float		clearColor[4] = {0.5f, 0.5f, 0.5f, 1.0f};
	float		lineVerticesBuffer[14];
	//float		lineVerticesBuffer[693];

	Matrix4x4F	projectionMatrix;
	Matrix4x4F	viewMatrix;

private:

#ifdef _DEBUG
public:
	virtual auto	ImGuiUpdate() -> void;
#endif

};


#endif /*__RENDERERPIPELINE_HPP__*/