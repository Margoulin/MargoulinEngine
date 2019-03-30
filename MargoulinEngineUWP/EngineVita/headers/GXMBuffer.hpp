#ifndef __GXM_BUFFER_HPP__
#define __GXM_BUFFER_HPP__

#include "GPUBuffer.hpp"

class Context;

class GXMBuffer : public GPUBuffer
{
public:
	GXMBuffer() = default;
	GXMBuffer(const GXMBuffer&) = delete;
	GXMBuffer(GXMBuffer&&) = delete;
	~GXMBuffer() = default;

	virtual auto	BindBuffer(Context* context) -> void {}
	virtual auto	UpdateBufferData(Context* context, void* dataPtr, unsigned int size = 0) -> void;

	auto	operator = (const GXMBuffer&)->GXMBuffer& = delete;
	auto	operator = (GXMBuffer&&)->GXMBuffer& = delete;

protected:

private:

};


#endif /*__GXM_BUFFER_HPP__*/