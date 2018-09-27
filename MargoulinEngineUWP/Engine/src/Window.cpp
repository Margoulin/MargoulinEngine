#include "Window.hpp"

#ifndef UWP 

std::function<LRESULT(HWND, UINT, WPARAM, LPARAM)>	Window::eventCallback = nullptr;

std::wstring	s2ws(std::string const& value)
{
	int len;
	int slength = (int)value.length() + 1;
	len = MultiByteToWideChar(CP_ACP, 0, value.c_str(), slength, 0, 0);
	wchar_t* buf = new wchar_t[len];
	MultiByteToWideChar(CP_ACP, 0, value.c_str(), slength, buf, len);
	std::wstring r(buf);
	delete[] buf;
	return r;
}

LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	
	if (Window::eventCallback)
		return Window::eventCallback(hWnd, msg, wParam, lParam);
	switch (msg)
	{
		case WM_DESTROY:
			PostQuitMessage(0);
			return 0;
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

auto	Window::CreateWindowInstance() -> Window*
{
	Window* window = new Window();
	window->className = s2ws("Rémi Androuin");
	std::wstring	windowName = s2ws("DX11");
	window->wc = { CS_CLASSDC, WndProc, 0, 0, GetModuleHandle(nullptr), nullptr, LoadCursor(nullptr, IDC_ARROW), nullptr, nullptr, window->className.c_str() };
	RegisterClass(&(window->wc));

	const int app_width = 1280;
	const int app_height = 720;

	window->size = Vector2F(app_width, app_height);

	DWORD style = WS_OVERLAPPEDWINDOW;
	RECT canvasRect = { 0, 0, app_width, app_height };
	AdjustWindowRect(&canvasRect, style, FALSE);

	window->winInstance = CreateWindow(window->className.c_str(), windowName.c_str(), style, (GetSystemMetrics(SM_CXSCREEN) - app_width) / 2, (GetSystemMetrics(SM_CYSCREEN) - app_height) / 2
		, canvasRect.right - canvasRect.left, canvasRect.bottom - canvasRect.top, nullptr, nullptr, window->wc.hInstance, nullptr);

	if (window->winInstance == nullptr)
	{
		window->Shutdown();
		return nullptr;
	}
	return window;
}

#endif

auto	Window::Shutdown() -> void
{
#ifndef UWP
	UnregisterClass(className.c_str(), wc.hInstance);
#endif
}

#include <imgui.h>
#ifdef _DEBUG
auto	Window::ImGuiUpdate() -> void
{
	ImGui::InputFloat2("Size", &size.x, -1, ImGuiInputTextFlags_ReadOnly);
}
#endif // _DEBUG
