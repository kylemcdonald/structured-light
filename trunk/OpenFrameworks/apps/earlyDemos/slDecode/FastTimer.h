// High-resolution timer for Windows using QueryPerformanceCounter()

#pragma once

#include "windows.h"
#include "stdio.h"

class FastTimer {
public:
	FastTimer() {
		QueryPerformanceFrequency(&ticksPerSecond);
		start();
		stopTime = startTime;
	}
	void start() {
		QueryPerformanceCounter(&startTime);
	}
	void stop() {
		QueryPerformanceCounter(&stopTime);
	}
	double getSeconds() {
		return (double) elapsed() / ticksPerSecond.QuadPart;
	}
	double getMilliseconds() {
		return ((double) elapsed() * 1000) / ticksPerSecond.QuadPart;
	}
	double getMicroseconds() {
		return ((double) elapsed() * 1000000) / ticksPerSecond.QuadPart;
	}
	double getNanoseconds() {
		return ((double) elapsed() * 1000000000) / ticksPerSecond.QuadPart;
	}
private:
	LARGE_INTEGER ticksPerSecond;
	LARGE_INTEGER startTime, stopTime;

	LONGLONG elapsed() {
		return stopTime.QuadPart - startTime.QuadPart;
	}
};
