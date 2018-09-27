#ifndef __UWPGAMEPAD_HPP__
#define __UWPGAMEPAD_HPP__

#include "Gamepad.hpp"

class UWPGamepad : public Gamepad
{
public:
	UWPGamepad(Windows::Gaming::Input::Gamepad^ value) { gamepad = value; }
	UWPGamepad(const UWPGamepad&) = delete;
	UWPGamepad(UWPGamepad&&) = delete;
	~UWPGamepad() = default;

	auto	UpdateValues() -> void;

	virtual auto	SetLeftRottorValue(double value) -> void;
	virtual auto	SetRightRottorValue(double value) -> void;
	virtual auto	SetLeftTriggerRottorValue(double value) -> void;
	virtual auto	SetRightTriggerRottorValue(double value) -> void;

	auto	CompareGamepadValue(Windows::Gaming::Input::Gamepad^ value) const -> bool { return value == gamepad; }

	auto	operator = (const UWPGamepad&)->UWPGamepad& = delete;
	auto	operator = (UWPGamepad&&)->UWPGamepad& = delete;

protected:

private:
	Windows::Gaming::Input::Gamepad^	gamepad;

};


#endif /*__UWPGAMEPAD_HPP__*/