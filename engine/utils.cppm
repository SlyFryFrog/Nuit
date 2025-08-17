module;
#include <filesystem>
#include <fstream>
#include <iostream>
#include <optional>
#include <ostream>
#include <sstream>

#if defined(__APPLE__)
#include <mach-o/dyld.h>
#endif
export module nuit:utils;

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
std::filesystem::path get_working_directory()
{
	// MacOS doesn't use the pwd of the executable when launching from clicking on the
	// executable directly.
#if defined(__APPLE__)
	char buffer[PATH_MAX];
	uint32_t size = sizeof(buffer);
	if (_NSGetExecutablePath(buffer, &size) == 0)
	{
		return std::filesystem::canonical(buffer).parent_path();
	}
#endif
	// Default implementation
	return std::filesystem::current_path().string();
}

/**
 * @brief Current working directory where the executable is located.
 */
export const std::string WorkingDirectory = get_working_directory();

export std::optional<std::string> read(const std::string& path)
{
	std::ifstream file(path);
	if (!file)
	{
		std::println(std::cerr, "Failed to open file: {}", path);
		return std::nullopt;
	}

	std::ostringstream ss;
	ss << file.rdbuf();
	return ss.str();
}
