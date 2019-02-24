#include "Logger.hpp"

#include <Windows.h>

std::vector<MString> Logger::logs = std::vector<MString>();
Logger*	Logger::instance = nullptr;

auto	Logger::Initialize() -> void
{
	instance = this;
	Service::Initialize();
}

auto	Logger::Log(MString const& value) -> void
{
	logs.push_back(value);
	if (!instance)
		OutputDebugStringW(MWString::FromString(value + '\n').Str());
	if (logs.size() > 100)
		logs.erase(logs.begin());
}

#include <imgui.h>
#ifdef _DEBUG
auto	Logger::ImGuiUpdate() -> void
{
	ImGui::BeginChild("Logs", ImVec2(0, -ImGui::GetItemsLineHeightWithSpacing()), false, ImGuiWindowFlags_HorizontalScrollbar);
	for (auto& log : logs)
		ImGui::Text(log.Str());
	ImGui::EndChild();
}
#endif