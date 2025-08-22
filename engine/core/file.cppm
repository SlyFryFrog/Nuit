module;
#include <optional>
#include <string>
export module nuit:file;

namespace Nuit
{
	export class File
	{
	public:
		static std::optional<std::string> read(const std::string& path);

		static void write(const std::string& path, const std::string& content);

		static void append(const std::string& path, const std::string& content);
	};
}