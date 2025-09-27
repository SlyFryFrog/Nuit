module;
#include <GL/glew.h>
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
		void unbind() const;

		void update(int width, int height);

		[[nodiscard]] GLuint get_texture() const
		{
			return m_texID;
		}
	};
} // namespace Nuit
