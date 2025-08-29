module;
#include <GL/glew.h>
#include <glm/vec3.hpp>
export module player;

import map;
import nuit;

using namespace Nuit;

export class Player final : Object
{
	GLuint m_vao{};
	GLuint m_vbo{};

public:
	glm::vec3 Position{};
	glm::vec3 Rotation{};
	glm::vec3 Scale{1.0f};

	float Speed = 5.0f;

public:
	Player();
	~Player() override;

	void _init() override;
	void _process(double delta) override;
	void _draw(const GLShaderProgram& shader) override;

	void move_and_slide(glm::vec3 movement);
};