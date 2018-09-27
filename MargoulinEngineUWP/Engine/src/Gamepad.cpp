#include "Gamepad.hpp"

#include <imgui.h>
#ifdef _DEBUG
auto	Gamepad::ImGuiUpdate() -> void
{
	ImGui::Value("Left Stick X Axis", (float)currrentState.leftStickXAxis);
	ImGui::Value("Left Stick Y Axis", (float)currrentState.leftStickYAxis);
	ImGui::Value("Right Stick X Axis", (float)currrentState.rightStickXAxis);
	ImGui::Value("Right Stick Y Axis", (float)currrentState.rightStickYAxis);
	ImGui::Value("Left Trigger", (float)currrentState.leftTrigger);
	ImGui::Value("Right Trigger", (float)currrentState.rightTrigger);
	ImGui::Value("Left Bumper", currrentState.leftBumper);
	ImGui::Value("Right Bumper", currrentState.rightBumper);
	ImGui::Value("Dpad Up", currrentState.dpadUp);
	ImGui::Value("Dpad Down", currrentState.dpadDown);
	ImGui::Value("Dpad Left", currrentState.dpadLeft);
	ImGui::Value("Dpad Right", currrentState.dpadRight);
	ImGui::Value("A", currrentState.aButton);
	ImGui::Value("B", currrentState.bButton);
	ImGui::Value("X", currrentState.xButton);
	ImGui::Value("Y", currrentState.yButton);
	ImGui::Value("Start Button", currrentState.startButton);
	ImGui::Value("Select Button", currrentState.selectButton);
	ImGui::Value("Left Rottor", (float)currrentState.leftRottor);
	ImGui::Value("Right Rottor", (float)currrentState.rightRottor);
	ImGui::Value("Left Trigger Rottor", (float)currrentState.leftTriggerRottor);
	ImGui::Value("Right Trigger Rottor", (float)currrentState.rightTriggerRottor);
}
#endif