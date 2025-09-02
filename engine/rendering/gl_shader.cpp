module;
#include <GL/glew.h>
#include <iostream>
#include <print>
module nuit;

namespace Nuit
{
	GLShaderProgram::~GLShaderProgram()
	{
		if (m_program)
		{
			glDeleteProgram(m_program);
		}
	}

	GLuint GLShaderProgram::id() const
	{
		return m_program;
	}

	void GLShaderProgram::create()
	{
		if (m_program)
		{
			unbind();
			glDeleteProgram(m_program);
		}

		m_program = glCreateProgram();
	}

	void GLShaderProgram::bind() const
	{
		if (m_program)
		{
			glUseProgram(m_program);
		}
	}

	void GLShaderProgram::unbind()
	{
		glUseProgram(0);
	}

	void GLShaderProgram::attach_shader(const GLuint shader) const
	{
		if (m_program)
		{
			glAttachShader(m_program, shader);
		}
	}

	void GLShaderProgram::compile_and_attach(const std::string& file, const GLenum type) const
	{
		const auto rawData = File::read(file);

		if (!rawData.has_value())
		{
			std::println(std::cerr, "Failed to read shader file!");
			return;
		}

		const char* data = rawData.value().c_str();

		const GLuint shader = glCreateShader(type);
		glShaderSource(shader, 1, &data, nullptr);
		glCompileShader(shader);

		GLint success;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			char log[512];
			glGetShaderInfoLog(shader, 512, nullptr, log);
			std::println(std::cerr, "{0}", log);
			return;
		}

		glAttachShader(m_program, shader);
		glDeleteShader(shader);
	}

	void GLShaderProgram::link() const
	{
		glLinkProgram(m_program);

		GLint success = 0;
		glGetProgramiv(m_program, GL_LINK_STATUS, &success);
		if (!success)
		{
			char log[1024];
			glGetProgramInfoLog(m_program, sizeof(log), nullptr, log);
			std::println(std::cerr, "Program link failed: {}", log);
		}
	}

	GLint GLShaderProgram::get_uniform_location(const std::string& name) const
	{
		return glGetUniformLocation(m_program, name.c_str());
	}
} // namespace Nuit
