#include "GXMBuffer.hpp"

#include "GXMContext.hpp"

auto	GXMBuffer::BindBuffer(Context* context) -> void
{
	GXMContext* gxmContext = (GXMContext*)context;
	if (bufferType == BufferType::VERTEX)
		sceGxmSetVertexStream(gxmContext->GetContext(), 0, bufferData->GetVoidAdress());
	else if (bufferType == BufferType::GENERAL)
	{

	}
}
	
auto	GXMBuffer::UpdateBufferData(Context* context, void* dataPtr, unsigned int size) -> void
{

}