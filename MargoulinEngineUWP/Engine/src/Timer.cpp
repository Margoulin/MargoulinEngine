#include "Timer.hpp"

#ifdef VITA
#include <psp2/types.h>
#include <psp2/power.h>
#endif

void Timer::Start()
{
#ifndef VITA
	QueryPerformanceFrequency(&frequency);
	QueryPerformanceCounter(&startTime);
#else
	frequency = scePowerGetArmClockFrequency();
	sceRtcGetCurrentTick(&startTime);
#endif
}

void Timer::Stop()
{
#ifndef VITA
	QueryPerformanceCounter(&stopTime);
#else
	sceRtcGetCurrentTick(&stopTime);
#endif
}

float	Timer::GetDuration() const
{
#ifndef VITA
	return (float)(stopTime.QuadPart - startTime.QuadPart) / (float)frequency.QuadPart;
#else
	return (float)(stopTime.tick - startTime.tick) / (float)frequency;
#endif
}