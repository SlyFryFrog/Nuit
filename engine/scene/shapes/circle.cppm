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

		~Circle() override;

		void _init() override;

		void _draw(const GLShaderProgram& shader) override;

	private:
		void set_vertices();
	};
} // namespace Nuit
