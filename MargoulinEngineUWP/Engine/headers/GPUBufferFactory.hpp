#ifndef __GPU_BUFFER_FACTORY_HPP__
#define __GPU_BUFFER_FACTORY_HPP__

class GPUBuffer;
class SubMeshData;
class Context;

class GPUBufferFactory
{
public:
	GPUBufferFactory() = default;
	GPUBufferFactory(const GPUBufferFactory&) = delete;
	GPUBufferFactory(GPUBufferFactory&&) = delete;
	~GPUBufferFactory() = default;

	virtual auto	SetContext(Context* value) -> void { context = value; }

	virtual auto	GenerateVertexBuffer(SubMeshData* meshData) -> GPUBuffer* = 0;
	virtual auto	GenerateIndexBuffer(SubMeshData* meshData) -> GPUBuffer* = 0;
	virtual auto	GenerateBuffer(unsigned int size) -> GPUBuffer* = 0;

	virtual auto	DeleteBuffer(GPUBuffer* buffer) -> void = 0;

	auto	operator = (const GPUBufferFactory&)->GPUBufferFactory& = delete;
	auto	operator = (GPUBufferFactory&&)->GPUBufferFactory& = delete;

protected:
	Context* context = nullptr;

};


#endif /*__GPU_BUFFER_FACTORY_HPP__*/