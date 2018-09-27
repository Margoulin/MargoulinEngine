#ifndef __GAMEPAD_HPP__
#define __GAMEPAD_HPP__

class Gamepad
{
public:
	struct GamepadState
	{
		double	leftStickXAxis = 0.0f;
		double	leftStickYAxis = 0.0f;
		double	rightStickXAxis = 0.0f;
		double	rightStickYAxis = 0.0f;
		double	leftTrigger = 0.0f;
		double	rightTrigger = 0.0f;
		bool	leftBumper = false;
		bool	rightBumper = false;
		bool	dpadUp = false;
		bool	dpadDown = false;
		bool	dpadLeft = false;
		bool	dpadRight = false;
		bool	aButton = false;
		bool	bButton = false;
		bool	xButton = false;
		bool	yButton = false;
		bool	startButton = false;
		bool	selectButton = false;

		double	leftRottor = 0.0f;
		double	rightRottor = 0.0f;
		double	leftTriggerRottor = 0.0f;
		double	rightTriggerRottor = 0.0f;
	};

	Gamepad() = default;
	Gamepad(const Gamepad&) = delete;
	Gamepad(Gamepad&&) = delete;
	~Gamepad() = default;

	virtual auto	UpdateValues() -> void = 0;
	
	virtual auto	SetLeftRottorValue(double value) -> void = 0;
	virtual auto	SetRightRottorValue(double value) -> void = 0;
	virtual auto	SetLeftTriggerRottorValue(double value) -> void = 0;
	virtual auto	SetRightTriggerRottorValue(double value) -> void = 0;

	auto	GetCurrentState() const -> GamepadState const& { return currrentState; }

	auto	operator = (const Gamepad&)->Gamepad& = delete;
	auto	operator = (Gamepad&&)->Gamepad& = delete;

protected:
	GamepadState	previousState;
	GamepadState	currrentState;

#ifdef _DEBUG
public:
	auto	ImGuiUpdate() -> void;
#endif
};


#endif /*__GAMEPAD_HPP__*/