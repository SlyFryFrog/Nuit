#include <filesystem>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/glm.hpp>
#include <print>

import nuit;
import player;
import map;

using namespace Nuit;

Window window("Raycaster", 600, 600);
GLShaderProgram shader_2d;
GLShaderProgram shader_3d;
Timer timer;
Grid grid;
Player player;
float angle;
constexpr int FOV = 90;

void draw_left(double delta, const GLShaderProgram& shader);
void draw_right(double delta, const GLShaderProgram& shader);

std::array<Ray2D, FOV> rays;

int main()
{
	std::filesystem::current_path(WorkingDirectory);

	window._init();
	GLRenderer::_init();

	shader_2d.create();
	shader_2d.compile_and_attach("shaders/2d/vertex.vert", VERTEX);
	shader_2d.compile_and_attach("shaders/2d/fragment.frag", FRAGMENT);
	shader_2d.link();

	shader_3d.create();
	shader_3d.compile_and_attach("shaders/3d/vertex.vert", VERTEX);
	shader_3d.compile_and_attach("shaders/3d/fragment.frag", FRAGMENT);
	shader_3d.link();

	timer.start();

	// Grid from -1,-1 to 1,1 (full normalized ortho space)
	grid = Grid(glm::vec2{0, 0}, glm::vec2{10.0f, 10.0f});
	grid.generate(10, 10);
	player._init();
	player.Position = glm::vec3{0, 0, 0};

	while (!window.is_done())
	{
		Window::clear();

		if (InputManager::is_ordered_pressed({KEY_LEFT_ALT, KEY_ESCAPE}))
		{
			break;
		}

		const double delta = timer.delta();

		player._process(delta);

		draw_left(delta, shader_2d);
		draw_right(delta, shader_3d);

		window._process();
	}

	return 0;
}

void draw_left(const double delta, const GLShaderProgram& shader)
{
	// Left: top-down ortho
	constexpr float zoom = 10.0f;

	const float vpWidth = static_cast<float>(window.get_width()) / 2.0f;
	const float vpHeight = static_cast<float>(window.get_height());
	const float aspect = vpWidth / vpHeight;

	Window::set_viewport(0, 0, static_cast<int>(vpWidth), static_cast<int>(vpHeight));
	const glm::mat4 proj = glm::ortho(-zoom * aspect, zoom * aspect, -zoom, zoom, -1.0f, 1.0f);

	glm::mat4 view = glm::translate(glm::mat4(1.0f),
									-glm::vec3{player.Position.x, player.Position.z, 0.0f});
	constexpr glm::mat4 model = glm::mat4(1.0f);

	constexpr float rotationSpeed = 2.5f;

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

	shader.bind();
	shader.set_uniform("uProjection", proj);
	shader.set_uniform("uView", view);
	shader.set_uniform("uModel", model);
	shader.set_uniform("uColor", glm::vec4{1.0f, 1.0f, 1.0f, 1.0f});
	grid.draw_filled(shader, map);
	player._draw(shader);

	for (int i = 0; i < rays.size() / 2; i++)
	{
		const float a = angle + i * (glm::radians(1.0f)); // step x degrees
		rays[i].Position = glm::vec2{player.Position.x, player.Position.z};
		rays[i].Direction = {cos(a), sin(a)};
		rays[i].cast(map);
		rays[i]._draw(shader);
	}

	for (int i = rays.size(); i >=rays.size() / 2; i--)
	{
		const float a = angle + i * (glm::radians(1.0f)); // step x degrees
		rays[i].Position = glm::vec2{player.Position.x, player.Position.z};
		rays[i].Direction = {cos(a), sin(a)};
		rays[i].cast(map);
		rays[i]._draw(shader);
	}

}

void draw_right(const double delta, const GLShaderProgram& shader)
{
	const int halfWidth = window.get_width() / 2;
	const int fullHeight = window.get_height();
	const float aspect = static_cast<float>(halfWidth) / fullHeight;

	Window::set_viewport(halfWidth, 0, halfWidth, fullHeight);
	const glm::mat4 proj = glm::perspective(glm::radians((float)FOV), aspect, 0.1f, 100.0f);

	glm::mat4 view = glm::lookAt(player.Position,
								 player.Position + glm::vec3{cos(angle), 0, sin(angle)},
								 glm::vec3{0, 1, 0});

	shader.bind();
	shader.set_uniform("uProjection", proj);
	shader.set_uniform("uView", view);
	shader.set_uniform("uModel", glm::mat4(1.0f));

	for (const auto& ray : rays)
	{
		// Only draw rays that hit a wall
		if (ray.Hit)
		{
			const float distance = glm::length(glm::vec2(ray.EndPosition.x - player.Position.x,
														 ray.EndPosition.y - player.Position.y));
			draw_vertical_line(ray.EndPosition, distance, window.get_height(), shader);
		}
	}
}
