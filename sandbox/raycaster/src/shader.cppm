module;
#include <GL/glew.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <optional>
#include <print>
export module shader;

import nuit;

std::optional<std::string> read(const std::string& path)
{
	std::ifstream file(path);
	if (!file)
	{
		std::println(std::cerr, "Failed to open file: {}", path);
		return std::nullopt;
	}

	std::ostringstream ss;
	ss << file.rdbuf();
	return ss.str();
}

GLuint compileShader(const GLenum type, const char* src)
{
	const GLuint shader = glCreateShader(type);
	glShaderSource(shader, 1, &src, nullptr);
	glCompileShader(shader);

	GLint success;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		char log[512];
		glGetShaderInfoLog(shader, 512, nullptr, log);
		throw std::runtime_error(log);
	}
	return shader;
}

void compile(const GLuint& program, const std::string& vertexPath, const std::string& fragmentPath)
{

	const std::string vertexSource = read(vertexPath).value_or("");
	const std::string fragmentSource = read(fragmentPath).value_or("");

	const char* vertexCode = vertexSource.c_str();
	const char* fragmentCode = fragmentSource.c_str();

	const GLuint vertex = compileShader(GL_VERTEX_SHADER, vertexCode);
	const GLuint fragment = compileShader(GL_FRAGMENT_SHADER, fragmentCode);

	glAttachShader(program, vertex);
	glAttachShader(program, fragment);
	glLinkProgram(program);
	glDeleteShader(vertex);
	glDeleteShader(fragment);
}

export GLuint createProgram(const std::string& vertexPath, const std::string& fragmentPath)
{
	const GLuint program = glCreateProgram();
	compile(program, vertexPath, fragmentPath);

	return program;
}
