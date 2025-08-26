module;
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <vector>
export module nuit:circle;

import :object;

namespace Nuit
{
	export class Circle : Object
	{
		GLuint m_vao{}, m_vbo{};
		int m_segments{};
		std::vector<float> m_vertices;

	public:
		float Radius{1.0f};
		glm::vec2 Position{};
		glm::vec4 Color{};

	public:
		explicit Circle(const int segments = 360) : m_segments(segments)
		{
		}

		~Circle() override
		{
			glDeleteBuffers(1, &m_vbo);
			glDeleteVertexArrays(1, &m_vao);
		}


		void _init() override
		{
			m_vertices.resize(m_segments * 2);	// Stores 2D positional coords
			set_vertices();

			glGenVertexArrays(1, &m_vao);
			glBindVertexArray(m_vao);

			glGenBuffers(1, &m_vbo);
			glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
			glBufferData(GL_ARRAY_BUFFER, static_cast<long>(m_vertices.size() * sizeof(float)),
						 m_vertices.data(), GL_STATIC_DRAW);

			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), nullptr);

			glBindVertexArray(0);
		}

		void _draw(const GLShaderProgram& shader) override
		{
			if (m_vao == 0) {
				_init();
			}

			shader.bind();
			shader.set_uniform("uRadius", Radius);
			shader.set_uniform("uColor", Color);

			set_vertices();
			glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
			glBufferSubData(GL_ARRAY_BUFFER, 0,
							static_cast<long>(m_vertices.size() * sizeof(float)),
							m_vertices.data());

			glBindVertexArray(m_vao);
			glDrawArrays(GL_LINE_LOOP, 0, m_segments);
			glBindVertexArray(0);
		}

	private:
		void set_vertices()
		{
			int index = 0;
			for (int i = 0; i < m_segments; ++i)
			{
				const auto theta = static_cast<float>(2.0f * M_PI * i / m_segments);
				m_vertices[index] = std::cos(theta);
				m_vertices[index + 1] = std::sin(theta);
				index += 2;

			}
		}
	};
} // namespace Nuit
