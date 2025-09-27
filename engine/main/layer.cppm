module;
#include <GL/glew.h>
export module nuit:layer;

namespace Nuit
{
	export class Layer
	{
		GLuint m_shader;
		GLuint m_texture;
		GLuint m_vao;
		GLuint m_vbo;
		GLuint m_ebo;

	public:
		Layer() = default;

		virtual ~Layer()
		{
		}

		virtual void _init()
		{
		}

		virtual void _process(double delta)
		{
		}

		virtual void _render()
		{
		}
	};
} // namespace Nuit
