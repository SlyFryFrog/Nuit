module;
#include <fstream>
#include <iosfwd>
#include <iostream>
#include <optional>
#include <ostream>
#include <print>
#include <sstream>
#include <string>
module nuit;

namespace Nuit
{
	std::optional<std::string> File::read(const std::string& path)
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

	void File::write(const std::string& path, const std::string& content)
	{
		std::ofstream out(path, std::ios::trunc);

		if (!out)
		{
			std::println(std::cerr, "Failed to open file for writing: {}", path);
			return;
		}

		out << content;
	}

	void File::append(const std::string& path, const std::string& content)
	{
		std::ofstream file(path, std::ios::app);

		if (!file)
		{
			std::println(std::cerr, "Failed to open file for appending: {}", path);
		}

		file << content;
	}
} // namespace Nuit
