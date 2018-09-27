#include "Logger.hpp"

#include "Engine.hpp"
#include <Windows.h>

std::vector<std::string> Logger::logs = std::vector<std::string>();

auto	Logger::Log(std::string value) -> void
{
	value += '\n';
	logs.push_back(value);
	Logger* logger = Engine::GetInstance()->GetService<Logger>("Logger");
	if (!logger)
	{
		std::wstring	tempWstring(value.begin(), value.end());
		OutputDebugStringW(tempWstring.c_str());
	}
	if (logs.size() > 100)
		logs.erase(logs.begin());
}

#include <imgui.h>
#ifdef _DEBUG
auto	Logger::ImGuiUpdate() -> void
{
	ImGui::BeginChild("Logs", ImVec2(0, -ImGui::GetItemsLineHeightWithSpacing()), false, ImGuiWindowFlags_HorizontalScrollbar);
	for (auto& log : logs)
		ImGui::Text(log.c_str());
	ImGui::EndChild();
}
#endif