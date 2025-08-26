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

		/**
		 * @brief Resets the timer to default state.
		 */
		void start();

		/**
		 * @brief Gets the delta time between the last delta() call and now. Updates timeframe for
		 * preparation of next delta.
		 *
		 * @return Time since last delta call.
		 */
		[[nodiscard]] double delta();
		/**
		 * @brief Returns the time since last delta, does not modify timeframe.
		 * @return Time since start of timeframe.
		 */
		[[nodiscard]] double peak_delta() const;

		[[nodiscard]] double elapsed();
		[[nodiscard]] double peak_elapsed() const;
	};
} // namespace Nuit
