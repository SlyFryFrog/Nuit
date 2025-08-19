module;
#include <GL/glew.h>
#include <array>
#include <glm/ext/matrix_transform.hpp>
#include <glm/glm.hpp>
export module player;

import nuit;

using namespace Nuit;

export class Player
{
	GLuint m_vao{};
	GLuint m_vbo{};

public:
	glm::vec3 Position{};
	glm::vec3 Rotation{};
	glm::vec3 Scale{0.05f};

	float Speed = 0.5f;

public:
	Player() = default;
	~Player()
	{
		glDeleteVertexArrays(1, &m_vao);
		glDeleteBuffers(1, &m_vbo);
	}

	void _init()
	{
		constexpr float vertices[] = {
			-0.05f, -0.05f,
			 0.05f, -0.05f,
			 0.05f,  0.05f,
			-0.05f,  0.05f
		};

		glGenVertexArrays(1, &m_vao);
		glGenBuffers(1, &m_vbo);

		glBindVertexArray(m_vao);
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), nullptr);
	}

	void _process(const double delta)
	{
		glm::vec3 movement{};

		if (InputManager::is_pressed(KEY_A))
		{
			movement.x -= Speed * delta;
		}
		if (InputManager::is_pressed(KEY_D))
		{
			movement.x += Speed * delta;
		}
		if (InputManager::is_pressed(KEY_S))
		{
			movement.z -= Speed * delta;
		}
		if (InputManager::is_pressed(KEY_W))
		{
			movement.z += Speed * delta;
		}
		if (InputManager::is_pressed(KEY_LEFT_SHIFT) || InputManager::is_pressed(KEY_RIGHT_SHIFT))
		{
			movement.y -= Speed * delta;
		}
		if (InputManager::is_pressed(KEY_SPACE))
		{
			movement.y += Speed * delta;
		}

		Position += movement;
	}

	void _draw(const GLShaderProgram& shader) const
	{
		const glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3{Position.x, Position.z, 0.0f})
				* glm::scale(glm::mat4(1.0f), Scale);

		shader.bind();
		shader.set_uniform("uModel", model);
		shader.set_uniform("uColor", glm::vec4{0.0f, 1.0f, 1.0f, 1.0f});

		glBindVertexArray(m_vao);
	    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
	}
};