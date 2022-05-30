#pragma once
#include <cassert>

namespace Utility
{
	template <typename T>
	T Max(const T aFirst, const T aSecond)
	{
		if (aFirst > aSecond)
		{
			return aFirst;
		}

		return aSecond;
	}

	template <typename T>
	T Min(const T aFirst, const T aSecond)
	{
		if (aFirst < aSecond)
		{
			return aFirst;
		}

		return aSecond;
	}

	template <typename T>
	T Abs(const T aValue)
	{
		if (aValue < 0)
		{
			return aValue * -1;
		}

		return aValue;
	}

	template <typename T>
	T Clamp(const T aValue, const T aMin, const T aMax)
	{
		assert(aMin <= aMax && "Min was bigger than max");

		if (aValue < aMin)
		{
			return aMin;
		}

		if (aValue > aMax)
		{
			return aMax;
		}

		return aValue;
	}

	template <typename T>
	T Lerp(const T a, const T b, float c)
	{
		if (c > 1)
		{
			c = 1;
		}

		if (c < 0)
		{
			c = 0;
		}

		return a + c * (b - a);
	}

	template <typename T>
	void Swap(T& aFirst, T& aSecond)
	{
		T tempFirst = aFirst;
		aFirst = aSecond;
		aSecond = tempFirst;
	}
}