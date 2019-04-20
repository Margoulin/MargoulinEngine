#ifndef __GPU_MEMORY_MANAGER_HPP__
#define __GPU_MEMORY_MANAGER_HPP__

#include <psp2/gxm.h>
#include <psp2/kernel/sysmem.h>
#include <psp2/types.h>
#include "MemoryMacro.hpp"

#define ALIGN(x, a)	(((x) + ((a) - 1)) & ~((a) - 1))


class GPUAdressBase
{
public:
	template<class OtherType>
	struct CustomIterator
	{
		CustomIterator(GPUAdressBase* ref)
		{
			adress = ref->GetVoidAdress();
			stride = sizeof(OtherType);
			arrayMemSize = ref->GetMemorySize();
		}

		void	MoveNext() { currentIdx++; }

		bool	IsValid() { return currentIdx * stride < arrayMemSize; }

		OtherType&	operator*() { return adress[currentIdx]; }
		OtherType&	operator->() { return adress[currentIdx]; }

		CustomIterator& operator++() { return *this; }

		OtherType*		adress = nullptr;
		unsigned int	currentIdx = 0;
		size_t			stride = 0;
		unsigned int	arrayMemSize = 0;
	};

	GPUAdressBase(void* vAdress, SceUID uid, unsigned int memorySize)
	{
		this->vAdress = vAdress;
		this->gpuUID = uid;
		this->memorySize = memorySize;
	}
	GPUAdressBase(const GPUAdressBase&) = delete;
	GPUAdressBase(GPUAdressBase&&) = delete;
	virtual ~GPUAdressBase()
	{
		vAdress = nullptr;

		if (sceKernelGetMemBlockBase(gpuUID, &vAdress) < 0)
			return;
		sceGxmUnmapMemory(vAdress);
		sceKernelFreeMemBlock(gpuUID);
	}
	
	virtual auto	GetVoidAdress() const -> void* { return vAdress; }
	virtual auto	GetMemorySize() const -> unsigned int { return memorySize; }
	template<typename Other>	
	auto	GetCustomIterator() const -> CustomIterator<Other> { return CustomIterator<Other>(this); }

	auto	operator = (const GPUAdressBase&)->GPUAdressBase& = delete;
	auto	operator = (GPUAdressBase&&)->GPUAdressBase& = delete;

protected:
	SceUID			gpuUID;
	void*			vAdress = nullptr;
	unsigned int	memorySize = 0;
};


template<class AdressType>
class GPUAdress : public GPUAdressBase
{
public:
	struct Iterator
	{
		Iterator(const GPUAdress* ref)
		{
			adress = ref->Get();
			arraySize = ref->GetSize();
		}

		void	MoveNext() { currentIdx++; }

		bool	IsValid()  { return currentIdx < arraySize; }

		AdressType&	operator*() { return adress[currentIdx]; }
		AdressType&	operator->() { return adress[currentIdx]; }

		Iterator& operator++() { return *this; }

		AdressType*		adress = nullptr;
		unsigned int	currentIdx = 0;
		unsigned int	arraySize = 0;
	};

	GPUAdress(void* vAdress, SceUID uid, unsigned int valuesNbr, unsigned int memorySize)
		:GPUAdressBase(vAdress, uid, memorySize)
	{
		this->adress = (AdressType*)vAdress;
		this->size = valuesNbr;
	}
	GPUAdress(const GPUAdress&) = delete;
	GPUAdress(GPUAdress&&) = delete;
	virtual ~GPUAdress()
	{
		adress = nullptr;
		size = 0;
	}

	AdressType& operator[](unsigned int idx) { return adress[idx]; }
	auto	Get() const -> AdressType* { return adress; }
	auto	GetSize() const -> unsigned int { return size; }
	auto	GetIterator() const -> Iterator { return Iterator(this); }

protected:
	AdressType*		adress = nullptr;
	unsigned int	size = 0;
};


class GPUUsseAdress : public GPUAdressBase
{
public:
	GPUUsseAdress(void* vAdress, SceUID uid, unsigned int memorySize, bool isVertexUsse);
	GPUUsseAdress(const GPUUsseAdress&) = delete;
	GPUUsseAdress(GPUUsseAdress&&) = delete;
	virtual ~GPUUsseAdress() override;

	auto	operator = (const GPUUsseAdress&)->GPUUsseAdress& = delete;
	auto	operator = (GPUUsseAdress&&)->GPUUsseAdress& = delete;

protected:

private:
	bool	isVertexUsse = false;
};


class GPUMemoryManager
{
public:
	GPUMemoryManager() = default;
	GPUMemoryManager(const GPUMemoryManager&) = delete;
	GPUMemoryManager(GPUMemoryManager&&) = delete;
	~GPUMemoryManager() = default;

	//static	auto	Initialize() -> void;
	//static	auto	Shutdown() -> void;

	template <typename ValueType> 
	static GPUAdress<ValueType>*	Allocate(unsigned int allocationsRequiredNbrs, SceKernelMemBlockType type = SCE_KERNEL_MEMBLOCK_TYPE_USER_RW_UNCACHE, SceGxmMemoryAttribFlags flags = SCE_GXM_MEMORY_ATTRIB_READ)
	{
		void* tempAdress = nullptr;

		size_t valueSize = sizeof(ValueType);
		unsigned int size = 0;
		if (type == SCE_KERNEL_MEMBLOCK_TYPE_USER_CDRAM_RW) {
			size = ALIGN(valueSize * allocationsRequiredNbrs, 256 * 1024);
		}
		else {
			size = ALIGN(valueSize * allocationsRequiredNbrs, 4 * 1024);
		}

		SceUID tempUID = sceKernelAllocMemBlock("gpu_mem", type, size, nullptr);

		if (tempUID < 0)
			return nullptr;

		if (sceKernelGetMemBlockBase(tempUID, &tempAdress) < 0)
			return nullptr;

		if (sceGxmMapMemory(tempAdress, size, flags) < 0)
			return nullptr;

		return NEW GPUAdress<ValueType>(tempAdress, tempUID, allocationsRequiredNbrs, size);
	}
	static GPUAdressBase*	AllocateCustom(SceKernelMemBlockType type, unsigned int memrequired, SceGxmMemoryAttribFlags flags);
	static GPUUsseAdress*	AllocateUsse(unsigned int memrequired, bool isVertex, unsigned int* outBufferOffset);

	auto	operator = (const GPUMemoryManager&)->GPUMemoryManager& = delete;
	auto	operator = (GPUMemoryManager&&)->GPUMemoryManager& = delete;

protected:

private:
	static	GPUMemoryManager*	instance;

};


#endif /*__GPUMemoryManager_HPP__*/