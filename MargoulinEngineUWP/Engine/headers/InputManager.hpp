#ifndef __INPUTMANAGER_HPP__
#define __INPUTMANAGER_HPP__

#include "Service.hpp"

#include <Windows.h>

class Gamepad;

class InputManager : public Service
{
public:
	InputManager() = default;
	InputManager(const InputManager&) = delete;
	InputManager(InputManager&&) = delete;
	~InputManager() = default;

	virtual auto	Initialize() -> void {}
	virtual auto	Shutdown() -> void {}

	auto	Update() -> void;

	auto	AddGamepad(Gamepad* pad) -> void;
	auto	RemoveGamepad(Gamepad* pad) -> bool;

	auto	GetGamepad(unsigned int const& idx) const -> Gamepad* { return (idx < 4) ? gamepads[idx] : nullptr; }
	auto	GetGamepadsCount() const -> unsigned int { return gamepadCount; }

#ifdef UWP
	auto	OnLeftClick(bool pressed) -> void;
	auto	OnRightClick(bool pressed) -> void;
	auto	OnMiddleClick(bool pressed) -> void;
	auto	OnPointerMoved(float x, float y) -> void;
	auto	OnKeyDown(unsigned int value) -> void;
#else
	auto	MessageHandler(HWND, UINT, WPARAM, LPARAM) -> LRESULT;
#endif

	auto	operator = (const InputManager&)->InputManager& = delete;
	auto	operator = (InputManager&&)->InputManager& = delete;

protected:

private:
	Gamepad*		gamepads[4];
	unsigned int	gamepadCount = 0;

#ifdef _DEBUG
	virtual auto	ImGuiUpdate() -> void;
#endif // _DEBUG
};


#endif /*__INPUTMANAGER_HPP__*/