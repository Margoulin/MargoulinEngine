#ifndef __MEMORY_MACRO_HPP__
#define __MEMORY_MACRO_HPP__

#include <stdlib.h>
#include "Profiler.hpp"
#include <new>

#ifdef _PROFILING_ENABLED_

inline void* operator new(size_t size, const char* file, int line) 
{
	void* ptr = malloc(size);
	Profiler* profiler = Profiler::GetInstance();
	if (profiler && profiler->IsInitialize())
		profiler->AddEntry(ptr, file, line, size);
	return ptr;
}

inline void operator delete(void* p) 
{
	Profiler* profiler = Profiler::GetInstance();
	if (profiler && profiler->IsInitialize())
		profiler->RemoveEntry(p);
	free(p); 
}

#define NEW new(__FILE__, __LINE__)

#else

#define NEW new

#endif

#define DEL(ptr) delete(ptr)
#define DELARRAY(ptr) delete[](ptr)

#endif /*__MEMORY_MACRO_HPP__*/