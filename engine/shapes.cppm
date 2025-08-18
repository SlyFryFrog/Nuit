module;
#include <GL/glew.h>
#include <glm/vec2.hpp>
#include <iostream>
#include <print>
#include <vector>
export module nuit:shapes;

namespace Nuit
{

	export class Shape2D
	{
	public:
		glm::vec2 Position;
	};

	export class Circle
	{
	public:
	};

	export class Grid
	{
		std::vector<glm::vec2> m_vertices;
		GLuint m_vao{};
		GLuint m_vbo{};

	public:
		glm::vec2 Position{};
		glm::vec2 Size{};

		Grid() = default;
		Grid(const glm::vec2& pos, const glm::vec2& size) : Size(size)
		{
			// Moves anchor to center.
			// Without this, it anchors origin in top-right.
			Position = pos + size / 2.0f;
		}

		~Grid()
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

		void generate(const int rows, const int cols)
		{
			m_vertices.clear();

			const float dx = Size.x / static_cast<float>(cols);
			const float dy = Size.y / static_cast<float>(rows);

			// Generate vertical lines
			for (int c = 0; c <= cols; ++c)
			{
				float x = Position.x + c * dx;
				m_vertices.emplace_back(x, Position.y);
				m_vertices.emplace_back(x, Position.y + Size.y);
			}

			// Generate horizontal lines
			for (int r = 0; r <= rows; ++r)
			{
				float y = Position.y + r * dy;
				m_vertices.emplace_back(Position.x, y);
				m_vertices.emplace_back(Position.x + Size.x, y);
			}

			glGenVertexArrays(1, &m_vao);
			glBindVertexArray(m_vao);
			glGenBuffers(1, &m_vbo);
			glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
			glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(glm::vec2), m_vertices.data(),
						 GL_STATIC_DRAW);
			glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
			glEnableVertexAttribArray(0);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindVertexArray(0);
		}

		[[nodiscard]] const std::vector<glm::vec2>& get_vertices() const
		{
			return m_vertices;
		}

		void _draw() const
		{
			glBindVertexArray(m_vao);
			glDrawArrays(GL_LINES, 0, static_cast<GLsizei>(m_vertices.size()));
		}

		template<size_t R, size_t C>
		void draw_filled(const int (&map)[R][C]) const
		{
			const int rows = R;
			const int cols = C;
			const float dx = Size.x / static_cast<float>(cols);
			const float dy = Size.y / static_cast<float>(rows);

			for (int r = 0; r < rows; r++)
			{
				for (int c = 0; c < cols; c++)
				{
					switch (map[r][c])
					{
					case 1:
						{
							const float x = Position.x + c * dx;
							const float y = Position.y + (rows - 1 - r) * dy;	// Reverse direction
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

							glGenBuffers(1, &vbo);
							glBindBuffer(GL_ARRAY_BUFFER, vbo);
							glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices,
										 GL_STATIC_DRAW);

							glEnableVertexAttribArray(0);
							glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float),
												  nullptr);

							glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

							glDeleteBuffers(1, &vbo);
							glDeleteVertexArrays(1, &vao);
							break;
						}
					default:
						break;
					}
				}
			}

			_draw();
		}
	};
} // namespace Nuit
