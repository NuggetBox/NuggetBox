#include "Time.h"

namespace Utility
{
	std::chrono::time_point<std::chrono::high_resolution_clock> Time::myStartTime;
	std::chrono::duration<double> Time::myTotalTime;
	std::chrono::duration<float> Time::myDeltaTime;

	void Time::Start()
	{
		myStartTime = std::chrono::high_resolution_clock::now();
		myTotalTime = std::chrono::seconds(0);
		myDeltaTime = std::chrono::seconds(0);
	}

	void Time::Update()
	{
		const std::chrono::duration<double> totalTimeLastFrame = myTotalTime;
		myTotalTime = std::chrono::high_resolution_clock::now() - myStartTime;
		myDeltaTime = myTotalTime - totalTimeLastFrame;
	}

	float Time::GetDeltaTime()
	{
		return myDeltaTime.count();
	}

	double Time::GetTotalTime()
	{
		return myTotalTime.count();
	}
}