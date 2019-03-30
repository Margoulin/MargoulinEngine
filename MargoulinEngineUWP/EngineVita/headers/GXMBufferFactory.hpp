#ifndef __GXM_BUFFER_FACTORY_HPP__
#define __GXM_BUFFER_FACTORY_HPP__

#include "GPUBufferFactory.hpp"

class GXMBufferFactory : public GPUBufferFactory 
{
public:
	GXMBufferFactory() = default;
	GXMBufferFactory(const GXMBufferFactory&) = delete;
	GXMBufferFactory(GXMBufferFactory&&) = delete;
	~GXMBufferFactory() = default;

	virtual auto	SetContext(Context* value) -> void override {}

	virtual auto	GenerateVertexBuffer(SubMeshData* meshData, bool dynamic = false) -> GPUBuffer* { return nullptr; }
	virtual auto	GenerateVertexBuffer(SkeletalMesh* meshData, bool dynamic = false) -> GPUBuffer* { return nullptr; }
	virtual auto	GenerateVertexBuffer(unsigned int bufferSize, unsigned int stride, void* adress, bool dynamic = false) -> GPUBuffer* { return nullptr; }
	virtual auto	GenerateIndexBuffer(SubMeshData* meshData) -> GPUBuffer* { return nullptr; }
	virtual auto	GenerateIndexBuffer(SkeletalMesh* meshData) -> GPUBuffer* { return nullptr; }
	virtual auto	GenerateBuffer(unsigned int size) -> GPUBuffer* { return nullptr; }

	virtual auto	GenerateVertexBufferArray(unsigned int bufferCount, SkeletalMesh* mesh) -> GPUBufferArray* { return nullptr; }

	virtual auto	DeleteBuffer(GPUBuffer* buffer) -> void {}

	auto	operator = (const GXMBufferFactory&)->GXMBufferFactory& = delete;
	auto	operator = (GXMBufferFactory&&)->GXMBufferFactory& = delete;

protected:

private:

};


#endif /*__GXM_BUFFER_FACTORY_HPP__*/