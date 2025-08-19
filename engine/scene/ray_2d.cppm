module;
#include <iostream>

#include <GL/glew.h>
#include <glm/ext/matrix_transform.hpp>
#include <glm/glm.hpp>
#include <ostream>
export module nuit:ray_2d;

import :gl_shader;
import :math;

namespace Nuit
{
	export class Ray2D
	{
	public:
		glm::vec2 Position;
		glm::vec2 Direction;
		glm::vec2 EndPosition;
		float Angle;
		int RaysCount;

		template <size_t R, size_t C>
		void check_hit(int (&map)[R][C], const glm::vec2 size)
		{
			const glm::vec2 stepSize = {sqrt(1 + pow((Direction.y / Direction.x), 2)),
								  sqrt(1 + pow((Direction.x / Direction.y), 2))};
			glm::ivec2 mapCheck = Position;
			std::cout << Position.x << ", " << Position.y << std::endl;
			glm::vec2 rayLength{};

			glm::ivec2 step{};

			// Determine directions of steps
			if (Direction.x < 0)
			{
				step.x = -1;
				rayLength.x = (Position.x - static_cast<float>(mapCheck.x)) * stepSize.x;
			}
			else if (Direction.x > 0)
			{
				step.x = 1;
				rayLength.x = (static_cast<float>(mapCheck.x + 1) - Position.x) * stepSize.x;
			}
			if (Direction.y < 0)
			{
				step.y = -1;
				rayLength.y = (Position.y - static_cast<float>(mapCheck.y)) * stepSize.y;
			}
			else if (Direction.y > 0)
			{
				step.y = 1;
				rayLength.y = (static_cast<float>(mapCheck.y + 1) - Position.y) * stepSize.y;
			}

			bool tileFound = false;
			constexpr float maxDistance = 100.0f;
			float distance = 0.0f;
			while (!tileFound && distance < maxDistance)
			{
				if (rayLength.x < rayLength.y)
				{
					mapCheck.x += step.x;
					distance = rayLength.x;
					rayLength.x += stepSize.x;
				}
				else
				{
					mapCheck.y += step.y;
					distance = rayLength.y;
					rayLength.y += stepSize.y;
				}

				// Ensure we aren't going out of bounds
				if (mapCheck.x >= 0 && mapCheck.x < C && mapCheck.y >= 0 && mapCheck.y < R)
				{
					switch (map[mapCheck.y][mapCheck.x])
					{
					case 1:
						tileFound = true;
						break;
					default:
						break;
					}
				}
			}

			glm::vec2 intersection{};
			if (tileFound)
			{
				intersection = Position + Direction * distance;
			}

			EndPosition = Direction;
		}

		void _draw(const GLShaderProgram& shader) const
		{
			const float vertices[] = {
				0.0, 0.0,				 // Origin relative to the model matrix
				EndPosition.x, EndPosition.y // End point
			};

			// Translate ray to position
			const glm::mat4 model = glm::translate(glm::mat4(1.0f),
												   glm::vec3{Position.x, Position.y, 0.0f});

			GLuint vao, vbo;
			glGenVertexArrays(1, &vao);
			glGenBuffers(1, &vbo);

			glBindVertexArray(vao);
			glBindBuffer(GL_ARRAY_BUFFER, vbo);
			glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), nullptr);

			shader.bind();
			shader.set_uniform("uModel", model);
			glDrawArrays(GL_LINES, 0, 2);

			glDeleteBuffers(1, &vbo);
			glDeleteVertexArrays(1, &vao);
		}
	};
} // namespace Nuit
