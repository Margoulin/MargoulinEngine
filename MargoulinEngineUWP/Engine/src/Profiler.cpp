#include "Profiler.hpp"

#include <fstream>
#include "Logger.hpp"

Profiler* Profiler::instance = nullptr;

auto	Profiler::Initialize() -> void
{
	instance = this;
	shutdownOrderIndex = 0;
	Service::Initialize();
}

auto	Profiler::Shutdown() -> void
{
#ifdef _PROFILING_ENABLED_
	if (dumpToLogAtShutdown)
		DumpToLog();
	if (dumpToFileAtShutdown)
		DumpToFile(dumpFilename);
#endif // _PROFILING_ENABLED

	instance = nullptr;
}

auto	Profiler::AddEntry(void* ptr, const char * filename, int lineNumber, size_t ptrSize) -> void
{
	MemoryPtrEntry	entry(filename, lineNumber, ptrSize);
	entries.insert(std::make_pair(ptr, entry));
}
	
auto	Profiler::RemoveEntry(void* ptr) -> void
{ 
	auto it = entries.find(ptr);
	if (it != entries.end())
		entries.erase(ptr);
}

auto	Profiler::DumpToFile(MString const& filename) -> void
{
	std::ofstream	ofstream(filename.Str());
	for (auto& entry : entries)
		ofstream << entry.second.GetEntryLine().Str() << std::endl;
	ofstream.close();
}

auto	Profiler::DumpToLog() -> void
{
	for (auto& entry : entries)
		Logger::Log(entry.second.GetEntryLine().Str());
}

#ifdef _DEBUG

#include <imgui/imgui.h>

auto	Profiler::ImGuiUpdate() -> void
{
#ifdef _PROFILING_ENABLED_
	ImGui::InputText("Dump filename", dumpFilename, 50);
	if (ImGui::RadioButton("Dump file at shutdown", dumpToFileAtShutdown)) dumpToFileAtShutdown = !dumpToFileAtShutdown;
	if (ImGui::RadioButton("Dump log at shutdown", dumpToLogAtShutdown)) dumpToLogAtShutdown = !dumpToLogAtShutdown;
#endif
}

#endif