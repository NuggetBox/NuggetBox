#include "Timer.h"

namespace Utility
{
	std::chrono::time_point<std::chrono::high_resolution_clock> Timer::myStartTime;
	std::chrono::duration<double> Timer::myTotalTime;
	std::chrono::duration<float> Timer::myDeltaTime;

	void Timer::Start()
	{
		myStartTime = std::chrono::high_resolution_clock::now();
		myTotalTime = std::chrono::seconds(0);
		myDeltaTime = std::chrono::seconds(0);
	}

	void Timer::Update()
	{
		const std::chrono::duration<double> totalTimeLastFrame = myTotalTime;
		myTotalTime = std::chrono::high_resolution_clock::now() - myStartTime;
		myDeltaTime = myTotalTime - totalTimeLastFrame;
	}

	float Timer::GetDeltaTime()
	{
		return myDeltaTime.count();
	}

	double Timer::GetTotalTime()
	{
		return myTotalTime.count();
	}
}