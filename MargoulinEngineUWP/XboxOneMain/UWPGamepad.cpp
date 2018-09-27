#include <pch.h>

#include "UWPGamepad.hpp"

auto	UWPGamepad::UpdateValues() -> void
{
	using namespace Windows::Gaming::Input;
	GamepadState	newState;
	auto reading = gamepad->GetCurrentReading();

	newState.leftStickXAxis = reading.LeftThumbstickX;
	newState.leftStickYAxis = reading.LeftThumbstickY;
	newState.rightStickXAxis = reading.RightThumbstickX;
	newState.rightStickYAxis = reading.RightThumbstickY;
	newState.leftTrigger = reading.LeftTrigger;
	newState.rightTrigger = reading.RightTrigger;
	newState.leftBumper = 
		(reading.Buttons & GamepadButtons::LeftShoulder) == GamepadButtons::LeftShoulder;
	newState.rightBumper = 
		(reading.Buttons & GamepadButtons::RightShoulder) == GamepadButtons::RightShoulder;
	newState.dpadUp = 
		(reading.Buttons & GamepadButtons::DPadUp) == GamepadButtons::DPadUp;
	newState.dpadDown = 
		(reading.Buttons & GamepadButtons::DPadDown) == GamepadButtons::DPadDown;
	newState.dpadLeft = 
		(reading.Buttons & GamepadButtons::DPadLeft) == GamepadButtons::DPadLeft;
	newState.dpadRight = 
		(reading.Buttons & GamepadButtons::DPadRight) == GamepadButtons::DPadRight;
	newState.aButton = 
		(reading.Buttons & GamepadButtons::A) == GamepadButtons::A;
	newState.bButton = 
		(reading.Buttons & GamepadButtons::B) == GamepadButtons::B;
	newState.xButton = 
		(reading.Buttons & GamepadButtons::X) == GamepadButtons::X;
	newState.yButton = 
		(reading.Buttons & GamepadButtons::Y) == GamepadButtons::Y;
	newState.startButton = 
		(reading.Buttons & GamepadButtons::Menu) == GamepadButtons::Menu;
	newState.selectButton = 
		(reading.Buttons & GamepadButtons::View) == GamepadButtons::View;
	newState.leftRottor = gamepad->Vibration.LeftMotor;
	newState.rightRottor = gamepad->Vibration.LeftMotor;
	newState.leftTriggerRottor = gamepad->Vibration.LeftTrigger;
	newState.rightTriggerRottor = gamepad->Vibration.RightTrigger;

	previousState = currrentState;
	currrentState = newState;
}

auto	UWPGamepad::SetLeftRottorValue(double value) -> void
{
	using namespace Windows::Gaming::Input;
	GamepadVibration	currentVibration = gamepad->Vibration;
	currentVibration.LeftMotor = value;
	gamepad->Vibration = currentVibration;
}

auto	UWPGamepad::SetRightRottorValue(double value) -> void
{
	using namespace Windows::Gaming::Input;
	GamepadVibration	currentVibration = gamepad->Vibration;
	currentVibration.RightMotor = value;
	gamepad->Vibration = currentVibration;
}

auto	UWPGamepad::SetLeftTriggerRottorValue(double value) -> void
{
	using namespace Windows::Gaming::Input;
	GamepadVibration	currentVibration = gamepad->Vibration;
	currentVibration.LeftTrigger = value;
	gamepad->Vibration = currentVibration;
}

auto	UWPGamepad::SetRightTriggerRottorValue(double value) -> void
{
	using namespace Windows::Gaming::Input;
	GamepadVibration	currentVibration = gamepad->Vibration;
	currentVibration.RightTrigger = value;
	gamepad->Vibration = currentVibration;
}