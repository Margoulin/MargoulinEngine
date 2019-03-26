#ifndef __GPU_BUFFER_HPP__
#define __GPU_BUFFER_HPP__

enum BufferType
{
	VERTEX,
	INDEX,
	GENERAL
};

class Context;


class GPUBufferArray
{
public:
	GPUBufferArray() = default;
	GPUBufferArray(const GPUBufferArray&) = delete;
	GPUBufferArray(GPUBufferArray&&) = delete;
	~GPUBufferArray() = default;

	virtual auto	BindBuffers(Context* context) -> void = 0;
	virtual auto	BindSingleBuffer(Context* context, unsigned int bufferID) -> void = 0;

	virtual auto	SetBufferCount(unsigned int const& value) -> void { bufferCount = value; }
	virtual auto	SetInitializedBufferCount(unsigned int const& value) -> void { initializedBufferCount = value; }
	virtual auto	SetBufferType(BufferType value) -> void { bufferType = value; }

	auto	operator = (const GPUBufferArray&)->GPUBufferArray& = delete;
	auto	operator = (GPUBufferArray&&)->GPUBufferArray& = delete;

protected:
	BufferType		bufferType;
	unsigned int	bufferCount = 0;
	unsigned int	initializedBufferCount = 0;
};


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