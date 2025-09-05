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
		movement.x -= Speed * static_cast<float>(delta);
	}
	if (InputManager::is_pressed(KEY_D))
	{
		movement.x += Speed * static_cast<float>(delta);
	}
	if (InputManager::is_pressed(KEY_S))
	{
		movement.z -= Speed * static_cast<float>(delta);
	}
	if (InputManager::is_pressed(KEY_W))
	{
		movement.z += Speed * static_cast<float>(delta);
	}
	move_and_slide(movement);
}

void Player::_draw(const GLShaderProgram& shader)
{
	// clang-format off
	const glm::mat4 model = glm::translate(
		glm::mat4(1.0f),
		glm::vec3{Position.x, Position.z, 0.0f}) * glm::scale(glm::mat4(1.0f), Scale);
	// clang-format on

	shader.bind();
	shader.set_uniform("uModel", model);
	shader.set_uniform("uColor", glm::vec4{0.0f, 1.0f, 1.0f, 1.0f});

	glBindVertexArray(m_vao);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
}

void Player::move_and_slide(const glm::vec3 movement)
{
	const float newX = Position.x + movement.x;
	const float newZ = Position.z + movement.z;
	const int mapY = std::floor(Position.z + movement.z);

	if (newX >= 0 && newX < generatedMap[mapY].size() &&
		generatedMap[static_cast<int>(Position.z)][static_cast<int>(newX)] == 0)
	{
		Position.x = newX;
	}
	if (newZ >= 0 && newZ < generatedMap.size() &&
		generatedMap[static_cast<int>(newZ)][static_cast<int>(Position.x)] == 0)
	{
		Position.z = newZ;
	}
}
