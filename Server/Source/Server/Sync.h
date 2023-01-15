#pragma once

namespace esc {

	// From: https://github.com/LWJGL/lwjgl/blob/master/src/java/org/lwjgl/opengl/Sync.java

	class RunningAvg
	{
	private:
		static constexpr int64 s_DampenThreshold = 10 * 1000 * 1000;
		static constexpr float s_DampenFactor = 0.9f;

		int64* Slots = nullptr;
		int32 SlotCount = 0;
		int32 Offset = 0;
	public:
		RunningAvg(int32 slotCount)
			: SlotCount(slotCount)
		{
			Slots = new int64[slotCount];
		}

		~RunningAvg()
		{
			delete[] Slots;
		}

		void Init(int64 value)
		{
			while (Offset < SlotCount)
				Slots[Offset++] = value;
		}

		void Add(int64 value)
		{
			Slots[Offset++ % SlotCount] = value;
			Offset %= SlotCount;
		}

		int64 Avg() const
		{
			int64 result = 0;
			for (int32 i = 0; i < SlotCount; i++)
				result += Slots[i];
			return result / SlotCount;
		}

		void DampenForLowResTicker()
		{
			if (Avg() > s_DampenThreshold)
			{
				for (int32 i = 0; i < SlotCount; i++)
					Slots[i] *= s_DampenFactor;
			}
		}
	};

	class Sync
	{
	public:
		static void Init();
		static void Shutdown();

		static void Tick(int32 fps);
	private:
		static int64 GetTime();

		template<typename T>
		static T Max(T x, T y) { return (x < y) ? y : x; }
	private:
		inline static RunningAvg* s_SleepDurations = nullptr;
		inline static RunningAvg* s_YieldDurations = nullptr;

		inline static int64 s_NextFrame = 0;
		inline static bool s_Initialized = false;
	};

}