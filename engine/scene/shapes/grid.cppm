module;
#include <GL/glew.h>
#include <vector>
#include <glm/vec2.hpp>
#include <glm/vec4.hpp>
export module nuit:grid;

import :shapes;

namespace Nuit
{
	export class Grid : public Shape2D
	{
		std::vector<glm::vec2> m_vertices;
		GLuint m_vao{};
		GLuint m_vbo{};

	public:
		glm::vec2 Size{};

		Grid() = default;
		Grid(const glm::vec2& position, const glm::vec2& size) :
			Shape2D(position), Size(size)
		{
		}

		~Grid() override;

		void _draw(const GLShaderProgram& shader) override;

		void generate(int rows, int cols);

		[[nodiscard]] const std::vector<glm::vec2>& get_vertices() const;

		template <size_t R, size_t C>
		void draw_filled(const GLShaderProgram& shader, const int (&map)[R][C])
		{
			const int rows = R;
			const int cols = C;
			const float dx = Size.x / static_cast<float>(cols);
			const float dy = Size.y / static_cast<float>(rows);

			shader.bind();
			shader.set_uniform("uColor", glm::vec4(0.6f, 0.6f, 0.6f, 1.0f));

			for (int r = 0; r < rows; r++)
			{
				for (int c = 0; c < cols; c++)
				{
					switch (map[r][c])
					{
					case 0:
						shader.set_uniform("uColor", glm::vec4(0.1f, 0.1f, 0.1f, 1.0f));
					case 1:
					{
						const float x = Position.x + dx * static_cast<float>(c);
						const float y = Position.y +
							dy * static_cast<float>(r); // (rows - 1 - r) * dy; // Reverse direction
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
						glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices,
						             GL_STATIC_DRAW);

						// Set the attrib pointer for the shader
						glEnableVertexAttribArray(0);
						glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float),
						                      nullptr);

						glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

						glDeleteBuffers(1, &vbo);
						glDeleteVertexArrays(1, &vao);
					}
					default:
						shader.set_uniform("uColor", glm::vec4(0.6f, 0.6f, 0.6f, 1.0f));
						break;
					}
				}
			}

			_draw(shader);
		}
	};
}