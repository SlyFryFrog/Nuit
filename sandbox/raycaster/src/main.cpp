#include <GL/glew.h>
#include <filesystem>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/glm.hpp>

import nuit;

using namespace Nuit;

Window window;
GLShaderProgram shader;
Timer timer;

glm::mat4 persProjection;
glm::mat4 view;
glm::mat4 model;
glm::mat4 orthoProj;
glm::vec3 movement{};
Grid grid;

void draw_left();
void draw_right();

int main()
{
	std::filesystem::current_path(WorkingDirectory);

	window._init();
	GLRenderer::_init();

	shader.create();
	shader.compile_and_attach("shaders/vertexShader.vert", GL_VERTEX_SHADER);
	shader.compile_and_attach("shaders/fragmentShader.frag", GL_FRAGMENT_SHADER);
	shader.link();
	shader.use();

	timer.start();

	// Grid from -1,-1 to 1,1 (full normalized ortho space)
	grid = Grid(glm::vec2{-1.0f, -1.0f}, glm::vec2{2.0f, 2.0f});
	grid.generate(20, 20);

	while (!window.is_done())
	{
		Window::clear();

		if (InputManager::is_ordered_pressed({KEY_LEFT_SHIFT, KEY_ESCAPE}))
		{
			break;
		}

		glm::vec3 currentMovement;
		constexpr float speed = 1.0f;
		double delta = timer.delta();
		if (InputManager::is_pressed(KEY_A))
		{
			currentMovement.x -= delta * speed;
		}
		if (InputManager::is_pressed(KEY_D))
		{
			currentMovement.x += delta * speed;
		}
		if (InputManager::is_pressed(KEY_S))
		{
			currentMovement.y -= delta * speed;
		}
		if (InputManager::is_pressed(KEY_W))
		{
			currentMovement.y += delta * speed;
		}

		movement = currentMovement;

		draw_left();
		draw_right();

		window._process();
	}

	return 0;
}

void draw_left()
{
	// Left: top-down ortho
	constexpr float zoom = 0.25f;

	const float vpWidth = static_cast<float>(window.Width) / 2.0f;
	const float vpHeight = static_cast<float>(window.Height);
	const float aspect = vpWidth / vpHeight;

	glm::mat4 proj = glm::ortho(-zoom * aspect, zoom * aspect,
								-zoom, zoom,
								-1.0f, 1.0f);

	Window::set_viewport(0, 0, static_cast<int>(vpWidth), static_cast<int>(vpHeight));

	model = glm::mat4(1.0f);
	view = glm::mat4(1.0f);

	view = glm::translate(model, -movement);

	shader.use();
	shader.set_uniform("uProjection", proj);
	shader.set_uniform("uView", view);
	shader.set_uniform("uModel", model);

	grid.draw();
}

void draw_right()
{
	// Right: perspective 3D
	const int halfWidth = window.Width / 2;
	const int fullHeight = window.Height;
	const float aspect = static_cast<float>(halfWidth) / static_cast<float>(fullHeight);

	Window::set_viewport(halfWidth, 0, halfWidth, fullHeight);
	persProjection = glm::perspective(glm::radians(60.0f), aspect, 0.1f, 100.0f);

	view = glm::lookAt(glm::vec3(0, 0, 3), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
	model = glm::mat4(1.0f);

	shader.use();
	shader.set_uniform("uProjection", persProjection);
	shader.set_uniform("uView", view);
	shader.set_uniform("uModel", model);

	grid.draw();
}
