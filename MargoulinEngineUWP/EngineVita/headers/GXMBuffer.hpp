#ifndef __GXM_BUFFER_HPP__
#define __GXM_BUFFER_HPP__

#include "GPUBuffer.hpp"

#include "GPUMemoryManager.hpp"

class Context;

class GXMBuffer : public GPUBuffer
{
public:
	GXMBuffer(BufferType const& type, GPUAdressBase* gpuAdress) : GPUBuffer(type) { bufferData = gpuAdress; }
	GXMBuffer(const GXMBuffer&) = delete;
	GXMBuffer(GXMBuffer&&) = delete;
	~GXMBuffer() = default;

	virtual auto	Shutdown() -> void { DEL(bufferData); }

	virtual auto	BindBuffer(Context* context) -> void;
	virtual auto	UpdateBufferData(Context* context, void* dataPtr, unsigned int size = 0) -> void;

	auto	GetVoidAdress() -> void* { return bufferData->GetVoidAdress(); }

	auto	operator=(const GXMBuffer&)->GXMBuffer& = delete;
	auto	operator=(GXMBuffer&&)->GXMBuffer& = delete;

protected:

private:
	GPUAdressBase*	bufferData = nullptr;
};


#endif /*__GXM_BUFFER_HPP__*/