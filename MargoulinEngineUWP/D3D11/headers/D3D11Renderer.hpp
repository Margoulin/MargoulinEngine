#ifndef __D3D11RENDERER_HPP__
#define __D3D11RENDERER_HPP__

#include "RendererPipeline.hpp"

#include <d3d11.h>
#include <wrl/client.h>
#include <MUtils/Maths/Matrix.hpp>
#include "TextureResource.hpp"

class D3D11Context;
class D3D11VertexShader;
class D3D11PixelShader;
class SubMeshData;

struct ViewProjectionConstantBuffer
{
	Matrix4x4F	View;
	Matrix4x4F	Projection;
};

struct SkeletonConstantBuffer
{
	unsigned int	boneCount = 0;
	float*			matricesData;
};

class D3D11Renderer : public RendererPipeline
{
public:
	D3D11Renderer() = default;
	D3D11Renderer(const D3D11Renderer&) = delete;
	D3D11Renderer(D3D11Renderer&&) = delete;
	~D3D11Renderer() = default;

	virtual auto	BeginRender() -> void;
	virtual auto	EndRender() -> void;
	virtual auto	Present() -> void;
	virtual auto	BindCamera(Matrix4x4F const& projectionMatrix, Matrix4x4F const& viewMatrix) -> void;
	virtual auto	RebindCamera() -> void;
	auto	InitializeTexture(TextureResource* tex) -> void;

	auto	SetContext(D3D11Context* value) -> void { context = value; }

	auto	Initialize() -> void;
	auto	Shutdown() -> void;

	auto	operator = (const D3D11Renderer&)->D3D11Renderer& = delete;
	auto	operator = (D3D11Renderer&&)->D3D11Renderer& = delete;

protected:

private:
	virtual auto	draw3DLine(Vector3F const& firstPoint, Vector3F const& secondPoint, Vector3F const& color) -> void;
	virtual auto	drawData(Mesh* mesh, Material* mat, Matrix4x4F const& modelMat) -> void;
	virtual auto	drawData(SkeletalMesh* mesh, Material* mat, Matrix4x4F const& modelMat) -> void;
	virtual auto	drawTexture(Vector4F const& screenRect, SubMeshData* texMesh, TextureRenderDataBase const& renderData) -> void;

	virtual auto	DrawRectangle(Vector2F const& screenPosition, Vector2F const& size, Vector4F const& color) -> void;
	virtual auto	DrawFilledRectangle(Vector2F const& screenPosition, Vector2F const& size, Vector4F const& color) -> void;
	virtual auto	DrawRoundedRectangle(Vector2F const& screenPosition, Vector2F const& size, Vector2F const& edgesRadius, Vector4F const& color) -> void;
	virtual auto	DrawFilledRoundedRectangle(Vector2F const& screenPosition, Vector2F const& size, Vector2F const& edgeseRadius, Vector4F const& color) -> void;
	virtual auto	DrawEllipse(Vector2F const& position, Vector2F const& radius, Vector4F const& colorValue) -> void;
	virtual auto	DrawFilledEllipse(Vector2F const& position, Vector2F const& radius, Vector4F const& colorValue) -> void;
	virtual auto	DrawLine(Vector2F const& point, Vector2F const& point2, Vector4F const& colorValue) -> void;
	virtual auto	DrawGeometry(PolygonRenderResource* polygon, Vector4F const& color) -> void;
	virtual auto	DrawFilledGeometry(PolygonRenderResource* polygon, Vector4F const& color) -> void;

	D3D11Context*							context = nullptr;
	ViewProjectionConstantBuffer			viewProjBufferData;
};


#endif /*__D3D11RENDERER_HPP__*/