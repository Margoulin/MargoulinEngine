#ifndef __WINDOW_HPP__
#define __WINDOW_HPP__

#include "CoreMinimal.hpp"

#ifndef VITA
#include <Windows.h>
#include <functional>
#include <wrl/client.h>
#endif
#include <MUtils/Maths/Vector.hpp>

struct IUnknown;

class Window
{
public:
	Window() = default;
	Window(const Window&) = delete;
	Window(Window&&) = delete;
	~Window() = default;

#ifdef WIN32
	static	auto	CreateWindowInstance(WNDPROC wndProc) -> Window*;
	static	std::function<LRESULT(HWND, UINT, WPARAM, LPARAM)>	eventCallback;

	auto	SetWindowTitle(MString const& value) -> void;

	auto	GetWindow() const -> HWND { return winInstance; }
#elif UWP
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
#ifdef WIN32 
	HWND		winInstance;
	WNDCLASS	wc;
	MWString	className;
#elif UWP
	Microsoft::WRL::ComPtr<IUnknown>	window;
#endif
	float			dpi = 256.0f;
	Vector2F		size;

#ifdef _DEBUG
public:
	auto	ImGuiUpdate() -> void;
	char	newWindowNameBuffer[50];
#endif
};


#endif /*__WINDOW_HPP__*/