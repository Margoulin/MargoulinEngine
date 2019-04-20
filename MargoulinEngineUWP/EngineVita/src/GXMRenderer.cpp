#include "GXMRenderer.hpp"

#include "GXMContext.hpp"
#include "GXMVertexShader.hpp"
#include "GXMFragmentShader.hpp"

#include <psp2/gxm.h>
#include <psp2/kernel/sysmem.h>
#include "GXMBuffer.hpp"

GXMRenderer::GXMRenderer()
{
	clearVertices = GPUMemoryManager::Allocate<Vector2F>(3, SCE_KERNEL_MEMBLOCK_TYPE_USER_RW_UNCACHE, SCE_GXM_MEMORY_ATTRIB_READ);
	clearIndices = GPUMemoryManager::Allocate<unsigned int>(3, SCE_KERNEL_MEMBLOCK_TYPE_USER_RW_UNCACHE, SCE_GXM_MEMORY_ATTRIB_READ);

	clearVertices->Get()[0].x = -1.0f;
	clearVertices->Get()[0].y = -1.0f;
	clearVertices->Get()[1].x = 3.0f;
	clearVertices->Get()[1].y = -1.0f;
	clearVertices->Get()[2].x = -1.0f;
	clearVertices->Get()[2].y = 3.0f;

	clearIndices->Get()[0] = 0;
	clearIndices->Get()[1] = 1;
	clearIndices->Get()[2] = 2;
}

GXMRenderer::~GXMRenderer()
{
	DEL(clearVertices);
	DEL(clearIndices);
}

auto	GXMRenderer::BeginRender() -> void
{
	sceGxmBeginScene(context->GetContext(), 0, context->GetRenderTarget(), nullptr, nullptr,
		context->GetBackBufferSyncObject(), context->GetBackBufferColorSurface(), context->GetDepthStencilSurface());

	clearVtxShader->BindShader(context);
	clearFragShader->BindShader(context);

	void* color_buffer;
	sceGxmReserveFragmentDefaultUniformBuffer(context->GetContext(), &color_buffer);
	clearFragShader->GetParameter(0)->UpdateParameterData(color_buffer, 4, clearColor);

	sceGxmSetVertexStream(context->GetContext(), 0, clearVertices->Get());
	sceGxmDraw(context->GetContext(), SCE_GXM_PRIMITIVE_TRIANGLES,
		SCE_GXM_INDEX_FORMAT_U32, clearIndices->Get(), 3);
}

auto	GXMRenderer::EndRender() -> void
{
	sceGxmEndScene(context->GetContext(), nullptr, nullptr);
}

auto	GXMRenderer::Present() -> void
{
	sceGxmPadHeartbeat(context->GetBackBufferColorSurface(), context->GetBackBufferSyncObject());

	DisplayData	dispData;
	dispData.Address = context->GetBackBufferData();
	sceGxmDisplayQueueAddEntry(context->GetFrontBufferSyncObject(), context->GetBackBufferSyncObject(), &dispData);

	context->UpdateBufferID();
}