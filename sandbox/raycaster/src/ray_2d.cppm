module;
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <vector>
export module ray_2d;

import nuit;
using namespace Nuit;

export class Ray2D
{
	GLuint m_vao{}, m_vbo{};

public:
	glm::vec2 Position;
	glm::vec2 Direction;
	glm::vec2 EndPosition;
	glm::ivec2 MapPosition;
	bool Hit{false};

public:
	Ray2D()
	{
		glGenVertexArrays(1, &m_vao);
		glGenBuffers(1, &m_vbo);
	}

	~Ray2D()
	{
		glDeleteVertexArrays(1, &m_vao);
		glDeleteBuffers(1, &m_vbo);
	}

	void _draw(GLShaderProgram& shader) const;

	void cast(const std::vector<std::vector<int>>& map);
};

export class Ray : public Ray2D
{
	GLuint m_vao = 0;
	GLuint m_vbo = 0;

public:
	~Ray() {
		glDeleteBuffers(1, &m_vbo);
		glDeleteVertexArrays(1, &m_vao);
	}

	void draw_vertical_line(const float x, const float yTop, const float x2, const float yBot)
	{
		if (m_vao == 0)
		{
			glGenVertexArrays(1, &m_vao);
			glGenBuffers(1, &m_vbo);
		}

		const float vertices[] = {
			static_cast<float>(x),  yTop,  // Top-left
			static_cast<float>(x),  yBot,  // Bottom-left
			static_cast<float>(x2), yBot,  // Bottom-right
			static_cast<float>(x2), yTop   // Top-right
		};

		glBindVertexArray(m_vao);
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

		glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}
};

