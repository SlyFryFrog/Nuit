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
	glm::vec2 HitPosition;
	float TexCoordX;
	bool Hit{};

public:
	Ray2D();
	~Ray2D();

	void _draw(GLShaderProgram& shader) const;

	void cast(const std::vector<std::vector<int>>& map);
};

export class Ray : public Ray2D
{
	GLuint m_vao{};
	GLuint m_vbo{};

public:
	~Ray();

	void draw_vertical_line(float x, float yTop, float x2, float yBot);
};
