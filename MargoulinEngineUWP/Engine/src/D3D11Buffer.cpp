#include "D3D11Buffer.hpp"

#include "D3D11Context.hpp"
#include "MemoryMacro.hpp"

D3D11BufferArray::D3D11BufferArray(unsigned int bufferNbr)
{
	buffers = NEW ID3D11Buffer*[bufferNbr];
	bufferStrides = NEW unsigned int[bufferNbr];
}

auto	D3D11BufferArray::BindBuffers(Context* context) -> void
{
	D3D11Context* d3d11Context = (D3D11Context*)context;

	if (bufferType == BufferType::VERTEX)
	{
		unsigned int offset[] = { 0, 0 };
		d3d11Context->GetDeviceContext()->IASetVertexBuffers(0, bufferCount, buffers, bufferStrides, offset);
	}
	else if (bufferType == BufferType::GENERAL)
		d3d11Context->GetDeviceContext()->VSSetConstantBuffers(bufferBindIndex, bufferCount, buffers);
}

auto	D3D11BufferArray::BindSingleBuffer(Context* context, unsigned int bufferID) -> void
{
	D3D11Context* d3d11Context = (D3D11Context*)context;
	if (bufferType == BufferType::VERTEX)
	{
		unsigned int offset = 0;
		d3d11Context->GetDeviceContext()->IASetVertexBuffers(0, 1, &buffers[bufferID], bufferStrides, &offset);
	}
	else if (bufferType == BufferType::GENERAL)
		d3d11Context->GetDeviceContext()->VSSetConstantBuffers(bufferBindIndex, 1, &buffers[bufferID]);
}

auto	D3D11BufferArray::Shutdown() -> void
{
	for (unsigned int pos = 0; pos < bufferCount; pos++)
	{
		if (buffers[pos])
			buffers[pos]->Release();
	}
	DELARRAY(buffers);
	DELARRAY(bufferStrides);
}

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
	
auto	D3D11Buffer::UpdateBufferData(Context* context, void* dataPtr, unsigned int size) -> void
{
	if (bufferType == BufferType::GENERAL)
	{
		D3D11Context* d3d11Context = (D3D11Context*)context;
		d3d11Context->GetDeviceContext()->UpdateSubresource(datas.Get(), 0, nullptr, dataPtr, 0, 0);
	}
	else if (bufferType == BufferType::VERTEX)
	{
		D3D11Context* d3d11Context = (D3D11Context*)context;
		D3D11_MAPPED_SUBRESOURCE mappedResource;
		d3d11Context->GetDeviceContext()->Map(datas.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
		memcpy(mappedResource.pData, dataPtr, size);
		d3d11Context->GetDeviceContext()->Unmap(datas.Get(), 0);
	}
}