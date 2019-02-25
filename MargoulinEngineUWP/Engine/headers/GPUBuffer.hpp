#ifndef __GPU_BUFFER_HPP__
#define __GPU_BUFFER_HPP__

enum BufferType
{
	VERTEX,
	INDEX,
	GENERAL
};

class Context;

class GPUBuffer
{
public:
	GPUBuffer() = default;
	GPUBuffer(const GPUBuffer&) = delete;
	GPUBuffer(GPUBuffer&&) = delete;
	~GPUBuffer() = default;

	virtual auto	BindBuffer(Context* context) -> void = 0;
	virtual auto	UpdateBufferData(Context* context, void* dataPtr, unsigned int size = 0) -> void = 0;

	virtual auto	SetBufferType(BufferType value) -> void { bufferType = value; }

	auto	operator = (const GPUBuffer&)->GPUBuffer& = delete;
	auto	operator = (GPUBuffer&&)->GPUBuffer& = delete;

protected:
	BufferType	bufferType;
private:
	
};


#endif /*__GPU_BUFFER_HPP__*/