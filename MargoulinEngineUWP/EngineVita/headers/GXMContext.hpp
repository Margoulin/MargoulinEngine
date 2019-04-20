#ifndef __GXM_CONTEXT_HPP__
#define __GXM_CONTEXT_HPP__

#include "GPUMemoryManager.hpp"
#include "Context.hpp"

#include <psp2/gxm.h>

#define DISPLAY_WIDTH				960
#define DISPLAY_HEIGHT				544
#define DISPLAY_STRIDE_IN_PIXELS	1024
#define DISPLAY_COLOR_FORMAT		SCE_GXM_COLOR_FORMAT_A8B8G8R8
#define DISPLAY_PIXEL_FORMAT		SCE_DISPLAY_PIXELFORMAT_A8B8G8R8
#define DISPLAY_BUFFER_COUNT		3
#define DISPLAY_MAX_PENDING_SWAPS	2

struct DisplayData
{
	void*	Address = nullptr;
};

class GXMContext final : public Context
{
public:
	GXMContext() = default;
	GXMContext(const GXMContext&) = delete;
	GXMContext(GXMContext&&) = delete;
	~GXMContext() = default;

	virtual auto	Initialize(Window* win) -> void;
	virtual auto	Shutdown() -> void;

	virtual auto	Swap() -> void;
	virtual auto	Resize(Vector2F const&) -> void {} //USELESS BECAUSE OF FIXED SIZE
	
	void	UpdateBufferID() { frontBufferIndex = backBufferIndex; backBufferIndex = (backBufferIndex + 1) % DISPLAY_BUFFER_COUNT; }

	SceGxmContext*		GetContext() const { return context; }
	SceGxmRenderTarget*	GetRenderTarget() const { return renderTarget; }
	SceGxmSyncObject*	GetBackBufferSyncObject() const { return displayBufferSync[backBufferIndex]; }
	SceGxmSyncObject*	GetFrontBufferSyncObject() const { return displayBufferSync[frontBufferIndex]; }
	SceGxmColorSurface*	GetBackBufferColorSurface() { return &(displaySurfaces[backBufferIndex]); }
	SceGxmDepthStencilSurface*	GetDepthStencilSurface() { return &depthSurface; }
	void*						GetBackBufferData() { return buffersData[backBufferIndex]->GetVoidAdress(); }

	auto	operator = (const GXMContext&)->GXMContext& = delete;
	auto	operator = (GXMContext&&)->GXMContext& = delete;

protected:

private:
	SceGxmContextParams		contextParams;

	SceGxmContext*		context = nullptr;
	SceGxmRenderTarget* renderTarget = nullptr;
	
	SceGxmColorSurface			displaySurfaces[DISPLAY_BUFFER_COUNT];
	SceGxmSyncObject*			displayBufferSync[DISPLAY_BUFFER_COUNT];
	GPUAdress<unsigned int>*	buffersData[DISPLAY_BUFFER_COUNT];

	GPUAdressBase*	vdmRingBuffer = nullptr;
	GPUAdressBase*	vertexRingBuffer = nullptr;
	GPUAdressBase*	fragmentRingBuffer = nullptr;
	GPUUsseAdress*	fragmentUsseRingBuffer = nullptr;
	GPUUsseAdress*	vertexUsseRingBuffer = nullptr;

	GPUAdressBase*				depthBufferData = nullptr;
	SceGxmDepthStencilSurface	depthSurface;

	unsigned int backBufferIndex = 0;
	unsigned int frontBufferIndex = 0;
};


#endif /*__GXM_CONTEXT_HPP__*/