#pragma once
#include <chrono>

class Timer
{
public:
	Timer();
	~Timer() = default;

	float Mark();
	float Peek();
private:
	std::chrono::high_resolution_clock::time_point lastTimePoint;
};

