#include "Timer.h"

namespace Utility
{
	std::chrono::time_point<std::chrono::high_resolution_clock> Timer::ourStartTime;
	std::chrono::duration<double> Timer::ourTotalTime;
	std::chrono::duration<float> Timer::ourDeltaTime;

	void Timer::Start()
	{
		ourStartTime = std::chrono::high_resolution_clock::now();
		ourTotalTime = std::chrono::seconds(0);
		ourDeltaTime = std::chrono::seconds(0);
		ResetTimeScale();
	}

	void Timer::Update()
	{
		const std::chrono::duration<double> totalTimeLastFrame = ourTotalTime;
		ourTotalTime = std::chrono::high_resolution_clock::now() - ourStartTime;
		ourDeltaTime = ourTotalTime - totalTimeLastFrame;
	}

	float Timer::GetDeltaTime()
	{
		return ourDeltaTime.count() * ourTimeScale;
	}

	double Timer::GetTotalTime()
	{
		return ourTotalTime.count();
	}

	void Timer::SetTimeScale(float aTimeScale)
	{
		ourTimeScale = aTimeScale;
	}

	float Timer::GetTimeScale()
	{
		return ourTimeScale;
	}

	void Timer::ResetTimeScale()
	{
		ourTimeScale = 1.0f;
	}
}