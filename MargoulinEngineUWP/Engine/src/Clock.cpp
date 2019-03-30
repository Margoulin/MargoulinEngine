#include "Clock.hpp"

#ifdef VITA
#include <psp2/power.h>
#endif

#include "Logger.hpp"

Clock::Clock()
{
#ifndef VITA
	QueryPerformanceFrequency(&frequency);
#else
	frequency = (float)(scePowerGetArmClockFrequency()) * 1000.0f;
#endif
}

auto	Clock::SetFramerateLimit(unsigned int frames) -> void
{
	framerateLimit = frames;
	framerateTime = 1.0f / (float)frames;

#ifdef _DEBUG
	maxFrametimeRange = framerateTime + 0.01f;
	minFrametimeRange = framerateTime - 0.01f;
#endif
}

auto	Clock::UnsetFramerateLimit() -> void
{
	framerateTime = 0.0f;
	framerateLimit = 0;
#ifdef _DEBUG
	maxFrametimeRange = 0.0f;
	minFrametimeRange = 0.0f;
#endif
}

auto	Clock::CanUpdate() -> bool
{
#ifndef VITA
	LARGE_INTEGER	currentTime;
	QueryPerformanceCounter(&currentTime);
	tempDeltaTime += (float)(currentTime.QuadPart - previousTime.QuadPart) / (float)frequency.QuadPart;
	previousTime = currentTime;
	if (tempDeltaTime >= framerateTime)
	{
		deltaTime = tempDeltaTime;
		tempDeltaTime = 0.0f;
		return true;
	}
	return false;
#else
	SceRtcTick currentTime;
	sceRtcGetCurrentTick(&currentTime);

	tempDeltaTime += (float)(currentTime.tick - previousTime.tick) / frequency;
	previousTime = currentTime;

	if (tempDeltaTime >= framerateTime)
	{
		deltaTime = tempDeltaTime;
		tempDeltaTime = 0.0;
		return true;
	}
	return false;
#endif // !VITA
}