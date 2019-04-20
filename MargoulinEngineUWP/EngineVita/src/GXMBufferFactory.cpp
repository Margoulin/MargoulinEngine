#include "GXMBufferFactory.hpp"

#include "SubMeshData.hpp"
#include "GXMBuffer.hpp"

#include "MemoryMacro.hpp"

auto	GXMBufferFactory::GenerateVertexBuffer(SubMeshData* meshData, bool dynamic) -> GPUBuffer*
{
	GPUAdress<float>*	adress = GPUMemoryManager::Allocate<float>(meshData->GetVerticesCount() * 3, 
		SCE_KERNEL_MEMBLOCK_TYPE_USER_RW_UNCACHE, dynamic? SCE_GXM_MEMORY_ATTRIB_RW : SCE_GXM_MEMORY_ATTRIB_READ);
	for (unsigned int pos = 0; pos < meshData->GetVerticesCount(); pos++)
	{
		adress->Get()[pos] = meshData->GetVertices()[pos].x;
		adress->Get()[pos + 1] = meshData->GetVertices()[pos].x;
		adress->Get()[pos + 2] = meshData->GetVertices()[pos].x;
	}
	
	return NEW GXMBuffer(BufferType::VERTEX, adress);
}

auto	GXMBufferFactory::GenerateVertexBuffer(SkeletalMesh* meshData, bool dynamic) -> GPUBuffer*
{
	return nullptr;
}

auto	GXMBufferFactory::GenerateVertexBuffer(unsigned int bufferSize, unsigned int stride, void* adress, bool dynamic) -> GPUBuffer*
{
	GPUAdressBase*	gpuAdress = GPUMemoryManager::AllocateCustom(SCE_KERNEL_MEMBLOCK_TYPE_USER_RW_UNCACHE, 
		bufferSize, dynamic ? SCE_GXM_MEMORY_ATTRIB_RW : SCE_GXM_MEMORY_ATTRIB_READ);
	
	return NEW GXMBuffer(BufferType::VERTEX, gpuAdress);
}

auto	GXMBufferFactory::GenerateIndexBuffer(SubMeshData* meshData) -> GPUBuffer*
{
	GPUAdress<unsigned int>* adress = GPUMemoryManager::Allocate<unsigned int>(meshData->GetIndicesCount(), 
		SCE_KERNEL_MEMBLOCK_TYPE_USER_RW_UNCACHE, SCE_GXM_MEMORY_ATTRIB_READ);
	return NEW GXMBuffer(BufferType::INDEX, adress);
}

auto	GXMBufferFactory::GenerateIndexBuffer(SkeletalMesh* meshData) -> GPUBuffer*
{
	return nullptr;
}

auto	GXMBufferFactory::GenerateBuffer(unsigned int size) -> GPUBuffer*
{
	GPUAdressBase*	adress = GPUMemoryManager::AllocateCustom(SCE_KERNEL_MEMBLOCK_TYPE_USER_RW_UNCACHE, size, SCE_GXM_MEMORY_ATTRIB_RW);

	return NEW GXMBuffer(BufferType::GENERAL, adress);
}

auto	GXMBufferFactory::DeleteBuffer(GPUBuffer* buffer) -> void
{
	buffer->Shutdown();
	DEL(buffer);
}