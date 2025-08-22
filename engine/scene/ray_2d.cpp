module;
#include <GL/glew.h>
#include <glm/ext/matrix_transform.hpp>
#include <glm/glm.hpp>
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
} // namespace Nuit
