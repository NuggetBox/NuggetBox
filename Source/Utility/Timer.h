#pragma once
#include <chrono>

namespace Utility
{
	class Timer
	{
	public:
		Timer() = delete;
		~Timer() = delete;
		Timer(const Timer& aTimer) = delete;
		Timer& operator=(const Timer& aTimer) = delete;

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