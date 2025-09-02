module;
#include <GL/glew.h>
#include <glm/vec2.hpp>
#include <glm/vec4.hpp>
#include <vector>
export module nuit:grid;

import :shapes;

namespace Nuit
{
	export class Grid final : public Shape2D
	{
		std::vector<glm::vec2> m_vertices;
		GLuint m_vao{};
		GLuint m_vbo{};

	public:
		glm::vec2 Size{};

		Grid() = default;

		Grid(const glm::vec2& position, const glm::vec2& size) : Shape2D(position), Size(size)
		{
		}

		~Grid() override;

		void _draw(const GLShaderProgram& shader) override;

		void generate(const std::vector<std::vector<int>>& grid);

		[[nodiscard]] const std::vector<glm::vec2>& get_vertices() const;

		void draw_filled(const GLShaderProgram& shader, const std::vector<std::vector<int>>& map);
	};
} // namespace Nuit
