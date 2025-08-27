module;
#include <GL/glew.h>
#include <glm/ext/matrix_transform.hpp>
#include <glm/vec2.hpp>
#include <print>
#include <vector>
module nuit;

namespace Nuit
{
	Grid::~Grid()
	{
		if (m_vbo)
		{
			glDeleteBuffers(1, &m_vbo);
		}
		if (m_vao)
		{
			glDeleteVertexArrays(1, &m_vao);
		}
	}

	void Grid::_draw(const GLShaderProgram& shader)
	{
		shader.bind();
		// Color of gridlines
		shader.set_uniform("uColor", glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));

		glBindVertexArray(m_vao);
		glDrawArrays(GL_LINES, 0, static_cast<GLsizei>(m_vertices.size()));
	}

	void Grid::generate(const int rows, const int cols)
	{
		m_vertices.clear();

		// Get average X and Y of each space
		const float dx = Size.x / static_cast<float>(cols);
		const float dy = Size.y / static_cast<float>(rows);

		// Generate vertical lines
		for (int c = 0; c <= cols; c++)
		{
			float x = Position.x + dx * static_cast<float>(c);
			m_vertices.emplace_back(x, Position.y);
			m_vertices.emplace_back(x, Position.y + Size.y);
		}

		// Generate horizontal lines
		for (int r = 0; r <= rows; r++)
		{
			float y = Position.y + dy * static_cast<float>(r);
			m_vertices.emplace_back(Position.x, y);
			m_vertices.emplace_back(Position.x + Size.x, y);
		}

		glGenVertexArrays(1, &m_vao);
		glBindVertexArray(m_vao);
		glGenBuffers(1, &m_vbo);

		glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
		glBufferData(GL_ARRAY_BUFFER, static_cast<long>(m_vertices.size() * sizeof(glm::vec2)), m_vertices.data(),
					 GL_STATIC_DRAW);

		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
		glEnableVertexAttribArray(0);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}

	const std::vector<glm::vec2>& Grid::get_vertices() const
	{
		return m_vertices;
	}
}
