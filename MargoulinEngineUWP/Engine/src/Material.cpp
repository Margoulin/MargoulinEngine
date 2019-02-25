#include "Material.hpp"

#include "D3D11PixelShader.hpp"

auto	Material::Initialize(ID3D11Device* device) -> void
{
	CD3D11_BUFFER_DESC unlitColorConstantBufferDesc(sizeof(Vector4F), D3D11_BIND_CONSTANT_BUFFER);
	device->CreateBuffer(&unlitColorConstantBufferDesc, nullptr, &unlitColorConstantBuffer);
}

auto	Material::Shutdown() -> void
{
	*unlitColorConstantBuffer.ReleaseAndGetAddressOf() = nullptr;
}

auto	Material::Bind(ID3D11DeviceContext* context) -> void
{
	context->UpdateSubresource(unlitColorConstantBuffer.Get(), 0, NULL,
		&unlitColor, 0, 0);
	context->VSSetConstantBuffers(2, 1, unlitColorConstantBuffer.GetAddressOf());
}

#include <imgui.h>

#ifdef _DEBUG

auto	Material::ImGuiUpdate() -> void
{
	ImGui::ColorEdit3("Unlit Color", (float*)&unlitColor.x);
}

#endif // _DEBUG
