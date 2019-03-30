#ifndef __TIMER_HPP__
#define __TIMER_HPP__

#ifndef VITA
#include <Windows.h>
#else
#include <psp2/rtc.h>
#endif

class Timer
{
public:
	void Start();
	void Stop();

	float	GetDuration() const;

private:
#ifndef VITA
	LARGE_INTEGER	frequency;
	LARGE_INTEGER	startTime;
	LARGE_INTEGER	stopTime;
#else
	int	frequency;
	SceRtcTick	startTime;
	SceRtcTick	stopTime;
#endif
};

#endif /*__TIMER_HPP__*/