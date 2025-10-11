module;
#include <GL/glew.h>
#include <vector>
#include <string>
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
	 * @brief Creates an empty texture of widthxheight size.
	 *
	 * @note This is useful for when you want to draw to a texture (i.e. for a framebuffer).
	 *
	 * @param width Image width.
	 * @param height Image Height.
	 * @return Texture ID.
	 */
	export GLuint create_texture(int width, int height);

	/**
	 * @brief Loads an image for use in OpenGL.
	 *
	 * @param filename Image being loaded.
	 * @return Texture ID.
	 */
	export GLuint load_texture(const std::string& filename);

	/**
	 * @brief Programmatically generates a texture with the given size. Alternates in a purple/black
	 * grid.
	 *
	 * @param width Width of the texture being generated.
	 * @param height Height of the texture being generated.
	 * @return Texture ID.
	 */
	export GLuint load_missing_texture(int width = 32, int height = 32);

	/**
	 * @brief Creates an empty, minimal texture to silence warnings.
	 */
	export void init_dummy_texture();

	/**
	 * @brief Set at initiation of GLRenderer.
	 */
	export GLuint DummyTextureID = 0;
} // namespace Nuit
