#include "D3D11Renderer.hpp"

#include "D3D11Context.hpp"
#include "Mesh.hpp"
#include "Material.hpp"
#include "PolygonRenderResource.hpp"

auto	D3D11Renderer::Initialize() -> void
{
	CD3D11_BUFFER_DESC viewProjConstantBufferDesc(sizeof(ViewProjectionConstantBuffer), D3D11_BIND_CONSTANT_BUFFER);
	context->GetDevice()->CreateBuffer(&viewProjConstantBufferDesc, nullptr, &viewProjConstantBuffer);

	CD3D11_BUFFER_DESC modelConstantBufferDesc(sizeof(Matrix4x4F), D3D11_BIND_CONSTANT_BUFFER);
	context->GetDevice()->CreateBuffer(&modelConstantBufferDesc, nullptr, &modelConstantBuffer);

	float aspectRatio = 16.0f / 9.0f;//1280 / 720;
	float fovAngleY = 70.0f * DirectX::XM_PI / 180.0f;

	if (aspectRatio < 1.0f)
		fovAngleY *= 2.0f;
	DirectX::XMMATRIX perspectiveMatrix = DirectX::XMMatrixPerspectiveFovRH(
		fovAngleY, aspectRatio, 0.01f, 100.0f);
	perspectiveMatrix = DirectX::XMMatrixTranspose(perspectiveMatrix);

	for (unsigned int pos = 0; pos < 4; pos++)
	{
		for (unsigned int pos2 = 0; pos2 < 4; pos2++)
			viewProjBufferData.Projection[pos * 4 + pos2] = perspectiveMatrix.r[pos].m128_f32[pos2];
	}
	//DirectX::XMStoreFloat4x4(&viewProjBufferData.projection, perspectiveMatrix);

	context->GetDeviceContext()->UpdateSubresource(viewProjConstantBuffer.Get(), 0, NULL,
		&viewProjBufferData, 0, 0);
}

auto	D3D11Renderer::Shutdown() -> void
{
	*viewProjConstantBuffer.ReleaseAndGetAddressOf() = nullptr;
	*modelConstantBuffer.ReleaseAndGetAddressOf() = nullptr;
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

	for (unsigned int pos = 0; pos < 4; pos++)
	{
		for (unsigned int pos2 = 0; pos2 < 4; pos2++)
			viewProjBufferData.View[pos * 4 + pos2] = editorCamera.GetLocalMatrix().r[pos].m128_f32[pos2];
	}

	//DirectX::XMStoreFloat4x4(&viewProjBufferData.view, editorCamera.GetLocalMatrix());
	context->GetDeviceContext()->UpdateSubresource(viewProjConstantBuffer.Get(), 0, NULL,
		&viewProjBufferData, 0, 0);

	context->GetDeviceContext()->VSSetConstantBuffers(0, 1, viewProjConstantBuffer.GetAddressOf());
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
	
auto	D3D11Renderer::drawData(Mesh* mesh, Material* mat, DirectX::XMMATRIX const& modelMat) -> void
{
	mat->Bind(context->GetDeviceContext());

	Matrix4x4F model;
	for (unsigned int pos = 0; pos < 4; pos++)
	{
		for (unsigned int pos2 = 0; pos2 < 4; pos2++)
			model[pos * 4 + pos2] = modelMat.r[pos].m128_f32[pos2];
	}
	context->GetDeviceContext()->UpdateSubresource(modelConstantBuffer.Get(), 0, NULL,
		model.GetArray(), 0, 0);

	SubMeshData** subMeshes = mesh->GetSubMeshTab();
	for (unsigned int pos = 0; pos < mesh->GetSubMeshNbr(); pos++)
	{
		SubMeshData* subMesh = subMeshes[pos];
		if (!subMesh->IsInVRAM())
			subMesh->UploadInVRAM(context->GetDevice());
		UINT offset = 0;

		UINT stride = sizeof(DirectX::XMFLOAT3);
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