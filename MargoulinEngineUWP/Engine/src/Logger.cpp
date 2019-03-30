#include "Logger.hpp"

#ifndef VITA
#include <Windows.h>
#else
#include <psp2/io/fcntl.h>
#endif

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
#ifndef VITA
	if (!instance)
		OutputDebugStringW(MWString::FromString(value + '\n').Str());
#else
	SceUID	fileId = sceIoOpen("ux0:data/MargoulinEngine.txt", SCE_O_WRONLY | SCE_O_CREAT, 0777);
	if (fileId < 0)
		return;
	sceIoLseek(fileId, 0, SCE_SEEK_END);
	sceIoWrite(fileId, value.Str(), (SceSize)value.Count());
	char ret = '\n';
	sceIoWrite(fileId, &ret, 1);
	sceIoClose(fileId);
#endif
	if (logs.size() > 100)
		logs.erase(logs.begin());
}

#include <imgui/imgui.h>
#ifdef _DEBUG
auto	Logger::ImGuiUpdate() -> void
{
	ImGui::BeginChild("Logs", ImVec2(0, -ImGui::GetItemsLineHeightWithSpacing()), false, ImGuiWindowFlags_HorizontalScrollbar);
	for (auto& log : logs)
		ImGui::Text(log.Str());
	ImGui::EndChild();
}
#endif