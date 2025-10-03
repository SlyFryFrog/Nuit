module;
#include <GL/glew.h>
#include <glm/vec2.hpp>
export module nuit:gl_framebuffer;

namespace Nuit
{
	/**
	 * @brief Manages OpenGL framebuffer objects (FBO) for off-screen rendering.
	 *
	 * The `GLFramebuffer` class encapsulates the creation, binding, and management of an OpenGL
	 * framebuffer, including its associated color texture and depth/stencil renderbuffer. It is
	 * designed for use in rendering pipelines where off-screen rendering or post-processing is
	 * required.
	 */
	export class GLFramebuffer
	{
		GLuint m_fbo{};	  ///< OpenGL framebuffer object handle.
		GLuint m_texID{}; ///< OpenGL texture handle for color attachment.
		GLuint m_rbo{};	  ///< OpenGL renderbuffer object handle for depth/stencil attachment.
		int m_width{1}, m_height{1}; ///< Dimensions of the framebuffer (default: 1x1).

	public:
		/**
		 * @brief Constructs a `GLFramebuffer` with default dimensions (1x1).
		 */
		GLFramebuffer();

		/**
		 * @brief Constructs a `GLFramebuffer` with the specified dimensions.
		 *
		 * @param width The width of the framebuffer in pixels.
		 * @param height The height of the framebuffer in pixels.
		 */
		GLFramebuffer(int width, int height);

		/**
		 * @brief Destroys the `GLFramebuffer` and releases all associated OpenGL resources.
		 */
		~GLFramebuffer();

		/**
		 * @brief Creates the framebuffer, texture, and renderbuffer objects.
		 *
		 * Initializes the framebuffer with a color texture and a depth/stencil renderbuffer.
		 * Must be called before the framebuffer can be used for rendering.
		 */
		void create();

		/**
		 * @brief Binds the framebuffer for rendering.
		 *
		 * All subsequent rendering commands will target this framebuffer until another is bound.
		 */
		void bind() const;

		/**
		 * @brief Unbinds the currently bound framebuffer, reverting to the default framebuffer.
		 */
		static void unbind();

		/**
		 * @brief Updates the dimensions of the framebuffer and its attachments.
		 *
		 * @param width The new width of the framebuffer in pixels.
		 * @param height The new height of the framebuffer in pixels.
		 */
		void update(int width, int height);

		/**
		 * @brief Gets the OpenGL texture handle associated with this framebuffer.
		 *
		 * @return GLuint The OpenGL texture ID.
		 */
		[[nodiscard]] GLuint get_texture() const
		{
			return m_texID;
		}

		/**
		 * @brief Gets the OpenGL framebuffer handle.
		 *
		 * @return GLuint The OpenGL framebuffer ID.
		 */
		[[nodiscard]] GLuint get_frame_buffer() const
		{
			return m_fbo;
		}

		/**
		 * @brief Gets the OpenGL renderbuffer handle.
		 *
		 * @return GLuint The OpenGL renderbuffer ID.
		 */
		[[nodiscard]] GLuint get_render_buffer() const
		{
			return m_rbo;
		}

		/**
		 * @brief Gets the dimensions of the framebuffer.
		 *
		 * @return glm::ivec2 The width and height of the framebuffer as a 2D vector.
		 */
		[[nodiscard]] glm::ivec2 get_size() const
		{
			return {m_width, m_height};
		}
	};
} // namespace Nuit
