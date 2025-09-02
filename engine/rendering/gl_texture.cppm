module;
#include <GL/glew.h>
#include <vector>
export module nuit:gl_texture;

namespace Nuit
{
	export GLuint load_texture(const std::string& filename);

	export GLuint load_missing_texture(int width = 64, int height = 64);
} // namespace Nuit
