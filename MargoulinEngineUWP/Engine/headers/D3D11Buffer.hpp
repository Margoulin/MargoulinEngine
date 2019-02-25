#ifndef __D3D11_BUFFER_HPP__
#define __D3D11_BUFFER_HPP__

#include "GPUBuffer.hpp"

#include <d3d11.h>
#include <wrl/client.h>

class D3D11Buffer : public GPUBuffer
{
public:
	D3D11Buffer(ID3D11Buffer* inBuffer) : datas(inBuffer) {}
	D3D11Buffer(const D3D11Buffer&) = delete;
	D3D11Buffer(D3D11Buffer&&) = delete;
	~D3D11Buffer() = default;

	virtual auto	BindBuffer(Context* context) -> void;
	virtual auto	UpdateBufferData(Context* context, void* dataPtr, unsigned int size = 0) -> void;
	
	auto	Shutdown() -> void { *datas.ReleaseAndGetAddressOf() = nullptr; }

	auto	SetBufferBindIndex(unsigned int const& value) -> void { bufferBindIndex = value; }
	auto	SetStride(unsigned int const& value) -> void { bufferStride = value; }

	auto	GetBuffer() const -> ID3D11Buffer* { return datas.Get(); }
	auto	GetBufferPtr() const -> ID3D11Buffer* const* { return datas.GetAddressOf(); }
	auto	GetBufferStride() const -> unsigned int { return bufferStride; }
	auto	GetBufferBindIndex() const -> unsigned int { return bufferBindIndex; }

	auto	operator = (const D3D11Buffer&)->D3D11Buffer& = delete;
	auto	operator = (D3D11Buffer&&)->D3D11Buffer& = delete;

protected:

private:
	Microsoft::WRL::ComPtr<ID3D11Buffer>	datas;
	unsigned int							bufferBindIndex = 0;
	unsigned int							bufferStride = 0;
};


#endif /*__D3D11_BUFFER_HPP__*/