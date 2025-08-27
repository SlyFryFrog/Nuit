module;
#include <glm/glm.hpp>
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
		bool Hit{false};

		template <size_t R, size_t C>
		void cast(int (&map)[R][C])
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

		void _draw(const GLShaderProgram& shader) const;
	};
} // namespace Nuit
