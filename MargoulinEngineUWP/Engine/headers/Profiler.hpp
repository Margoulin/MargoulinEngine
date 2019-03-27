#ifndef __PROFILER_HPP__
#define __PROFILER_HPP__

#include "Service.hpp"
#include <MUtils/String.hpp>

#include <map>

struct MemoryPtrEntry
{
	MemoryPtrEntry(const char* filename, int lineNumb, size_t ptrSize)
		: Filename(filename)
	{
		LineNumber = lineNumb;
		PointerSize = ptrSize;
	}

	MString	Filename;
	int		LineNumber = 0;
	size_t	PointerSize = 0;

	MString GetEntryLine()
	{
		MString ret;
		ret += "Pointer of size : ";
		ret += MString::FromInt(PointerSize);
		ret += " at line ";
		ret += MString::FromInt(LineNumber);
		ret += " of ";
		ret += Filename;
		return ret;
	}
};

class Profiler : public Service
{
public:
	Profiler() = default;
	Profiler(const Profiler&) = delete;
	Profiler(Profiler&&) = delete;
	~Profiler() = default;

	virtual auto	Initialize() -> void;
	virtual auto	Shutdown() -> void;
	virtual auto	Update() -> void {}

	auto	AddEntry(void* ptr, const char * filename, int lineNumber, size_t ptrSize) -> void;
	auto	RemoveEntry(void* ptr) -> void;

	auto	DumpToFile(MString const& filename) -> void;
	auto	DumpToLog() -> void;

	static auto	GetInstance() -> Profiler* { return instance; }

	auto	operator = (const Profiler&)->Profiler& = delete;
	auto	operator = (Profiler&&)->Profiler& = delete;

protected:

private:
	static Profiler*	instance;

	std::map<void*, MemoryPtrEntry>	entries;

#ifdef _PROFILING_ENABLED_
	char	dumpFilename[50];
	bool	dumpToLogAtShutdown = false;
	bool	dumpToFileAtShutdown = false;
#endif // _PROFILING_ENABLED_

#ifdef _DEBUG
public:
	virtual auto	ImGuiUpdate() -> void;
#endif
};

#endif /*__PROFILER_HPP__*/