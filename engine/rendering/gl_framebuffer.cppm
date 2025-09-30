module;
#include <GL/glew.h>
#include <glm/vec2.hpp>
export module nuit:gl_framebuffer;

import :gl_texture;

namespace Nuit
{
	export class GLFramebuffer
	{
		GLuint m_fbo = 0;
		GLuint m_texID = 0;
		GLuint m_rbo = 0;
		int m_width{1}, m_height{1};	// Initialize with valid size

	public:
		GLFramebuffer();
		GLFramebuffer(int width, int height);
		~GLFramebuffer();

		void create();

		void bind() const;
		static void unbind();

		void update(int width, int height);

		[[nodiscard]] GLuint get_texture() const
		{
			return m_texID;
		}

		[[nodiscard]] GLuint get_frame_buffer() const
		{
			return m_fbo;
		}

		[[nodiscard]] GLuint get_render_buffer() const
		{
			return m_rbo;
		}

		[[nodiscard]] glm::ivec2 get_size() const
		{
			return {m_width, m_height};
		}
	};
} // namespace Nuit
