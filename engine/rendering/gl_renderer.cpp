module;
#include <GL/glew.h>
#include <print>
module nuit;

namespace Nuit
{
	void GLRenderer::init()
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

		init_dummy_texture();
	}

	void GLRenderer::set_polygon_mode(const PolygonMode mode)
	{
		if (mode == PolygonMode::FILL)
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}
		else if (mode == PolygonMode::POINT)
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
		}
		else if (mode == PolygonMode::LINE)
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}
	}

	void GLRenderer::enable_depth_testing(const bool enable)
	{
		if (enable)
		{
			glEnable(GL_DEPTH_TEST);
		}
		else
		{
			glDisable(GL_DEPTH_TEST);
		}
	}
} // namespace Nuit
