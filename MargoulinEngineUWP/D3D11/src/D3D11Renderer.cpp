#include "D3D11Renderer.hpp"

#include "D3D11Context.hpp"
#include "Mesh.hpp"
#include "SkeletalMesh.hpp"
#include "Material.hpp"
#include "PolygonRenderResource.hpp"

#include "D3D11PixelShader.hpp"
#include "D3D11VertexShader.hpp"
#include "ShaderFactory.hpp"

#include "./Logger.hpp"

#include "Engine.hpp"
#include "GraphicalLibrary.hpp"
#include "D3D11VertexShader.hpp"
#include "D3D11Buffer.hpp"
#include "D3D11TextureData.hpp"

auto	D3D11Renderer::Initialize() -> void
{
}

auto	D3D11Renderer::Shutdown() -> void
{
	((D3D11Buffer*)modelBuffer)->Shutdown();
	DEL(modelBuffer);
	((D3D11Buffer*)viewProjBuffer)->Shutdown();
	DEL(viewProjBuffer);
}

auto	D3D11Renderer::BeginRender() -> void
{
#ifdef UWP
	CD3D11_VIEWPORT m_screenViewport = CD3D11_VIEWPORT(
		0.0f,
		0.0f,
		context->GetRenderTargetSize().x,
		context->GetRenderTargetSize().y);
	context->GetDeviceContext()->RSSetViewports(1, &m_screenViewport);

	ID3D11RenderTargetView *const targets[1] = { context->GetRenderTarget() };
	context->GetDeviceContext()->OMSetRenderTargets(1, targets, context->GetDepthStencilView());
#endif

	context->GetDeviceContext()->ClearRenderTargetView(context->GetRenderTarget(), clearColor);
	context->GetDeviceContext()->ClearDepthStencilView(context->GetDepthStencilView(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	modelBuffer->BindBuffer(context);
}

auto	D3D11Renderer::EndRender() -> void
{
	context->GetDeviceContext()->VSSetShader(nullptr, nullptr, 0);
	context->GetDeviceContext()->PSSetShader(nullptr, nullptr, 0);
}

auto	D3D11Renderer::Present() -> void
{
	context->Swap();
}
	
auto	D3D11Renderer::BindCamera(Matrix4x4F const& projectionMatrix, Matrix4x4F const& viewMatrix) -> void
{
	viewProjBufferData.View = Matrix4x4F::Transpose(viewMatrix);	
	viewProjBufferData.Projection = Matrix4x4F::Transpose(projectionMatrix);

	viewProjBuffer->UpdateBufferData(context, (void*)&viewProjBufferData);
	viewProjBuffer->BindBuffer(context);

	this->projectionMatrix = projectionMatrix;
	this->viewMatrix = viewMatrix;
}

auto	D3D11Renderer::RebindCamera() -> void
{
	viewProjBufferData.View = Matrix4x4F::Transpose(viewMatrix);
	viewProjBufferData.Projection = Matrix4x4F::Transpose(projectionMatrix);
	viewProjBuffer->UpdateBufferData(context, (void*)&viewProjBufferData);
	viewProjBuffer->BindBuffer(context);
}

auto	D3D11Renderer::draw3DLine(Vector3F const& firstPoint, Vector3F const& secondPoint, Vector3F const& color) -> void
{
	modelBuffer->UpdateBufferData(context, (void*)Matrix4x4F::identity.GetArray());

	memcpy(lineVerticesBuffer, &firstPoint.x, 3 * sizeof(float));
	memcpy(&lineVerticesBuffer[3], &color.x, 4 * sizeof(float));
	lineVerticesBuffer[6] = 1.0f;
	memcpy(&lineVerticesBuffer[7], &secondPoint.x, 3 * sizeof(float));
	memcpy(&lineVerticesBuffer[10], &color.x, 4 * sizeof(float));
	lineVerticesBuffer[13] = 1.0f;

	lineBuffer->UpdateBufferData(context, lineVerticesBuffer, (sizeof(Vector3F) + sizeof(Vector4F)) * 2);
	lineBuffer->BindBuffer(context);

	context->GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
	context->GetDeviceContext()->Draw(2, 0);
}

auto	D3D11Renderer::drawData(Mesh* mesh, Material* mat, Matrix4x4F const& modelMat) -> void
{
	mat->Bind(context);

	modelBuffer->UpdateBufferData(context, (void*)Matrix4x4F::Transpose(modelMat).GetArray());

	SubMeshData** subMeshes = mesh->GetSubMeshTab();
	for (unsigned int pos = 0; pos < mesh->GetSubMeshNbr(); pos++)
	{
		SubMeshData* subMesh = subMeshes[pos];
		subMesh->GetVertexBuffer()->BindBuffer(context);
		subMesh->GetIndexBuffer()->BindBuffer(context);

		context->GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		context->GetDeviceContext()->DrawIndexed(subMesh->GetIndicesCount(), 0, 0);
	}
}
	
auto	D3D11Renderer::drawData(SkeletalMesh* mesh, Material* mat, Matrix4x4F const& modelMat) -> void
{
	mat->Bind(context);

	SkeletonConstantBuffer	skeletalShaderData;
	skeletalShaderData.boneCount = mesh->GetRig()->GetBoneCount();
	skeletalShaderData.matricesData = mesh->GetBindPose()->bonesFloat.data();

	skelMeshBuffer->UpdateBufferData(context, &skeletalShaderData);

	modelBuffer->UpdateBufferData(context, (void*)Matrix4x4F::Transpose(modelMat).GetArray());
	
	mesh->GetVertexBufferArray()->BindBuffers(context);
	mesh->GetIndexBuffer()->BindBuffer(context);

	context->GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	context->GetDeviceContext()->DrawIndexed(mesh->GetIndicesCount(), 0, 0);
}
	
auto	D3D11Renderer::DrawRectangle(Vector2F const& screenPosition, Vector2F const& size, Vector4F const& colorValue) -> void
{
	D2D1::ColorF	color(colorValue.x, colorValue.y, colorValue.z, colorValue.w);

	ComPtr<ID2D1SolidColorBrush>	tempBrush = nullptr;
	context->d2dContext->CreateSolidColorBrush(color, &tempBrush);
	context->d2dContext->SaveDrawingState(context->stateBlock.Get());
	context->d2dContext->BeginDraw();

	D2D1_RECT_F	rect;
	rect.left = screenPosition.x;
	rect.right = screenPosition.x + size.x;
	rect.top = screenPosition.y;
	rect.bottom = screenPosition.y + size.y;

	context->d2dContext->DrawRectangle(&rect, tempBrush.Get());

	HRESULT hr = context->d2dContext->EndDraw();
	context->d2dContext->RestoreDrawingState(context->stateBlock.Get());
	tempBrush.Reset();
}

auto	D3D11Renderer::DrawFilledRectangle(Vector2F const& screenPosition, Vector2F const& size, Vector4F const& colorValue) -> void
{
	D2D1::ColorF	color(colorValue.x, colorValue.y, colorValue.z, colorValue.w);

	ComPtr<ID2D1SolidColorBrush>	tempBrush = nullptr;
	context->d2dContext->CreateSolidColorBrush(color, &tempBrush);
	context->d2dContext->SaveDrawingState(context->stateBlock.Get());
	context->d2dContext->BeginDraw();

	D2D1_RECT_F	rect;
	rect.left = screenPosition.x;
	rect.right = screenPosition.x + size.x;
	rect.top = screenPosition.y;
	rect.bottom = screenPosition.y + size.y;
	
	context->d2dContext->FillRectangle(&rect, tempBrush.Get());

	HRESULT hr = context->d2dContext->EndDraw();
	context->d2dContext->RestoreDrawingState(context->stateBlock.Get());
	tempBrush.Reset();
}

auto	D3D11Renderer::DrawRoundedRectangle(Vector2F const& screenPosition, Vector2F const& size, Vector2F const& edgesRadius, Vector4F const& colorValue) -> void
{
	D2D1::ColorF	color(colorValue.x, colorValue.y, colorValue.z, colorValue.w);

	ComPtr<ID2D1SolidColorBrush>	tempBrush = nullptr;
	context->d2dContext->CreateSolidColorBrush(color, &tempBrush);
	context->d2dContext->SaveDrawingState(context->stateBlock.Get());
	context->d2dContext->BeginDraw();

	D2D1_RECT_F	rect;
	rect.left = screenPosition.x;
	rect.right = screenPosition.x + size.x;
	rect.top = screenPosition.y;
	rect.bottom = screenPosition.y + size.y;
	D2D1_ROUNDED_RECT roundedRect = D2D1::RoundedRect(
		rect, edgesRadius.x, edgesRadius.y);

	context->d2dContext->DrawRoundedRectangle(&roundedRect, tempBrush.Get());

	HRESULT hr = context->d2dContext->EndDraw();
	context->d2dContext->RestoreDrawingState(context->stateBlock.Get());
	tempBrush.Reset();
}

auto	D3D11Renderer::DrawFilledRoundedRectangle(Vector2F const& screenPosition, Vector2F const& size, Vector2F const& edgesRadius, Vector4F const& colorValue) -> void
{
	D2D1::ColorF	color(colorValue.x, colorValue.y, colorValue.z, colorValue.w);

	ComPtr<ID2D1SolidColorBrush>	tempBrush = nullptr;
	context->d2dContext->CreateSolidColorBrush(color, &tempBrush);
	context->d2dContext->SaveDrawingState(context->stateBlock.Get());
	context->d2dContext->BeginDraw();

	D2D1_RECT_F	rect;
	rect.left = screenPosition.x;
	rect.right = screenPosition.x + size.x;
	rect.top = screenPosition.y;
	rect.bottom = screenPosition.y + size.y;
	D2D1_ROUNDED_RECT roundedRect = D2D1::RoundedRect(
		rect, edgesRadius.x, edgesRadius.y);

	context->d2dContext->FillRoundedRectangle(&roundedRect, tempBrush.Get());

	HRESULT hr = context->d2dContext->EndDraw();
	context->d2dContext->RestoreDrawingState(context->stateBlock.Get());
	tempBrush.Reset();
}

auto	D3D11Renderer::DrawEllipse(Vector2F const& position, Vector2F const& radius, Vector4F const& colorValue) -> void
{
	D2D1::ColorF	color(colorValue.x, colorValue.y, colorValue.z, colorValue.w);

	ComPtr<ID2D1SolidColorBrush>	tempBrush = nullptr;
	context->d2dContext->CreateSolidColorBrush(color, &tempBrush);
	context->d2dContext->SaveDrawingState(context->stateBlock.Get());
	context->d2dContext->BeginDraw();

	D2D1_ELLIPSE	ell;
	ell.point = D2D1::Point2F(position.x, position.y);
	ell.radiusX = radius.x;
	ell.radiusY = radius.y;

	context->d2dContext->DrawEllipse(&ell, tempBrush.Get());

	HRESULT hr = context->d2dContext->EndDraw();
	context->d2dContext->RestoreDrawingState(context->stateBlock.Get());
	tempBrush.Reset();
}

auto	D3D11Renderer::DrawFilledEllipse(Vector2F const& position, Vector2F const& radius, Vector4F const& colorValue) -> void
{
	D2D1::ColorF	color(colorValue.x, colorValue.y, colorValue.z, colorValue.w);

	ComPtr<ID2D1SolidColorBrush>	tempBrush = nullptr;
	context->d2dContext->CreateSolidColorBrush(color, &tempBrush);
	context->d2dContext->SaveDrawingState(context->stateBlock.Get());
	context->d2dContext->BeginDraw();

	D2D1_ELLIPSE	ell;
	ell.point = D2D1::Point2F(position.x, position.y);
	ell.radiusX = radius.x;
	ell.radiusY = radius.y;

	context->d2dContext->FillEllipse(&ell, tempBrush.Get());

	HRESULT hr = context->d2dContext->EndDraw();
	context->d2dContext->RestoreDrawingState(context->stateBlock.Get());
	tempBrush.Reset();
}
	
auto	D3D11Renderer::DrawLine(Vector2F const& point, Vector2F const& point2, Vector4F const& colorValue) -> void 
{
	D2D1::ColorF	color(colorValue.x, colorValue.y, colorValue.z, colorValue.w);

	ComPtr<ID2D1SolidColorBrush>	tempBrush = nullptr;
	context->d2dContext->CreateSolidColorBrush(color, &tempBrush);
	context->d2dContext->SaveDrawingState(context->stateBlock.Get());
	context->d2dContext->BeginDraw();

	D2D1_POINT_2F first;
	first.x = point.x;
	first.y = point.y;
	D2D1_POINT_2F second;
	second.x = point2.x;
	second.y = point2.y;

	context->d2dContext->DrawLine(first, second, tempBrush.Get());

	HRESULT hr = context->d2dContext->EndDraw();
	context->d2dContext->RestoreDrawingState(context->stateBlock.Get());
	tempBrush.Reset();
}

auto	D3D11Renderer::DrawGeometry(PolygonRenderResource* polygon, Vector4F const& colorValue) -> void
{
	ID2D1Geometry* geo = polygon->GetGeometry();
	if (!geo)
		return;

	D2D1::ColorF	color(colorValue.x, colorValue.y, colorValue.z, colorValue.w);

	ComPtr<ID2D1SolidColorBrush>	tempBrush = nullptr;
	context->d2dContext->CreateSolidColorBrush(color, &tempBrush);
	context->d2dContext->SaveDrawingState(context->stateBlock.Get());
	context->d2dContext->BeginDraw();

	context->d2dContext->DrawGeometry(geo, tempBrush.Get());

	HRESULT hr = context->d2dContext->EndDraw();
	context->d2dContext->RestoreDrawingState(context->stateBlock.Get());
	tempBrush.Reset();
}

auto	D3D11Renderer::DrawFilledGeometry(PolygonRenderResource* polygon, Vector4F const& colorValue) -> void
{
	ID2D1Geometry* geo = polygon->GetGeometry();
	if (!geo)
		return;

	D2D1::ColorF	color(colorValue.x, colorValue.y, colorValue.z, colorValue.w);

	ComPtr<ID2D1SolidColorBrush>	tempBrush = nullptr;
	context->d2dContext->CreateSolidColorBrush(color, &tempBrush);
	context->d2dContext->SaveDrawingState(context->stateBlock.Get());
	context->d2dContext->BeginDraw();

	context->d2dContext->FillGeometry(geo, tempBrush.Get());

	HRESULT hr = context->d2dContext->EndDraw();
	context->d2dContext->RestoreDrawingState(context->stateBlock.Get());
	tempBrush.Reset();

}

auto	D3D11Renderer::drawTexture(Vector4F const& screenRect, SubMeshData* texMesh, TextureRenderDataBase const& renderData) -> void
{
	D3D11TextureRenderData* d3d11RenderData = (D3D11TextureRenderData*)&renderData;
	Vector2F halfScreen = context->GetRenderTargetSize() * 0.5f;
	viewProjBufferData.View = Matrix4x4F::Transpose(Matrix4x4F::identity);
	viewProjBufferData.Projection = Matrix4x4F::Transpose(Matrix4x4F::identity);
	viewProjBufferData.Projection = Matrix4x4F::Transpose(Matrix4x4F::Orthographic(halfScreen.x * 2.0f, halfScreen.y * 2.0f, 0.0f, 100.0f));
	viewProjBuffer->UpdateBufferData(context, (void*)&viewProjBufferData);
	viewProjBuffer->BindBuffer(context);

	std::vector<float> fullVertices;
	Vector3F* vert = texMesh->GetVertices();
	Vector2F* uvs = texMesh->GetUV();
	for (unsigned int pos = 0; pos < texMesh->GetVerticesCount(); pos++)
	{
		fullVertices.push_back(vert[pos].x);
		fullVertices.push_back(vert[pos].y);
		fullVertices.push_back(vert[pos].z);
		fullVertices.push_back(uvs[pos].x);
		fullVertices.push_back(uvs[pos].y);
	}

	texMesh->GetVertexBuffer()->UpdateBufferData(context, fullVertices.data(), 20 * sizeof(float));

	context->GetDeviceContext()->PSSetShaderResources(0, 1, &d3d11RenderData->shaderView);
	context->GetDeviceContext()->PSSetSamplers(0, 1, &d3d11RenderData->samplerState);

	texMesh->GetVertexBuffer()->BindBuffer(context);
	texMesh->GetIndexBuffer()->BindBuffer(context);
	
	//context->GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//context->GetDeviceContext()->DrawIndexed(6, 0, 0);
}

auto	D3D11Renderer::InitializeTexture(TextureResource* tex) -> void
{
	D3D11TextureData* texData = (D3D11TextureData*)tex->GetTextureData();
	texData->InitializeD3D11Datas(context->GetDevice());

#ifdef _DEBUG
	//context->MarkD3D11ObjectName(tex->GetD3D11Resource(), MString("Texture : resource"));
	//context->MarkD3D11ObjectName(tex->GetShaderView(), MString("Texture : shader view"));
	//context->MarkD3D11ObjectName(tex->GetSamplerState(), MString("Texture : sampler state"));
#endif
}