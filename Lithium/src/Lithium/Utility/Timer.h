#pragma once

#include "Time.h"
#include <stdint.h>

namespace Li
{
	class Timer
	{
	public:
		Timer(Duration::us delay, bool post_completion_value = false, bool repeat = false);

		bool Update(Duration::us dt);

		bool IsCompleted()
		{
			return m_Completed;
		}

		inline Duration::us GetDelay() const
		{
			return m_Delay;
		}

		inline Duration::us GetElapsed() const
		{
			return m_Elapsed;
		}

		inline float GetFraction() const
		{
			return (float)m_Elapsed.count() / (float)m_Delay.count();
		}

		inline int GetRepeatCount() const
		{
			return m_RepeatCount;
		}

		inline void Reset()
		{
			m_Elapsed = Duration::us(0);
			m_Completed = false;
			m_RepeatCount = 0;
		}

		inline void SetDelay(Duration::us delay)
		{
			m_Delay = delay;
		}

	private:
		Duration::us m_Delay;
		Duration::us m_Elapsed;

		bool m_Repeat;
		int m_RepeatCount;

		bool m_Completed;
		bool m_PostCompletionValue;
	};
}