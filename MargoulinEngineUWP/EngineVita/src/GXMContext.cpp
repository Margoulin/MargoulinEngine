#include "GXMContext.hpp"

#include <string.h>
#include <psp2/display.h>

int	vBlankWait = 1;

void	DisplayCallback(const void* callbackData)
{
	SceDisplayFrameBuf framebuf;
	const DisplayData*	displayData = (const DisplayData*)callbackData;

	memset(&framebuf, 0x00, sizeof(SceDisplayFrameBuf));
	framebuf.size = sizeof(SceDisplayFrameBuf);
	framebuf.base = displayData->Address;
	framebuf.pitch = DISPLAY_STRIDE_IN_PIXELS;
	framebuf.pixelformat = DISPLAY_PIXEL_FORMAT;
	framebuf.width = DISPLAY_WIDTH;
	framebuf.height = DISPLAY_HEIGHT;
	sceDisplaySetFrameBuf(&framebuf, SCE_DISPLAY_SETBUF_NEXTFRAME);

	if (vBlankWait)
		sceDisplayWaitVblankStart();
}

auto	GXMContext::Initialize(Window* win) -> void
{
	SceGxmInitializeParams	initializeParams;
	memset(&initializeParams, 0, sizeof(SceGxmInitializeParams));
	initializeParams.flags = 0;
	initializeParams.displayQueueMaxPendingCount = DISPLAY_MAX_PENDING_SWAPS;
	initializeParams.displayQueueCallback = DisplayCallback;
	initializeParams.displayQueueCallbackDataSize = sizeof(DisplayData);
	initializeParams.parameterBufferSize = SCE_GXM_DEFAULT_PARAMETER_BUFFER_SIZE;

	sceGxmInitialize(&initializeParams);

	vdmRingBuffer = GPUMemoryManager::AllocateCustom(SCE_KERNEL_MEMBLOCK_TYPE_USER_RW_UNCACHE, SCE_GXM_DEFAULT_VDM_RING_BUFFER_SIZE, SCE_GXM_MEMORY_ATTRIB_READ);
	vertexRingBuffer = GPUMemoryManager::AllocateCustom(SCE_KERNEL_MEMBLOCK_TYPE_USER_RW_UNCACHE, SCE_GXM_DEFAULT_VERTEX_RING_BUFFER_SIZE, SCE_GXM_MEMORY_ATTRIB_READ);
	fragmentRingBuffer = GPUMemoryManager::AllocateCustom(SCE_KERNEL_MEMBLOCK_TYPE_USER_RW_UNCACHE, SCE_GXM_DEFAULT_FRAGMENT_RING_BUFFER_SIZE, SCE_GXM_MEMORY_ATTRIB_READ);
	unsigned int fragmentUsseRingBufferOffset;
	fragmentUsseRingBuffer = GPUMemoryManager::AllocateUsse(SCE_GXM_DEFAULT_FRAGMENT_USSE_RING_BUFFER_SIZE, false, &fragmentUsseRingBufferOffset);

	memset(&contextParams, 0, sizeof(SceGxmContextParams));
	contextParams.hostMem = malloc(SCE_GXM_MINIMUM_CONTEXT_HOST_MEM_SIZE);
	contextParams.hostMemSize = SCE_GXM_MINIMUM_CONTEXT_HOST_MEM_SIZE;
	contextParams.vdmRingBufferMem = vdmRingBuffer->GetVoidAdress();
	contextParams.vdmRingBufferMemSize = vdmRingBuffer->GetMemorySize();
	contextParams.vertexRingBufferMem = vertexRingBuffer->GetVoidAdress();
	contextParams.vertexRingBufferMemSize = vertexRingBuffer->GetMemorySize();
	contextParams.fragmentRingBufferMem = fragmentRingBuffer->GetVoidAdress();
	contextParams.fragmentRingBufferMemSize = fragmentRingBuffer->GetMemorySize();
	contextParams.fragmentUsseRingBufferMem = fragmentUsseRingBuffer->GetVoidAdress();
	contextParams.fragmentUsseRingBufferMemSize = fragmentUsseRingBuffer->GetMemorySize();
	contextParams.fragmentUsseRingBufferOffset = fragmentUsseRingBufferOffset;
	sceGxmCreateContext(&contextParams, &context);

	SceGxmRenderTargetParams renderTargetParams;
	memset(&renderTargetParams, 0, sizeof(SceGxmRenderTargetParams));
	renderTargetParams.flags = 0;
	renderTargetParams.width = DISPLAY_WIDTH;
	renderTargetParams.height = DISPLAY_HEIGHT;
	renderTargetParams.scenesPerFrame = 1;
	renderTargetParams.multisampleMode = SCE_GXM_MULTISAMPLE_NONE;
	renderTargetParams.multisampleLocations = 0;
	renderTargetParams.driverMemBlock = -1;

	sceGxmCreateRenderTarget(&renderTargetParams, &renderTarget);

	for (int pos = 0; pos < DISPLAY_BUFFER_COUNT; pos++)
	{
		buffersData[pos] = GPUMemoryManager::Allocate<unsigned int>(DISPLAY_STRIDE_IN_PIXELS * DISPLAY_HEIGHT, SCE_KERNEL_MEMBLOCK_TYPE_USER_CDRAM_RW, SCE_GXM_MEMORY_ATTRIB_RW);
		for (auto it = buffersData[pos]->GetIterator(); it.IsValid(); it.MoveNext())
			*it = 0xff000000;
		sceGxmColorSurfaceInit(&displaySurfaces[pos], DISPLAY_COLOR_FORMAT, SCE_GXM_COLOR_SURFACE_LINEAR, SCE_GXM_COLOR_SURFACE_SCALE_NONE, 
			SCE_GXM_OUTPUT_REGISTER_SIZE_32BIT, DISPLAY_WIDTH, DISPLAY_HEIGHT, DISPLAY_STRIDE_IN_PIXELS, buffersData[pos]->GetVoidAdress());
		sceGxmSyncObjectCreate(&displayBufferSync[pos]);
	}

	const unsigned int alignedWidth = ALIGN(DISPLAY_WIDTH, SCE_GXM_TILE_SIZEX);
	const unsigned int alignedHeight = ALIGN(DISPLAY_HEIGHT, SCE_GXM_TILE_SIZEY);
	unsigned int sampleCount = alignedWidth * alignedHeight;
	unsigned int depthStrideInSamples = alignedWidth;

	depthBufferData = GPUMemoryManager::AllocateCustom(SCE_KERNEL_MEMBLOCK_TYPE_USER_RW_UNCACHE, 4 * sampleCount, SCE_GXM_MEMORY_ATTRIB_READ);
	sceGxmDepthStencilSurfaceInit(&depthSurface, SCE_GXM_DEPTH_STENCIL_FORMAT_S8D24, 
		SCE_GXM_DEPTH_STENCIL_SURFACE_TILED, depthStrideInSamples, depthBufferData->GetVoidAdress(), nullptr);
}

auto	GXMContext::Shutdown() -> void
{
	sceGxmFinish(context);

	sceGxmDisplayQueueFinish();
	DEL(depthBufferData);
	for (int pos = 0; pos < DISPLAY_BUFFER_COUNT; pos++)
	{
		DEL(buffersData[pos]);
		sceGxmSyncObjectDestroy(displayBufferSync[pos]);
	}
	sceGxmDestroyRenderTarget(renderTarget);

	sceGxmDestroyContext(context);

	DEL(fragmentUsseRingBuffer);
	DEL(fragmentRingBuffer);
	DEL(vertexRingBuffer);
	DEL(vdmRingBuffer);

	sceGxmTerminate();
}

auto	GXMContext::Swap() -> void
{
	sceGxmPadHeartbeat(&(displaySurfaces[backBufferIndex]), displayBufferSync[backBufferIndex]);

	DisplayData	displayData;
	displayData.Address = buffersData[backBufferIndex]->GetVoidAdress();
	sceGxmDisplayQueueAddEntry(displayBufferSync[frontBufferIndex], displayBufferSync[backBufferIndex], &displayData);
	frontBufferIndex = backBufferIndex;
	backBufferIndex = (backBufferIndex + 1) % DISPLAY_BUFFER_COUNT;
}