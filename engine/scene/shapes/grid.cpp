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
		glDeleteBuffers(1, &m_vbo);
		glDeleteVertexArrays(1, &m_vao);
	}

	void Grid::_draw(GLShaderProgram& shader)
	{
		shader.bind();
		// Color of gridlines
		shader.set_uniform("uColor", glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));

		glBindVertexArray(m_vao);
		glDrawArrays(GL_LINES, 0, static_cast<GLsizei>(m_vertices.size()));
	}

	void Grid::generate(const std::vector<std::vector<int>>& grid)
	{
		m_vertices.clear();

		// Get average X and Y of each space
		size_t maxColumn = 0;
		for (const auto& row : grid)
			maxColumn = std::max(row.size(), maxColumn);

		// Generate horizontal lines
		for (int r = 0; r <= grid.size(); r++)
		{
			constexpr float dy = 1;
			float y = Position.y + dy * static_cast<float>(r);
			m_vertices.emplace_back(Position.x, y);
			m_vertices.emplace_back(Position.x + maxColumn, y);
		}

		// Generate vertical lines
		for (int c = 0; c <= maxColumn; c++)
		{
			constexpr float dx = 1;
			float x = Position.x + dx * static_cast<float>(c);
			m_vertices.emplace_back(x, Position.y);
			m_vertices.emplace_back(x, Position.y + grid.size());
		}

		glGenVertexArrays(1, &m_vao);
		glBindVertexArray(m_vao);
		glGenBuffers(1, &m_vbo);

		glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
		glBufferData(GL_ARRAY_BUFFER, static_cast<long>(m_vertices.size() * sizeof(glm::vec2)),
					 m_vertices.data(), GL_STATIC_DRAW);

		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
		glEnableVertexAttribArray(0);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}

	const std::vector<glm::vec2>& Grid::get_vertices() const
	{
		return m_vertices;
	}
	void Grid::draw_filled(GLShaderProgram& shader, const std::vector<std::vector<int>>& map)
	{
		const int rows = map.size();

		shader.bind();
		shader.set_uniform("uColor", glm::vec4(0.6f, 0.6f, 0.6f, 1.0f));

		for (int r = 0; r < rows; r++)
		{
			const int cols = map.at(r).size();

			for (int c = 0; c < cols; c++)
			{
				constexpr float dx = 1;
				constexpr float dy = 1;

				switch (map[r][c])
				{
				case 0:
					shader.set_uniform("uColor", glm::vec4(0.1f, 0.1f, 0.1f, 1.0f));
					break;
				case 1:
					shader.set_uniform("uColor", glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));
					break;
				case 2:
					shader.set_uniform("uColor", glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
					break;
				case 3:
					shader.set_uniform("uColor", glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
					break;
				default:
					shader.set_uniform("uColor", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
				}
				const float x = Position.x + dx * static_cast<float>(c);
				const float y = Position.y +
					dy * static_cast<float>(r); // (rows - 1 - r) * dy; // Reverse
				// direction
				// clang-format off
				// Define vertices for a square
				// Used to visualize position of player relative to grid of map
				const float vertices[] = {
					x, y,				// bottom left
					x + dx, y,			// bottom right
					x + dx, y + dy,		// top right
					x, y + dy			// top left
				};
				// clang-format on


				GLuint vao, vbo;
				glGenVertexArrays(1, &vao);
				glBindVertexArray(vao);

				// Bind buffers and define buffer data for vbo
				glGenBuffers(1, &vbo);
				glBindBuffer(GL_ARRAY_BUFFER, vbo);
				glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

				// Set the attrib pointer for the shader
				glEnableVertexAttribArray(0);
				glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), nullptr);

				glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

				glDeleteBuffers(1, &vbo);
				glDeleteVertexArrays(1, &vao);
			}
		}

		_draw(shader);
	}
} // namespace Nuit
