module;
#include <filesystem>

#if defined(__APPLE__)
#include <mach-o/dyld.h>
#endif
module nuit;

namespace Nuit
{
	/**
	 * @brief Returns the absolute path to the running executable.
	 *
	 * @return Absolute path to the executable.
	 *
	 * @note Works on macOS and Linux. Returns an empty path on failure.
	 */
	std::string get_working_directory()
	{
		// MacOS doesn't use the pwd of the executable when launching from clicking on the
		// executable directly.
#if defined(__APPLE__)
		char buffer[PATH_MAX];
		uint32_t size = sizeof(buffer);
		if (_NSGetExecutablePath(buffer, &size) == 0)
		{
			return std::filesystem::canonical(buffer).parent_path().string();
		}
#endif
		// Default implementation
		return std::filesystem::current_path().string();
	}
} // namespace Nuit
