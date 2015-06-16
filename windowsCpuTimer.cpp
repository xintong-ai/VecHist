#include "windowsCpuTimer.h"

WindowsCpuTimer::WindowsCpuTimer()
{
	reinit();
}

void WindowsCpuTimer::reinit()
{
	timeElapsed = 0.0f;
	
}

void WindowsCpuTimer::startTimer()
{
	
	QueryPerformanceFrequency(&freqLi);

	pcFreq = double(freqLi.QuadPart)/1000.0;
	QueryPerformanceCounter(&freqLi);
	startTime = freqLi.QuadPart;
	
}

void WindowsCpuTimer::stopTimer()
{
	float stopWatchTime = 0.0f;
	
	///////////////////////////

	//Timing code end
	QueryPerformanceCounter(&freqLi);
	stopWatchTime = double(freqLi.QuadPart-startTime) / pcFreq;


	///////////////////////////

	timeElapsed += stopWatchTime;
}

int WindowsCpuTimer::getTimeElapsed()
{
	return timeElapsed;
}

