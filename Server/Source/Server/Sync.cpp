#include "ServerPCH.h"
#include "Sync.h"

#include "Time.h"

namespace esc {

	static constexpr int64 s_NanosInSecond = 1000 * 1000 * 1000;

	void Sync::Init()
	{
		s_SleepDurations = new RunningAvg(10);
		s_YieldDurations = new RunningAvg(10);

		s_SleepDurations->Init(static_cast<int64>(1000 * 1000));
		s_YieldDurations->Init(static_cast<int64>(-(GetTime() - GetTime()) * 1.333));

		s_NextFrame = GetTime();

		s_Initialized = true;
	}

	void Sync::Shutdown()
	{
		delete s_SleepDurations;
		delete s_YieldDurations;

		s_Initialized = false;
	}

	void Sync::Tick(int32 fps)
	{
		if (!s_Initialized || fps <= 0)
			return;
		
		for (int64 t0 = GetTime(), t1 = 0; (s_NextFrame - t0) > s_SleepDurations->Avg(); t0 = t1)
		{
			using namespace std::chrono_literals;
			std::this_thread::sleep_for(1ms);
			s_SleepDurations->Add((t1 = GetTime()) - t0);
		}

		s_SleepDurations->DampenForLowResTicker();

		for (int64 t0 = GetTime(), t1 = 0; (s_NextFrame - t0) > s_YieldDurations->Avg(); t0 = t1)
		{
			std::this_thread::yield();
			s_YieldDurations->Add((t1 = GetTime()) - t0);
		}

		s_NextFrame = Max(s_NextFrame + s_NanosInSecond / fps, GetTime());
	}

	int64 Sync::GetTime()
	{
		return (Time::GetSysTime() * s_NanosInSecond) / Time::GetFrequency();
	}

}