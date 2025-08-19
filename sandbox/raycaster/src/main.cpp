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

glm::mat4 view;
Grid grid;
Player player;
Ray2D ray;
float angle;

void draw_left(double delta);
void draw_right(double delta);

int map[][20] = {{1, 1, 1, 1, 1, 1, 1, 1, 1, 1}, {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
				 {1, 1, 0, 0, 0, 1, 0, 0, 0, 1}, {1, 0, 0, 1, 1, 1, 0, 0, 0, 1},
				 {1, 0, 0, 0, 0, 0, 0, 0, 0, 1}, {1, 0, 0, 0, 0, 0, 1, 1, 0, 1},
				 {1, 0, 0, 0, 0, 0, 0, 1, 0, 1}, {1, 0, 1, 1, 0, 0, 0, 0, 0, 1},
				 {1, 0, 0, 0, 0, 1, 1, 0, 0, 1}, {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
				{1, 1, 1, 1, 1, 1, 1, 1, 1, 1}, {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
				 {1, 1, 0, 0, 0, 1, 0, 0, 0, 1}, {1, 0, 0, 1, 1, 1, 0, 0, 0, 1},
				 {1, 0, 0, 0, 0, 0, 0, 0, 0, 1}, {1, 0, 0, 0, 0, 0, 1, 1, 0, 1},
				 {1, 0, 0, 0, 0, 0, 0, 1, 0, 1}, {1, 0, 1, 1, 0, 0, 0, 0, 0, 1},
				 {1, 0, 0, 0, 0, 1, 1, 0, 0, 1}, {1, 1, 1, 1, 1, 1, 1, 1, 1, 1}};

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
	grid = Grid(glm::vec2{0, 0}, glm::vec2{20.0f, 20.0f});
	grid.generate(20, 10);
	player._init();

	while (!window.is_done())
	{
		Window::clear();

		if (InputManager::is_ordered_pressed({KEY_LEFT_ALT, KEY_ESCAPE}))
		{
			break;
		}

		const double delta = timer.delta();

		player._process(delta);
		draw_left(delta);
		draw_right(delta);

		window._process();
	}

	return 0;
}

void draw_left(double delta)
{
	// Left: top-down ortho
	constexpr float zoom = 10.0f;

	const float vpWidth = static_cast<float>(window.Width) / 2.0f;
	const float vpHeight = static_cast<float>(window.Height);
	const float aspect = vpWidth / vpHeight;

	Window::set_viewport(0, 0, static_cast<int>(vpWidth), static_cast<int>(vpHeight));
	const glm::mat4 proj = glm::ortho(-zoom * aspect, zoom * aspect, -zoom, zoom, -1.0f, 1.0f);

	view = glm::translate(glm::mat4(1.0f), -glm::vec3{player.Position.x, player.Position.z, 0.0f});

	constexpr float rotationSpeed = 1.5f; // radians per second
	constexpr float rayLength = 0.1f;

	if (InputManager::is_pressed(KEY_E))
	{
		angle -= rotationSpeed * delta;
	}
	if (InputManager::is_pressed(KEY_Q))
	{
		angle += rotationSpeed * delta;
	}

	// Wrap angle between 0 and 2*PI
	if (angle < 0)
	{
		angle += 2 * PI;
	}
	if (angle > 2 * PI)
	{
		angle -= 2 * PI;
	}

	ray.Position = glm::vec2{player.Position.x, player.Position.z};
	ray.Direction = {cos(angle), sin(angle)};


	shader.bind();
	shader.set_uniform("uProjection", proj);
	shader.set_uniform("uView", view);
	shader.set_uniform("uColor", glm::vec4{1.0f, 1.0f, 1.0f, 1.0f});
	grid.draw_filled(shader, map);
	player._draw(shader);
	ray.check_hit(map, grid.Size);
	ray._draw(shader);
}

void draw_right(double delta)
{
	// Right: perspective 3D
	const int halfWidth = window.Width / 2;
	const int fullHeight = window.Height;
	const float aspect = static_cast<float>(halfWidth) / static_cast<float>(fullHeight);

	Window::set_viewport(halfWidth, 0, halfWidth, fullHeight);
	const glm::mat4 proj = glm::perspective(glm::radians(60.0f), aspect, 0.1f, 100.0f);

	view = glm::lookAt(player.Position, glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
	constexpr glm::mat4 model = glm::mat4(1.0f);

	shader.bind();
	shader.set_uniform("uProjection", proj);
	shader.set_uniform("uView", view);
	shader.set_uniform("uModel", model);

	grid._draw(shader);
}
