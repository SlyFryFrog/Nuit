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

		Grid() = default;

		Grid(const glm::vec2& position) : Shape2D(position)
		{
		}

		~Grid() override;

		void _draw(GLShaderProgram& shader) override;

		void generate(const std::vector<std::vector<int>>& grid);

		[[nodiscard]] const std::vector<glm::vec2>& get_vertices() const;

		void draw_filled(GLShaderProgram& shader, const std::vector<std::vector<int>>& map);
	};
} // namespace Nuit
