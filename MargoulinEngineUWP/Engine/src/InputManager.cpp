#include "InputManager.hpp"

#include "imgui_impl_dx11.h"
#include "Engine.hpp"
#include "GraphicalLibrary.hpp"
#include "RendererPipeline.hpp"
#include "Context.hpp"
#include "Gamepad.hpp"

auto	InputManager::Update() -> void
{
	for (unsigned int pos = 0; pos < gamepadCount; pos++)
		gamepads[pos]->UpdateValues();

#if defined(_XBOX_ONE) && defined(_DEBUG)
	if (gamepadCount > 0)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.MouseDown[0] = gamepads[0]->GetCurrentState().aButton;

		imGuiCursor.x += (float)gamepads[0]->GetCurrentState().leftStickXAxis * 2.0f;
		imGuiCursor.y -= (float)gamepads[0]->GetCurrentState().leftStickYAxis * 2.0f;

		if (imGuiCursor.x < 0.0f)
			imGuiCursor.x = 0.0f;
		else if (imGuiCursor.x > 1920.0f)
			imGuiCursor.x = 1920.0f;
		if (imGuiCursor.y < 0.0f)
			imGuiCursor = 0.0f;
		else if (imGuiCursor.y > 1080.0f)
			imGuiCursor = 1080.0f;
		io.MousePos.x = imGuiCursor.x * 2.0f;
		io.MousePos.y = imGuiCursor.y * 2.0f;
	}
#endif
}

auto	InputManager::AddGamepad(Gamepad* pad) -> void
{
	gamepads[gamepadCount] = pad;
	gamepadCount++;
}

auto	InputManager::RemoveGamepad(Gamepad* pad) -> bool
{
	for (unsigned int pos = 0; pos < gamepadCount; pos++)
	{
		if (pad == gamepads[pos])
		{
			gamepadCount--;
			gamepads[pos] = nullptr;
			DEL(pad);
			return true;
		}
	}
	return false;
}

#ifdef UWP

auto	InputManager::OnLeftClick(bool pressed) -> void
{
	ImGuiIO&	io = ImGui::GetIO();
	io.MouseDown[0] = pressed;
}

auto	InputManager::OnRightClick(bool pressed) -> void
{
	ImGuiIO&	io = ImGui::GetIO();
	io.MouseDown[1] = pressed;
}

auto	InputManager::OnMiddleClick(bool pressed) -> void
{
	ImGuiIO&	io = ImGui::GetIO();
	io.MouseDown[2] = pressed;
}

auto	InputManager::OnPointerMoved(float x, float y) -> void
{
	ImGuiIO&	io = ImGui::GetIO();
	io.MousePos.x = x;
	io.MousePos.y = y;
}

auto	InputManager::OnKeyDown(unsigned int value) -> void
{
	ImGuiIO&	io = ImGui::GetIO();
	if (value < 256)
		io.KeysDown[value] = 1;
}

#else

auto	InputManager::MessageHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) -> LRESULT
{
	ImGuiIO& io = ImGui::GetIO();
	switch (msg)
	{
	case WM_LBUTTONDOWN:
		io.MouseDown[0] = true;
		return true;
	case WM_LBUTTONUP:
		io.MouseDown[0] = false;
		return true;
	case WM_RBUTTONDOWN:
		io.MouseDown[1] = true;
		return true;
	case WM_RBUTTONUP:
		io.MouseDown[1] = false;
		return true;
	case WM_MBUTTONDOWN:
		io.MouseDown[2] = true;
		return true;
	case WM_MBUTTONUP:
		io.MouseDown[2] = false;
		return true;
	case WM_MOUSEWHEEL:
		io.MouseWheel += GET_WHEEL_DELTA_WPARAM(wParam) > 0 ? +1.0f : -1.0f;
		return true;
	case WM_MOUSEMOVE:
	{
		io.MousePos.x = (signed short)(lParam);
		io.MousePos.y = (signed short)(lParam >> 16);
		return true;
	}
	case WM_KEYDOWN:
		if (wParam < 256)
			io.KeysDown[wParam] = 1;
		return true;
	case WM_KEYUP:
		if (wParam < 256)
			io.KeysDown[wParam] = 0;
		return true;
	case WM_CHAR:
		// You can also use ToAscii()+GetKeyboardState() to retrieve characters.
		if (wParam > 0 && wParam < 0x10000)
			io.AddInputCharacter((unsigned short)wParam);
		if (wParam == VK_ESCAPE)
			PostQuitMessage(0);
		return true;
	case WM_SIZE:
	{
		RECT	rect;
		GetClientRect(hWnd, &rect);
		GraphicalLibrary* lib = Engine::GetInstance()->GetService<GraphicalLibrary>("Renderer");
		lib->GetContext()->Resize(Vector2F((float)(rect.right - rect.left), (float)(rect.bottom - rect.top)));
		//io.DisplaySize = ImVec2((float)(rect.right - rect.left), (float)(rect.bottom - rect.top));
		//GraphicalLibrary* lib = (GraphicalLibrary*)(Engine::GetInstance()->GetService("Renderer"));
		return true;
	}
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

#endif

#include <imgui.h>
#ifdef _DEBUG

auto	InputManager::ImGuiUpdate() -> void
{
	ImGui::Value("Gamepads count", gamepadCount);
	if (ImGui::TreeNode("Gamepads"))
	{
		for (unsigned int pos = 0; pos < gamepadCount; pos++)
		{
			MString	temp = "Gamepad " + MString::FromInt(pos);
			if (ImGui::TreeNode(temp.Str()))
			{
				gamepads[pos]->ImGuiUpdate();
				ImGui::TreePop();
			}
		}
		ImGui::TreePop();
	}
}

#endif