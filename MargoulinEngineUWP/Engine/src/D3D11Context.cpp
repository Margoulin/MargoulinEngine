#include "D3D11Context.hpp"

#include "Window.hpp"
#include "DirectXHelper.hpp"
#include <dxgi1_4.h>
#include <d3d11_3.h>
#include <imgui.h>

static const D3D_FEATURE_LEVEL featureLevelArray[] = { D3D_FEATURE_LEVEL_11_0
, D3D_FEATURE_LEVEL_10_1
, D3D_FEATURE_LEVEL_10_0
, D3D_FEATURE_LEVEL_9_3 };
static int g_FeatureLevelIndex = 0;
static const char* g_VsTarget[] = { "vs_5_0", "vs_4_1", "vs_4_0", "vs_3_0" };
static const char* g_PsTarget[] = { "ps_5_0", "ps_4_1", "ps_4_0", "ps_3_0" };

auto	D3D11Context::Initialize(Window* window) -> void
{
	Context::Initialize(window);
	renderTargetSize = window->GetSize();
#ifndef UWP 
	HRESULT hr = S_OK;

	UINT createDeviceFlag = D3D11_CREATE_DEVICE_BGRA_SUPPORT;
	D2D1_FACTORY_OPTIONS options;
	ZeroMemory(&options, sizeof(D2D1_FACTORY_OPTIONS));

#ifdef _DEBUG
	createDeviceFlag |= D3D11_CREATE_DEVICE_DEBUG;
	options.debugLevel = D2D1_DEBUG_LEVEL_INFORMATION;
#endif

	DXGI_SWAP_CHAIN_DESC sd = { 0 };
	sd.Windowed = TRUE;
	sd.OutputWindow = window->GetWindow();
	sd.BufferCount = 1;
	sd.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	sd.SampleDesc.Count = 1;

	D3D_FEATURE_LEVEL featureLevel;
	hr = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE
		, nullptr, createDeviceFlag, featureLevelArray
		, _countof(featureLevelArray), D3D11_SDK_VERSION
		, &sd, swapChain.GetAddressOf(), device.GetAddressOf(), &featureLevel
		, deviceContext.GetAddressOf());
	if FAILED(hr)
		return;

	D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED,
		__uuidof(ID2D1Factory3), &options, &d2dFactory);

	ComPtr<IDXGIDevice3> dxgiDevice;
	device.As(&dxgiDevice);

	d2dFactory->CreateDevice(dxgiDevice.Get(), &d2dDevice);

	d2dDevice->CreateDeviceContext(D2D1_DEVICE_CONTEXT_OPTIONS_NONE,
		&d2dContext);

	d2dFactory->CreateDrawingStateBlock(&stateBlock);

	D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED,
		__uuidof(ID2D1Factory3), &options, &d2dFactory);

	d2dFactory->CreateDevice(dxgiDevice.Get(), &d2dDevice);

	d2dDevice->CreateDeviceContext(D2D1_DEVICE_CONTEXT_OPTIONS_NONE,
		&d2dContext);

	d2dFactory->CreateDrawingStateBlock(&stateBlock);

	createRenderTarget();

#ifdef _DEBUG
	device.Get()->QueryInterface(__uuidof(ID3D11Debug), (void**)(&debugInterface));
#endif // _DEBUG
#else
	createDevice();
#endif
}

auto	D3D11Context::Shutdown() -> void
{
	if (stateBlock)
		stateBlock.Get()->Release();
	if (d2dTargetBitmap)
		d2dTargetBitmap.Get()->Release();
	if (d2dContext)
		d2dContext.Get()->Release();
	if (d2dDevice)
		d2dDevice.Get()->Release();
	if (d2dFactory)
		d2dFactory.Get()->Release();
	if (deviceContext)
		deviceContext->ClearState();
	if (mainRenderTargetView)
		mainRenderTargetView.Get()->Release();
	if (depthStencilView)
		depthStencilView.Get()->Release();
	if (swapChain)
		swapChain.Get()->Release();
	if (deviceContext)
		deviceContext.Get()->Release();
	if (device)
		device.Get()->Release();

#ifdef _DEBUG
	debugInterface->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL);
	SAFE_RELEASE(debugInterface);
#endif // _DEBUG

}

auto	D3D11Context::Resize(Vector2F const& size) -> void
{
	window->SetSize(size);
	renderTargetSize = size;

	unsetRenderTarget();

#ifdef UWP
#ifdef _DEBUG
	ImGuiIO& io = ImGui::GetIO();
	io.DisplaySize = ImVec2(renderTargetSize.x, renderTargetSize.y);
#endif

	if (swapChain)
		resizeBuffers();
	else
		createSwapChain();
#else 
	resizeBuffers();
#endif
	createRenderTarget();
}

#ifdef UWP 

auto	D3D11Context::Begin() -> void
{
	CD3D11_VIEWPORT m_screenViewport = CD3D11_VIEWPORT(
		0.0f,
		0.0f,
		renderTargetSize.x,
		renderTargetSize.y);
	deviceContext->RSSetViewports(1, &m_screenViewport);

	ID3D11RenderTargetView *const targets[1] = { mainRenderTargetView.Get() };
	deviceContext->OMSetRenderTargets(1, targets, depthStencilView.Get());
}

auto	D3D11Context::SetWindow() -> void
{
	renderTargetSize = window->GetSize();
#ifdef _DEBUG
	ImGuiIO& io = ImGui::GetIO();
	io.DisplaySize = ImVec2(renderTargetSize.x, renderTargetSize.y);
#endif

	d2dContext->SetDpi(window->GetDPI(), window->GetDPI());
	unsetRenderTarget();

	if (swapChain)
		resizeBuffers();
	else
		createSwapChain();
	createRenderTarget();
}

auto	D3D11Context::createDevice() -> void
{
	HRESULT hr = S_OK;

	UINT createDeviceFlag = D3D11_CREATE_DEVICE_BGRA_SUPPORT;
	D2D1_FACTORY_OPTIONS options;
	ZeroMemory(&options, sizeof(D2D1_FACTORY_OPTIONS));

#ifdef _DEBUG
	createDeviceFlag |= D3D11_CREATE_DEVICE_DEBUG;
	options.debugLevel = D2D1_DEBUG_LEVEL_INFORMATION;
#endif
	D3D_FEATURE_LEVEL featureLevels[] =
	{
		//D3D_FEATURE_LEVEL_12_1,
		//D3D_FEATURE_LEVEL_12_0,
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
		D3D_FEATURE_LEVEL_9_3,
		D3D_FEATURE_LEVEL_9_2,
		D3D_FEATURE_LEVEL_9_1
	};

	hr = D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, 0,
		createDeviceFlag, featureLevels, ARRAYSIZE(featureLevels),
		D3D11_SDK_VERSION, &device, &featureLevel, &deviceContext);

	D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED,
		__uuidof(ID2D1Factory3), &options, &d2dFactory);

	ComPtr<IDXGIDevice3> dxgiDevice;
	device.As(&dxgiDevice);

	d2dFactory->CreateDevice(dxgiDevice.Get(), &d2dDevice);

	d2dDevice->CreateDeviceContext(D2D1_DEVICE_CONTEXT_OPTIONS_NONE, 
		&d2dContext);

	d2dFactory->CreateDrawingStateBlock(&stateBlock);

#ifdef _DEBUG
	device.Get()->QueryInterface(__uuidof(ID3D11Debug), (void**)(&debugInterface));
#endif // _DEBUG
}

auto	D3D11Context::createSwapChain() -> void
{
	DXGI_SWAP_CHAIN_DESC1 swapChainDesc = { 0 };

	swapChainDesc.Width = renderTargetSize.x;		// Match the size of the window.
	swapChainDesc.Height = renderTargetSize.y;
	swapChainDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;				// This is the most common swap chain format.
	swapChainDesc.Stereo = false;
	swapChainDesc.SampleDesc.Count = 1;								// Don't use multi-sampling.
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.BufferCount = 2;									// Use double-buffering to minimize latency.
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;	// All Windows Store apps must use this SwapEffect.
	swapChainDesc.Flags = 0;
	swapChainDesc.Scaling = DXGI_SCALING_NONE;
	swapChainDesc.AlphaMode = DXGI_ALPHA_MODE_IGNORE;
	
	// This sequence obtains the DXGI factory that was used to create the Direct3D device above.
	ComPtr<IDXGIDevice3> dxgiDevice;
	device.As(&dxgiDevice);

	ComPtr<IDXGIAdapter> dxgiAdapter;
	dxgiDevice->GetAdapter(&dxgiAdapter);

	ComPtr<IDXGIFactory4> dxgiFactory;
	dxgiAdapter->GetParent(IID_PPV_ARGS(&dxgiFactory));
	ComPtr<IDXGISwapChain1> tempSwapChain;
	HRESULT hr = dxgiFactory->CreateSwapChainForCoreWindow(device.Get(), window->GetWindow(), &swapChainDesc, nullptr, &tempSwapChain);
	tempSwapChain.As(&swapChain);

	dxgiDevice->SetMaximumFrameLatency(1);
}

#endif

auto	D3D11Context::unsetRenderTarget() -> void
{
	deviceContext->OMGetRenderTargets(0, nullptr, nullptr);
	if (mainRenderTargetView)
		mainRenderTargetView = nullptr;
	if (depthStencilView)
		depthStencilView = nullptr;
	if (d2dContext)
		d2dContext->SetTarget(nullptr);
	if (d2dTargetBitmap)
		d2dTargetBitmap = nullptr;

	deviceContext->Flush();
}

auto	D3D11Context::resizeBuffers() -> void
{
#ifdef UWP
	swapChain->ResizeBuffers(2, renderTargetSize.x, renderTargetSize.y, DXGI_FORMAT_B8G8R8A8_UNORM, 0);
#else
	swapChain->ResizeBuffers(1, renderTargetSize.x, renderTargetSize.y, DXGI_FORMAT::DXGI_FORMAT_UNKNOWN, 0);
#endif
}

auto	D3D11Context::createRenderTarget() -> void
{
	ComPtr<ID3D11Texture2D1> backBuffer;
	swapChain->GetBuffer(0, IID_PPV_ARGS(&backBuffer));

	device->CreateRenderTargetView(backBuffer.Get(), nullptr,
			&mainRenderTargetView);

	CD3D11_TEXTURE2D_DESC depthStencilDesc(
		DXGI_FORMAT_D24_UNORM_S8_UINT,
		renderTargetSize.x, renderTargetSize.y,
		1, // This depth stencil view has only one texture.
		1, // Use a single mipmap level.
		D3D11_BIND_DEPTH_STENCIL
	);

	ComPtr<ID3D11Texture2D> depthStencil;
	device->CreateTexture2D(
		&depthStencilDesc, nullptr, &depthStencil);

	CD3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc(D3D11_DSV_DIMENSION_TEXTURE2D);
	device->CreateDepthStencilView(
			depthStencil.Get(),
			&depthStencilViewDesc,
			&depthStencilView);

	deviceContext->OMSetRenderTargets(1, mainRenderTargetView.GetAddressOf(),
		depthStencilView.Get());
	// le GetBuffer() incremente le compteur de reference il faut donc faire un Release()

	D3D11_VIEWPORT m_screenViewport = CD3D11_VIEWPORT(
		0.0f, 1.0f, renderTargetSize.x, renderTargetSize.y);

	deviceContext->RSSetViewports(1, &m_screenViewport);

	D2D1_BITMAP_PROPERTIES1 bitmapProperties = D2D1::BitmapProperties1(
		D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CANNOT_DRAW,
		D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED),
		window->GetDPI(), window->GetDPI());

	ComPtr<IDXGISurface2> dxgiBackBuffer;
	swapChain->GetBuffer(0, IID_PPV_ARGS(&dxgiBackBuffer));

	d2dContext->CreateBitmapFromDxgiSurface(dxgiBackBuffer.Get(),
		&bitmapProperties, &d2dTargetBitmap);

	d2dContext->SetTarget(d2dTargetBitmap.Get());
	d2dContext->SetDpi(window->GetDPI(), window->GetDPI());

	d2dContext->SetTextAntialiasMode(D2D1_TEXT_ANTIALIAS_MODE_GRAYSCALE);
}