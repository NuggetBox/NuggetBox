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

		static void SetTimeScale(float aTimeScale);
		static float GetTimeScale();
		static void ResetTimeScale();

	private:
		static std::chrono::time_point<std::chrono::high_resolution_clock> ourStartTime;
		static std::chrono::duration<double> ourTotalTime;
		static std::chrono::duration<float> ourDeltaTime;

		static inline float ourTimeScale;
	};
}