#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <ostream>
#include <filesystem>

import nuit;
import shader;

using namespace Nuit;

Window window;
GLuint shader;

Timer timer;

void draw_left();
void draw_right();

int main()
{
	std::filesystem::current_path(WorkingDirectory);

	window._init();
	GLRenderer::_init();

	shader = createProgram("shaders/vertexShader.vert", "shaders/fragmentShader.frag");
	glUseProgram(shader);


	timer.start();

	while (!window.is_done())
	{
		Window::clear();

		if (InputManager::is_ordered_pressed({KEY_LEFT_SHIFT, KEY_ESCAPE}))
		{
			break;
		}

		window._process();
	}

	return 0;
}


void draw_left()
{
	// Draw left top-down view
	Window::set_viewport(0, 0, window.Width / 2, window.Height);
	glUseProgram(shader);
	glUniform2f(glGetUniformLocation(shader, "uOffset"), 0.0f, 0.0f);
}

void draw_right()
{
	Window::set_viewport(window.Width / 2, 0, window.Width, window.Height);
}