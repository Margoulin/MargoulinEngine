#ifndef __CLOCK_HPP__
#define __CLOCK_HPP__

#ifndef VITA
#include <Windows.h>
#else
#include <psp2/rtc.h>
#endif

class Clock
{
public:
	Clock();
	Clock(const Clock&) = delete;
	Clock(Clock&&) = delete;
	~Clock() = default;

	auto	SetFramerateLimit(unsigned int frames) -> void;
	auto	UnsetFramerateLimit() -> void;

	auto	CanUpdate() -> bool;

	auto	GetDeltaTime() const -> float { return deltaTime; }
#ifdef _DEBUG
	auto	GetMinFrametimeRange() const -> float { return minFrametimeRange; }
	auto	GetMaxFrametimeRange() const -> float { return maxFrametimeRange; }
#endif

	auto	operator = (const Clock&)->Clock& = delete;
	auto	operator = (Clock&&)->Clock& = delete;

protected:

private:
#ifndef VITA
	LARGE_INTEGER	previousTime;
	LARGE_INTEGER	frequency;
#else
	float		frequency;
	SceRtcTick	previousTime;
#endif
	unsigned int	framerateLimit = 0;
	float			deltaTime = 0.0f;
	float			tempDeltaTime = 0.0f;
	float			framerateTime = 0.0f;

#ifdef _DEBUG
	float	maxFrametimeRange = 0.0f;
	float	minFrametimeRange = 0.0f;
#endif
};


#endif /*__CLOCK_HPP__*/