#ifndef __D3D11CONTEXT_HPP__
#define __D3D11CONTEXT_HPP__

#include "Context.hpp"

#include <wrl/client.h>
#include <d3d11.h>
#include <d2d1_3.h>

using namespace Microsoft::WRL;

class D3D11Context final : public Context
{
public:
	D3D11Context() = default;
	D3D11Context(const D3D11Context&) = delete;
	D3D11Context(D3D11Context&&) = delete;
	~D3D11Context() = default;

	virtual auto	Initialize(Window* win) -> void;
	virtual auto	Shutdown() -> void override;
	
	virtual auto	Swap() -> void { swapChain.Get()->Present(0, 0); }
	virtual auto	Resize(Vector2F const& size) -> void;

#ifdef UWP 
	auto	Begin() -> void;
	auto	SetWindow() -> void;
#endif

	auto	GetDevice() const -> ID3D11Device* { return device.Get(); }
	auto	GetDeviceContext() const -> ID3D11DeviceContext* { return deviceContext.Get(); }
	auto	GetSwapChain() const -> IDXGISwapChain* { return swapChain.Get(); }
	auto	GetRenderTarget() const -> ID3D11RenderTargetView* { return mainRenderTargetView.Get(); }
	auto	GetDepthStencilView() const -> ID3D11DepthStencilView* { return depthStencilView.Get(); }
	auto	GetRenderTargetSize() const -> Vector2F const& { return renderTargetSize; }

	auto	operator = (const D3D11Context&)->D3D11Context& = delete;
	auto	operator = (D3D11Context&&)->D3D11Context& = delete;

	ComPtr<ID2D1Factory3>			d2dFactory;
	ComPtr<ID2D1Device2>			d2dDevice;
	ComPtr<ID2D1DeviceContext2>		d2dContext;
	ComPtr<ID2D1Bitmap1>			d2dTargetBitmap;
	ComPtr<ID2D1DrawingStateBlock1> stateBlock;

protected:

private:
#ifdef UWP
	auto	createDevice() -> void;
	auto	createSwapChain() -> void;
#endif // UWP
	auto	unsetRenderTarget() -> void;
	auto	resizeBuffers() -> void;
	auto	createRenderTarget() -> void;

	ComPtr<ID3D11Device>			device = nullptr;
	ComPtr<ID3D11DeviceContext>		deviceContext = nullptr;
	ComPtr<IDXGISwapChain>			swapChain = nullptr;
	ComPtr<ID3D11RenderTargetView>	mainRenderTargetView = nullptr;
	ComPtr<ID3D11DepthStencilView>	depthStencilView = nullptr;
	Vector2F						renderTargetSize;
	D3D_FEATURE_LEVEL				featureLevel = D3D_FEATURE_LEVEL_10_0;

	
#ifdef _DEBUG
	ID3D11Debug*	debugInterface = nullptr;
#endif // _DEBUG


};


#endif /*__D3D11CONTEXT_HPP__*/