module;
#include <GL/glew.h>
#include <glm/ext/matrix_transform.hpp>
#include <glm/glm.hpp>
#include <iostream>
module player;

import nuit;
import map;

using namespace Nuit;

Player::Player() = default;

Player::~Player()
{
	glDeleteVertexArrays(1, &m_vao);
	glDeleteBuffers(1, &m_vbo);
}

void Player::_init()
{
	constexpr float vertices[] = {-0.05f, -0.05f, 0.05f, -0.05f, 0.05f, 0.05f, -0.05f, 0.05f};

	glGenVertexArrays(1, &m_vao);
	glGenBuffers(1, &m_vbo);

	glBindVertexArray(m_vao);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
}

void Player::_process(const double delta)
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
	move_and_slide(movement);
}

void Player::_draw(const GLShaderProgram& shader)
{
	const glm::mat4 model = glm::translate(glm::mat4(1.0f),
										   glm::vec3{Position.x, Position.z, 0.0f}) *
		glm::scale(glm::mat4(1.0f), Scale);

	shader.bind();
	shader.set_uniform("uModel", model);
	shader.set_uniform("uColor", glm::vec4{0.0f, 1.0f, 1.0f, 1.0f});

	glBindVertexArray(m_vao);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
}

void Player::move_and_slide(const glm::vec3 movement)
{
	const glm::vec3 temp = Position + movement;

	int mapY = static_cast<int>(std::floor(temp.z));
	int mapX = static_cast<int>(std::floor(temp.x));

	if (!(map[(int)Position.z][int(Position.x + movement.x)] ||
		  map[(int)(Position.z + movement.z)][(int)Position.x]))
	{
		if (mapX < 0 || mapY < 0 || mapX >= 20 || mapY >= 20)
		{
			Position = temp;
		}
		else if (map[mapY][mapX] != 1)
		{
			Position = temp;
		}
		else if (map[mapY][static_cast<int>(Position.x)] != 1)
		{
			Position.z = temp.z;
		}
		else if (map[static_cast<int>(Position.z)][mapX] != 1)
		{
			Position.x = temp.x;
		}
	}
}
