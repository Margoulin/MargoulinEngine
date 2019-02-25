#ifndef __D3D11_BUFFER_FACTORY_HPP__
#define __D3D11_BUFFER_FACTORY_HPP__

#include "GPUBufferFactory.hpp"

class D3D11Context;
class Context;

class D3D11BufferFactory : public GPUBufferFactory
{
public:
	D3D11BufferFactory() = default;
	D3D11BufferFactory(const D3D11BufferFactory&) = delete;
	D3D11BufferFactory(D3D11BufferFactory&&) = delete;
	~D3D11BufferFactory() = default;

	virtual auto	SetContext(Context* value) -> void override;
	
	virtual auto	GenerateVertexBuffer(SubMeshData* meshData) -> GPUBuffer*;
	virtual auto	GenerateIndexBuffer(SubMeshData* meshData) -> GPUBuffer*;
	virtual auto	GenerateBuffer(unsigned int size) -> GPUBuffer*;

	virtual auto	DeleteBuffer(GPUBuffer* buffer) -> void;

	auto	operator = (const D3D11BufferFactory&)->D3D11BufferFactory& = delete;
	auto	operator = (D3D11BufferFactory&&)->D3D11BufferFactory& = delete;

protected:

private:
	D3D11Context*	d3d11Context = nullptr;
};


#endif /*__D3D11_BUFFER_FACTORY_HPP__*/