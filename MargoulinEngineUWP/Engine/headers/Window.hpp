#ifndef __WINDOW_HPP__
#define __WINDOW_HPP__

#include <Windows.h>
#include <string>
#include <functional>
#include <wrl/client.h>
#include "Maths/Vector.hpp"

class IUnknown;

class Window
{
public:
	Window() = default;
	Window(const Window&) = delete;
	Window(Window&&) = delete;
	~Window() = default;

#ifndef UWP 
	static	auto	CreateWindowInstance() -> Window*;
	static	std::function<LRESULT(HWND, UINT, WPARAM, LPARAM)>	eventCallback;

	auto	GetWindow() const -> HWND { return winInstance; }
#else
	auto	UpdateHandle(IUnknown* value) -> void { window = value; }
	
	auto	SetDPI(float const value) -> void { dpi = value; }

	auto	GetWindow() const -> IUnknown* { return window.Get(); }
#endif
	auto	Shutdown() -> void;

	auto	SetSize(Vector2F const& value) -> void { size = value; }

	auto	GetDPI() const -> float const& { return dpi; }
	auto	GetSize() const -> Vector2F const& { return size; }

	auto	operator = (const Window&)->Window& = delete;
	auto	operator = (Window&&)->Window& = delete;

protected:

private:
#ifndef UWP 
	HWND			winInstance;
	WNDCLASS		wc;
	std::wstring	className;
#else
	Microsoft::WRL::ComPtr<IUnknown>	window;
#endif
	float			dpi = 256.0f;
	Vector2F		size;

#ifdef _DEBUG
public:
	auto	ImGuiUpdate() -> void;
#endif
};


#endif /*__WINDOW_HPP__*/