#pragma once

#include "Core/PrimitiveTypes.h"

#include <Windows.h>

namespace esc {

	class Time
	{
	private:
		inline static int64 s_Frequency = 0;
		inline static int64 s_Offset = 0;
	public:
		static void Init()
		{
			QueryPerformanceFrequency((LARGE_INTEGER*)&s_Frequency);

			int64 timerValue = 0;
			QueryPerformanceCounter((LARGE_INTEGER*)&timerValue);
			s_Offset = timerValue;
		}

		static float Get()
		{
			int64 timerValue = 0;
			QueryPerformanceCounter((LARGE_INTEGER*)&timerValue);
			return static_cast<float>(timerValue - s_Offset) / s_Frequency;
		}

		static int64 GetSysTime()
		{
			int64 timerValue = 0;
			QueryPerformanceCounter((LARGE_INTEGER*)&timerValue);
			return timerValue;
		}

		static int64 GetFrequency()
		{
			return s_Frequency;
		}
	};

}