module;
#include <optional>
#include <string>
export module nuit:file;

namespace Nuit
{
	export class File
	{
	public:
		/**
		 * Reads the contents of a file and returns std::optional<std::string> to check the success
		 * of the read.
		 *
		 * @param path	Path to the file. If a relative path is provided, your program
		 * must set the current working directory beforehand - i.e. at runtime.
		 * @return std::optional containing the file contents if the file was
		 * successfully opened and read; std::nullopt otherwise.
		 */
		static std::optional<std::string> read(const std::string& path);

		static void write(const std::string& path, const std::string& content);

		static void append(const std::string& path, const std::string& content);
	};
} // namespace Nuit
