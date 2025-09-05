module;
#include <GL/glew.h>
#include <vector>
export module nuit:gl_texture;

namespace Nuit
{
	/**
	 * @brief Returns the corresponding OpenGL image format enum based on the number of channels.
	 *
	 * @param channelCount Number of color channels (1–4).
	 * @return The OpenGL format. Returns GL_NONE if the channel count is invalid.
	 */
	export GLint get_image_format(int channelCount);

	/**
	 * @brief Loads an image for use in OpenGL.
	 *
	 * @param filename Image being loaded.
	 * @return OpenGL texture ID.
	 */
	export GLuint load_texture(const std::string& filename);

	/**
	 * @brief Programmatically generates a texture with the given size. Alternates in a purple/black
	 * grid.
	 *
	 * @param width Width of the texture being generated.
	 * @param height Height of the texture being generated.
	 * @return GL Texture ID.
	 */
	export GLuint load_missing_texture(int width = 32, int height = 32);

	export void init_dummy_texture();

	export GLuint DummyTextureID = 0;
} // namespace Nuit
