module;
#include <GL/glew.h>
#include <glm/ext/matrix_transform.hpp>
#include <glm/vec2.hpp>
#include <print>
#include <vector>
export module nuit:shapes;

import :gl_shader;
import :object;
import :ray_2d;

namespace Nuit
{
	export class Shape2D : Object
	{
	public:
		glm::vec2 Position{};

		Shape2D() = default;

		explicit Shape2D(glm::vec2 position) : Position(position)
		{

		}
	};

	export class Ray : public Ray2D
	{
		GLuint m_vao = 0;
		GLuint m_vbo = 0;

	public:
		~Ray() {
			glDeleteBuffers(1, &m_vbo);
			glDeleteVertexArrays(1, &m_vao);
		}

		void draw_vertical_line(const int x, const float yTop, const int x2, const float yBot)
		{
			if (m_vao == 0)
			{
				glGenVertexArrays(1, &m_vao);
				glGenBuffers(1, &m_vbo);
			}

			const float vertices[] = {
				static_cast<float>(x), yTop,
				static_cast<float>(x2), yBot
			};

			glBindVertexArray(m_vao);

			glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
			glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);

			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

			glDrawArrays(GL_LINES, 0, 2);

			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindVertexArray(0);
		}
	};
} // namespace Nuit
