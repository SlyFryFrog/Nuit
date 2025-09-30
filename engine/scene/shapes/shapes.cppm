module;
#include <GL/glew.h>
#include <glm/ext/matrix_transform.hpp>
#include <glm/vec2.hpp>
#include <print>
#include <vector>
export module nuit:shapes;

import :gl_shader;
import :object;

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
} // namespace Nuit
