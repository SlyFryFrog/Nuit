module;
#include <GL/glew.h>
#include <glm/vec3.hpp>
export module player;

import nuit;
import map;

using namespace Nuit;

export class Player
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
	~Player();

	void _init();
	void _process(double delta);
	void _draw(const GLShaderProgram& shader);

	void move_and_slide(glm::vec3 movement);
};