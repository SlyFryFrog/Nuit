#include <filesystem>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/glm.hpp>

import nuit;
import player;

using namespace Nuit;

Window window;
GLShaderProgram shader;
Timer timer;

glm::mat4 persProjection;
glm::mat4 view;
glm::mat4 model;
glm::mat4 orthoProj;
Grid grid;
Player player;

void draw_left();
void draw_right();

int main()
{
	std::filesystem::current_path(WorkingDirectory);

	window._init();
	GLRenderer::_init();

	shader.create();
	shader.compile_and_attach("shaders/vertexShader.vert", VERTEX);
	shader.compile_and_attach("shaders/fragmentShader.frag", FRAGMENT);
	shader.link();
	shader.bind();

	timer.start();

	// Grid from -1,-1 to 1,1 (full normalized ortho space)
	grid = Grid(glm::vec2{-1.0f, -1.0f}, glm::vec2{2.0f, 2.0f});
	grid.generate(20, 20);
	player._init();

	while (!window.is_done())
	{
		Window::clear();

		if (InputManager::is_ordered_pressed({KEY_LEFT_SHIFT, KEY_ESCAPE}))
		{
			break;
		}

		glm::vec3 currentMovement;
		constexpr float speed = 0.1f;
		const double delta = timer.delta();

		player._process(delta);
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
	view = glm::translate(model, -glm::vec3{player.Position.x, player.Position.z, 0.0f});

	shader.bind();
	shader.set_uniform("uProjection", proj);
	shader.set_uniform("uView", view);
	shader.set_uniform("uModel", model);

	grid.draw();
	player._draw(shader);
}

void draw_right()
{
	// Right: perspective 3D
	const int halfWidth = window.Width / 2;
	const int fullHeight = window.Height;
	const float aspect = static_cast<float>(halfWidth) / static_cast<float>(fullHeight);

	Window::set_viewport(halfWidth, 0, halfWidth, fullHeight);
	persProjection = glm::perspective(glm::radians(60.0f), aspect, 0.1f, 100.0f);

	view = glm::lookAt(player.Position, glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
	model = glm::mat4(1.0f);

	shader.bind();
	shader.set_uniform("uProjection", persProjection);
	shader.set_uniform("uView", view);
	shader.set_uniform("uModel", model);

	grid.draw();
}
