module;
#include <chrono>
export module nuit:timer;

namespace Nuit
{
	export class Timer
	{
		std::chrono::time_point<std::chrono::high_resolution_clock> m_start;
		std::chrono::time_point<std::chrono::high_resolution_clock> m_prevTime;

	public:
		Timer() = default;
		~Timer() = default;

		void start()
		{
			m_start = std::chrono::high_resolution_clock::now();
			m_prevTime = m_start;
		}

		[[nodiscard]] double delta()
		{
			const auto now = std::chrono::high_resolution_clock::now();
			const double time =
				std::chrono::duration_cast<std::chrono::duration<double>>(now - m_prevTime).count();
			m_prevTime = now;

			return time;
		}

		[[nodiscard]] double peak_delta() const
		{
			const auto now = std::chrono::high_resolution_clock::now();
			return std::chrono::duration_cast<std::chrono::duration<double>>(now - m_prevTime)
				.count();
		}

		[[nodiscard]] double elapsed()
		{
			const auto now = std::chrono::high_resolution_clock::now();
			const double time =
				std::chrono::duration_cast<std::chrono::duration<double>>(now - m_start).count();
			m_start = now;
			return time;
		}

		[[nodiscard]] double peak_elapsed() const
		{
			const auto now = std::chrono::high_resolution_clock::now();
			return std::chrono::duration_cast<std::chrono::duration<double>>(now - m_start).count();
		}
	};
}
