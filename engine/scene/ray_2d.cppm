module;
#include <glm/glm.hpp>
#include <vector>
#include <GL/glew.h>
export module nuit:ray_2d;

import :gl_shader;
import :math;

namespace Nuit
{
	export class Ray2D
	{
		GLuint m_vao{}, m_vbo{};

	public:
		glm::vec2 Position;
		glm::vec2 Direction;
		glm::vec2 EndPosition;
		glm::ivec2 MapPosition;
		bool Hit{false};

	public:
		Ray2D()
		{
			glGenVertexArrays(1, &m_vao);
			glGenBuffers(1, &m_vbo);
		}

		~Ray2D()
		{
			glDeleteVertexArrays(1, &m_vao);
			glDeleteBuffers(1, &m_vbo);
		}

		void _draw(const GLShaderProgram& shader) const;

		void cast(const std::vector<std::vector<int>>& map);
	};
} // namespace Nuit
