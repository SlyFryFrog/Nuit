module;
#include <filesystem>
export module nuit:utils;

namespace Nuit
{
#ifdef USE_DOUBLE
	export using fp_type = double;
#else
	export using fp_type = float;
#endif

	/**
	 * @brief Returns the absolute path to the running executable.
	 *
	 * @return Absolute path to the executable.
	 *
	 * @note Works on macOS and Linux. Returns an empty path on failure.
	 */
	export std::string get_working_directory();

	/**
	 * @brief Current working directory where the executable is located. Is determined at runtime.
	 */
	export const inline std::string WorkingDirectory = get_working_directory();

	export struct StringHasher
	{
		std::size_t operator()(const std::string& string) const
		{
			size_t hash = 0;
			for (const auto& c : string)
			{
				hash = hash * 31 + c;
			}

			return hash;
		}
	};
}
