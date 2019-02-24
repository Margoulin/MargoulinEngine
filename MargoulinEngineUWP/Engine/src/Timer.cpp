#include "Timer.hpp"

void Timer::Start()
{
	QueryPerformanceFrequency(&frequency);
	QueryPerformanceCounter(&startTime);
}

void Timer::Stop()
{
	QueryPerformanceCounter(&stopTime);
}

float	Timer::GetDuration() const
{
	return (float)(stopTime.QuadPart - startTime.QuadPart) / (float)frequency.QuadPart;
}