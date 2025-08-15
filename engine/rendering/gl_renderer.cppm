module;
#include <GL/glew.h>
#include <print>
export module nuit:gl_renderer;

namespace Nuit
{
	export class GLRenderer
	{
	public:
		GLRenderer() = default;

		~GLRenderer() = default;

		static void _init()
		{
			glewExperimental = GL_TRUE;

			if (const GLenum err = glewInit(); err != GLEW_OK)
			{
				std::println("GLEW failed to initialize correctly: {0}",
							 reinterpret_cast<const char*>(glewGetErrorString(err)));
			}

			std::println("Renderer: {0}", reinterpret_cast<const char*>(glGetString(GL_RENDERER)));
			std::println("OpenGL version: {0}", reinterpret_cast<const char*>(glGetString(GL_VERSION)));

			int numAttributes;
			glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &numAttributes);
			std::println("Maximum number of vertex attributes supported: {0}", numAttributes);
		}
	};
}