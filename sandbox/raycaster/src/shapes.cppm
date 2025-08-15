module;
#include <GL/glew.h>
#include <cmath>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <vector>
export module shapes;

class Shape
{
protected:
	GLuint m_vao{}, m_vbo{};
	std::vector<float> m_vertices;

public:
	glm::vec2 Position{0, 0};
	glm::vec3 Color{0, 0, 0};

	virtual void draw(GLuint shader) = 0;
};

export class Circle : public Shape
{
public:
	float Radius{1};
	int Segments;

public:
	explicit Circle(const int segments = 360) : Segments(segments)
	{
	}

	~Circle()
	{
		glDeleteBuffers(1, &m_vbo);
		glDeleteVertexArrays(1, &m_vao);
	}

	void _init()
	{
		m_vertices.resize(Segments * 2);
		set_vertices();

		glGenVertexArrays(1, &m_vao);
		glBindVertexArray(m_vao);

		glGenBuffers(1, &m_vbo);
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
		glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(float), m_vertices.data(), GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), nullptr);

		glBindVertexArray(0);
	}

	void draw(const GLuint shader) override
	{
		glUniform1f(glGetUniformLocation(shader, "uRadius"), Radius);
		glUniform3fv(glGetUniformLocation(shader, "uColor"), 1, &Color[0]);

		set_vertices();
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
		glBufferSubData(GL_ARRAY_BUFFER, 0, m_vertices.size() * sizeof(float), m_vertices.data());

		glBindVertexArray(m_vao);
		glDrawArrays(GL_LINE_LOOP, 0, Segments);
		glBindVertexArray(0);
	}

private:
	void set_vertices()
	{
		int index = 0;
		for (int i = 0; i < Segments; ++i)
		{
			const float theta = 2.0f * M_PI * i / Segments;
			m_vertices[index] = Position.x + std::cos(theta);
			index++;
			m_vertices[index] = Position.y + std::sin(theta);
			index++;
		}
	}
};

export class Square : public Shape
{
	public:

	void draw(GLuint shader) override
	{

	}
};
