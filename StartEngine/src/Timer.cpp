#include "Timer.h"

using namespace std::chrono;

Timer::Timer()
{
	lastTimePoint = std::chrono::high_resolution_clock::now();
}

float Timer::Mark()
{
	const auto old = lastTimePoint;
	lastTimePoint = high_resolution_clock::now();
	const duration<float> elapsedTime = lastTimePoint - old;
	return elapsedTime.count();
}

float Timer::Peek()
{
	return duration<float>(high_resolution_clock::now() - lastTimePoint).count();
}
