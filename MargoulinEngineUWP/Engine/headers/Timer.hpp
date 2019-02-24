#ifndef __TIMER_HPP__
#define __TIMER_HPP__

#include <Windows.h>

class Timer
{
public:
	void Start();
	void Stop();

	float	GetDuration() const;

private:
	LARGE_INTEGER	frequency;
	LARGE_INTEGER	startTime;
	LARGE_INTEGER	stopTime;
};

#endif /*__TIMER_HPP__*/