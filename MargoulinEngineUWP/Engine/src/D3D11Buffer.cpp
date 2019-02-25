#include "D3D11Buffer.hpp"

#include "D3D11Context.hpp"

auto	D3D11Buffer::BindBuffer(Context* context) -> void
{
	D3D11Context* d3d11Context = (D3D11Context*)context;
	if (bufferType == BufferType::VERTEX)
	{
		unsigned int offset = 0;
		d3d11Context->GetDeviceContext()->IASetVertexBuffers(0, 1, datas.GetAddressOf(), &bufferStride, &offset);
	}
	else if (bufferType == BufferType::INDEX)
		d3d11Context->GetDeviceContext()->IASetIndexBuffer(datas.Get(), DXGI_FORMAT_R32_UINT, 0);
	else if (bufferType == BufferType::GENERAL)
		d3d11Context->GetDeviceContext()->VSSetConstantBuffers(bufferBindIndex, 1, datas.GetAddressOf());
}
	
auto	D3D11Buffer::UpdateBufferData(Context* context, void* dataPtr) -> void
{
	D3D11Context* d3d11Context = (D3D11Context*)context;
	d3d11Context->GetDeviceContext()->UpdateSubresource(datas.Get(), 0, nullptr, dataPtr, 0, 0);
}