#include "D3D11Renderer.hpp"

#include "D3D11Context.hpp"
#include "Mesh.hpp"
#include "Material.hpp"
#include "PolygonRenderResource.hpp"

#include "D3D11PixelShader.hpp"
#include "D3D11VertexShader.hpp"
#include "ShaderFactory.hpp"

#include "./Logger.hpp"

#include "Engine.hpp"
#include "GraphicalLibrary.hpp"
#include "D3D11VertexShader.hpp"

auto	D3D11Renderer::Initialize() -> void
{
	CD3D11_BUFFER_DESC viewProjConstantBufferDesc(sizeof(ViewProjectionConstantBuffer), D3D11_BIND_CONSTANT_BUFFER);
	context->GetDevice()->CreateBuffer(&viewProjConstantBufferDesc, nullptr, &viewProjConstantBuffer);

	CD3D11_BUFFER_DESC modelConstantBufferDesc(sizeof(Matrix4x4F), D3D11_BIND_CONSTANT_BUFFER);
	context->GetDevice()->CreateBuffer(&modelConstantBufferDesc, nullptr, &modelConstantBuffer);

	context->GetDeviceContext()->UpdateSubresource(viewProjConstantBuffer.Get(), 0, NULL,
		&viewProjBufferData, 0, 0);

	D3D11_SUBRESOURCE_DATA vertexBufferData = { 0 };
	vertexBufferData.pSysMem = textureVertices;
	vertexBufferData.SysMemPitch = 0;
	vertexBufferData.SysMemSlicePitch = 0;
	CD3D11_BUFFER_DESC vertexBufferDesc(20 * (unsigned int)(sizeof(float)), D3D11_BIND_VERTEX_BUFFER);
	vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	context->GetDevice()->CreateBuffer(&vertexBufferDesc, &vertexBufferData, &textureVertexBuffer);

	D3D11_SUBRESOURCE_DATA indexBufferData = { 0 };
	indexBufferData.pSysMem = textureIndices;
	indexBufferData.SysMemPitch = 0;
	indexBufferData.SysMemSlicePitch = 0;
	CD3D11_BUFFER_DESC indexBufferDesc((unsigned int)sizeof(unsigned int) * 6, D3D11_BIND_INDEX_BUFFER);
	context->GetDevice()->CreateBuffer(&indexBufferDesc, &indexBufferData, &textureIndexBuffer);

	textureVertices[2] = 0.0f;
	textureVertices[7] = 0.0f;
	textureVertices[12] = 0.0f;
	textureVertices[17] = 0.0f;
	setTextureVertices(0, Vector2F(0.0f, 1.0f), Vector2F(0.0f, 0.0f));
	setTextureVertices(1, Vector2F(1.0f, 1.0f), Vector2F(1.0f, 0.0f));
	setTextureVertices(2, Vector2F(1.0f, 0.0f), Vector2F(1.0f, 1.0f));
	setTextureVertices(3, Vector2F(0.0f, 0.0f), Vector2F(0.0f, 1.0f));

#ifdef  _DEBUG
	context->MarkD3D11ObjectName(viewProjConstantBuffer.Get(), MString("View Proj Constant Buffer"));
	context->MarkD3D11ObjectName(modelConstantBuffer.Get(), MString("Model Constant Buffer"));
	context->MarkD3D11ObjectName(textureVertexBuffer.Get(), MString("Texture Vertex Buffer"));
	context->MarkD3D11ObjectName(textureIndexBuffer.Get(), MString("Texture Index Buffer"));
#endif //_DEBUG

}

auto	D3D11Renderer::Shutdown() -> void
{
	*viewProjConstantBuffer.ReleaseAndGetAddressOf() = nullptr;
	*modelConstantBuffer.ReleaseAndGetAddressOf() = nullptr;
	*textureVertexBuffer.ReleaseAndGetAddressOf() = nullptr;
	*textureIndexBuffer.ReleaseAndGetAddressOf() = nullptr;
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

	context->GetDeviceContext()->VSSetConstantBuffers(1, 1, modelConstantBuffer.GetAddressOf());
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

	context->GetDeviceContext()->UpdateSubresource(viewProjConstantBuffer.Get(), 0, NULL,
		&viewProjBufferData, 0, 0);

	context->GetDeviceContext()->VSSetConstantBuffers(0, 1, viewProjConstantBuffer.GetAddressOf());
}

auto	D3D11Renderer::drawData(Mesh* mesh, Material* mat, Matrix4x4F const& modelMat) -> void
{
	mat->Bind(context->GetDeviceContext());

	context->GetDeviceContext()->UpdateSubresource(modelConstantBuffer.Get(), 0, NULL,
		Matrix4x4F::Transpose(modelMat).GetArray(), 0, 0);

	SubMeshData** subMeshes = mesh->GetSubMeshTab();
	for (unsigned int pos = 0; pos < mesh->GetSubMeshNbr(); pos++)
	{
		SubMeshData* subMesh = subMeshes[pos];
		if (!subMesh->IsInVRAM())
			subMesh->UploadInVRAM(context->GetDevice());
		UINT offset = 0;

		UINT stride = sizeof(Vector3F);
		context->GetDeviceContext()->IASetVertexBuffers(0, 1, subMesh->GetVertexBuffer(), &stride, &offset);
		context->GetDeviceContext()->IASetIndexBuffer(subMesh->GetIndexBuffer(),
			DXGI_FORMAT_R32_UINT, 0);
		context->GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		context->GetDeviceContext()->DrawIndexed(subMesh->GetIndicesCount(), 0, 0);
	}
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

auto	D3D11Renderer::drawTexture(Vector4F const& screenRect, TextureRenderData const& renderData) -> void
{
	Vector2F halfScreen = context->GetRenderTargetSize() * 0.5f;
	viewProjBufferData.View = Matrix4x4F::Transpose(Matrix4x4F::identity);
	viewProjBufferData.Projection = Matrix4x4F::Transpose(Matrix4x4F::identity);
	viewProjBufferData.Projection = Matrix4x4F::Transpose(Matrix4x4F::Orthographic(halfScreen.x * 2.0f, halfScreen.y * 2.0f, 0.0f, 100.0f));
	context->GetDeviceContext()->UpdateSubresource(viewProjConstantBuffer.Get(), 0, NULL,
		&viewProjBufferData, 0, 0);
	context->GetDeviceContext()->VSSetConstantBuffers(0, 1, viewProjConstantBuffer.GetAddressOf());

	float left = -halfScreen.x + screenRect.x;
	float right = left + screenRect.w;
	float top = halfScreen.y - screenRect.y;
	float bottom = top - screenRect.z;

	setTextureVertices(0, Vector2F(left, top), Vector2F(renderData.textureRect.x, renderData.textureRect.y));
	setTextureVertices(1, Vector2F(right, top), Vector2F(renderData.textureRect.w, renderData.textureRect.y));
	setTextureVertices(2, Vector2F(right, bottom), Vector2F(renderData.textureRect.w, renderData.textureRect.z));
	setTextureVertices(3, Vector2F(left, bottom), Vector2F(renderData.textureRect.x, renderData.textureRect.z));

	D3D11_MAPPED_SUBRESOURCE mappedResource;
	context->GetDeviceContext()->Map(textureVertexBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	memcpy(mappedResource.pData, textureVertices, 20 * sizeof(float));
	context->GetDeviceContext()->Unmap(textureVertexBuffer.Get(), 0);

	context->GetDeviceContext()->PSSetShaderResources(0, 1, &renderData.shaderView);
	context->GetDeviceContext()->PSSetSamplers(0, 1, &renderData.samplerState);

	UINT offset = 0;
	UINT stride = sizeof(float) * 5;
	context->GetDeviceContext()->IASetVertexBuffers(0, 1, textureVertexBuffer.GetAddressOf(), &stride, &offset);
	context->GetDeviceContext()->IASetIndexBuffer(textureIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
	context->GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	context->GetDeviceContext()->DrawIndexed(6, 0, 0);
}

auto	D3D11Renderer::setTextureVertices(unsigned int const& count, Vector2F const& screenPos, Vector2F const& uv) -> void
{
	if (count > 3)
		return;

	textureVertices[count * 5] = screenPos.x;
	textureVertices[count * 5 + 1] = screenPos.y;
	textureVertices[count * 5 + 3] = uv.x;
	textureVertices[count * 5 + 4] = uv.y;
}
	
auto	D3D11Renderer::InitializeTexture(TextureResource* tex) -> void
{
	tex->InitializeD3D11Datas(context->GetDevice());

#ifdef _DEBUG
	context->MarkD3D11ObjectName(tex->GetD3D11Resource(), MString("Texture : resource"));
	context->MarkD3D11ObjectName(tex->GetShaderView(), MString("Texture : shader view"));
	context->MarkD3D11ObjectName(tex->GetSamplerState(), MString("Texture : sampler state"));
#endif
}