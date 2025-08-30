module;
#include <GL/glew.h>
#include <glm/ext/matrix_transform.hpp>
#include <glm/glm.hpp>
#include <vector>
module nuit;

import :gl_shader;
import :math;

namespace Nuit
{
	void Ray2D::_draw(const GLShaderProgram& shader) const
	{
		const glm::vec2 localEnd = EndPosition - Position;

		const float vertices[] = {
			0.0f, 0.0f,			   // Origin
			localEnd.x, localEnd.y // Endpoint relative to Position
		};

		const glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3{Position, 0.0f});

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
	void Ray2D::cast(const std::vector<std::vector<int>>& map)
	{
		constexpr float maxDistance = 100.0f;

		const glm::vec2 stepSize = {
			std::sqrt(1.0f + (Direction.y * Direction.y) / (Direction.x * Direction.x)),
			std::sqrt(1.0f + (Direction.x * Direction.x) / (Direction.y * Direction.y))};

		// Convert Position (float/double) to an int
		// Need to floor to now round low negative decimals up (ie -0.003 not 0, now -1)
		glm::ivec2 mapCheck = glm::ivec2(std::floor(Position.x), std::floor(Position.y));
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
			if (mapCheck.x >= 0 && mapCheck.x < map.at(mapCheck.y).size() && mapCheck.y >= 0 && mapCheck.y < map.size())
			{
				switch (map[mapCheck.y][mapCheck.x])
				{
				case 1:
				case 2:
				case 3:
					MapPosition = mapCheck;
					tileFound = true;
					break;
				default:
					break;
				}
			}
		}

		if (tileFound)
		{
			Hit = true;
			EndPosition = Position + Direction * distance;
		}
		else
		{
			Hit = false;
			EndPosition = Position + Direction * maxDistance;
		}
	}
} // namespace Nuit
