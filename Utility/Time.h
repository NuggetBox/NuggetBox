#pragma once
#include <chrono>

namespace Utility
{
	class Time
	{
	public:
		Time() = delete;
		~Time() = delete;
		Time(const Time& aTimer) = delete;
		Time& operator=(const Time& aTimer) = delete;

		static void Start();
		static void Update();
		static float GetDeltaTime();
		static double GetTotalTime();

	private:
		static std::chrono::time_point<std::chrono::high_resolution_clock> myStartTime;
		static std::chrono::duration<double> myTotalTime;
		static std::chrono::duration<float> myDeltaTime;
	};
}