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
	glm::vec3 Scale{1.0f};
	float Rotation{PI_2};
	float Speed{5.0f};
	float RotationSpeed{2.5f};

public:
	Player();
	~Player() override;

	void _init() override;
	void _process(double delta) override;
	void _draw(GLShaderProgram& shader) override;

	void move_and_slide(glm::vec3 movement);

private:
	[[nodiscard]] glm::vec3 get_forward() const;
	[[nodiscard]] glm::vec3 get_right() const;
};