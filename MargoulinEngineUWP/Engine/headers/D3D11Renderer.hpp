#ifndef __D3D11RENDERER_HPP__
#define __D3D11RENDERER_HPP__

#include "RendererPipeline.hpp"

#include <d3d11.h>
#include <wrl/client.h>
#include "Maths/Matrix.hpp"

class D3D11Context;

struct ViewProjectionConstantBuffer
{
	Matrix4x4F	View;
	Matrix4x4F	Projection;
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

	auto	SetContext(D3D11Context* value) -> void { context = value; }

	auto	Initialize() -> void;
	auto	Shutdown() -> void;

	auto	operator = (const D3D11Renderer&)->D3D11Renderer& = delete;
	auto	operator = (D3D11Renderer&&)->D3D11Renderer& = delete;

protected:

private:
	virtual auto	drawData(Mesh* mesh, Material* mat, Matrix4x4F const& modelMat) -> void;

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
	Microsoft::WRL::ComPtr<ID3D11Buffer>	viewProjConstantBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer>	modelConstantBuffer;
	ViewProjectionConstantBuffer			viewProjBufferData;
};


#endif /*__D3D11RENDERER_HPP__*/