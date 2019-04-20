#include "GPUMemoryManager.hpp"

GPUUsseAdress::GPUUsseAdress(void* vAdress, SceUID uid, unsigned int memorySize, bool isVertexUsse)
	:GPUAdressBase(vAdress, uid, memorySize)
{
	this->isVertexUsse = isVertexUsse;
}

GPUUsseAdress::~GPUUsseAdress()
{
	void* tempMem = nullptr;
	if (sceKernelGetMemBlockBase(gpuUID, &tempMem) < 0)
		return;
	if (isVertexUsse)
		sceGxmUnmapVertexUsseMemory(tempMem);
	else
		sceGxmUnmapFragmentUsseMemory(tempMem);
	sceKernelFreeMemBlock(gpuUID);
}

GPUAdressBase*	GPUMemoryManager::AllocateCustom(SceKernelMemBlockType type, unsigned int memrequired, SceGxmMemoryAttribFlags flags)
{
	unsigned int size = 0;
	if (type == SCE_KERNEL_MEMBLOCK_TYPE_USER_CDRAM_RW) {
		size = ALIGN(memrequired, 256 * 1024);
	}
	else {
		size = ALIGN(memrequired, 4 * 1024);
	}

	SceUID tempUID = sceKernelAllocMemBlock("gpu_mem", type, size, nullptr);

	void* tempAdress = nullptr;
	if (tempUID < 0)
		return nullptr;

	if (sceKernelGetMemBlockBase(tempUID, &tempAdress) < 0)
		return nullptr;

	if (sceGxmMapMemory(tempAdress, size, flags) < 0)
		return nullptr;

	return NEW GPUAdressBase(tempAdress, tempUID, size);
}

GPUUsseAdress*	GPUMemoryManager::AllocateUsse(unsigned int memrequired, bool isVertex, unsigned int* outBufferOffset)
{
	unsigned int size = ALIGN(memrequired, 4096);

	SceUID tempUID = sceKernelAllocMemBlock(isVertex? "vertex_usse" : "fragment_usse", SCE_KERNEL_MEMBLOCK_TYPE_USER_RW_UNCACHE, size, nullptr);

	void* tempAdress = nullptr;
	if (tempUID < 0)
		return nullptr;

	if (sceKernelGetMemBlockBase(tempUID, &tempAdress) < 0)
		return nullptr;

	if (isVertex)
	{
		if (sceGxmMapVertexUsseMemory(tempAdress, size, outBufferOffset) < 0)
			return nullptr;
	}
	else
	{
		if (sceGxmMapFragmentUsseMemory(tempAdress, size, outBufferOffset) < 0)
			return nullptr;
	}
	return NEW GPUUsseAdress(tempAdress, tempUID, size, isVertex);
}