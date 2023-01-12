#pragma once

#include "Core/PrimitiveTypes.h"

#include <Windows.h>

namespace esc {

	class Time
	{
	private:
		inline static uint64 s_Frequency = 0;
		inline static uint64 s_Offset = 0;
	public:
		static void Init()
		{
			QueryPerformanceFrequency((LARGE_INTEGER*)&s_Frequency);

			uint64 timerValue = 0;
			QueryPerformanceCounter((LARGE_INTEGER*)&timerValue);
			s_Offset = timerValue;
		}

		static float Get()
		{
			uint64 timerValue = 0;
			QueryPerformanceCounter((LARGE_INTEGER*)&timerValue);
			return (float)(timerValue - s_Offset) / s_Frequency;
		}
	};

}