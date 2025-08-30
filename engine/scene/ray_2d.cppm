module;
#include <glm/glm.hpp>
#include <vector>
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
		glm::ivec2 MapPosition;
		bool Hit{false};

		void _draw(const GLShaderProgram& shader) const;

		void cast(const std::vector<std::vector<int>>& map);
	};
} // namespace Nuit
