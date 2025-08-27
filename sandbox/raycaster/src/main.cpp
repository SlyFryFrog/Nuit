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

constexpr int FOV = 90;
float angle;
float rate;

std::vector<Ray> rays;

void draw_left(double delta, const GLShaderProgram& shader);
void draw_right(double delta, const GLShaderProgram& shader);

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
	grid = Grid(glm::vec2{0, 0}, glm::vec2{20.0f, 20.0f});
	grid.generate(20, 20);
	player._init();
	player.Position = glm::vec3{10, 0, 10};

	while (!window.is_done())
	{
		Window::clear();

		if (InputManager::is_ordered_pressed({KEY_LEFT_ALT, KEY_ESCAPE}))
		{
			break;
		}
		if (InputManager::is_just_pressed(KEY_R))
		{
			std::println("Reloading shaders...");
			shader_2d.create();
			shader_2d.compile_and_attach("shaders/2d/vertex.vert", VERTEX);
			shader_2d.compile_and_attach("shaders/2d/fragment.frag", FRAGMENT);
			shader_2d.link();

			shader_3d.create();
			shader_3d.compile_and_attach("shaders/3d/vertex.vert", VERTEX);
			shader_3d.compile_and_attach("shaders/3d/fragment.frag", FRAGMENT);
			shader_3d.link();
			std::println("Finished reloading shaders");
		}

		const double delta = timer.delta();
		player._process(delta);

		rate = FOV / (static_cast<float>(window.get_width()));
		rays.resize(window.get_width());

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

	const glm::mat4 view = glm::translate(glm::mat4(1.0f),
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

	for (int i = 0; i < rays.size(); i++)
	{
		const float a = angle + glm::radians(i * -rate); // step x degrees
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

	Window::set_viewport(halfWidth, 0, halfWidth, fullHeight);

	const glm::mat4 proj = glm::ortho(0.0f, static_cast<float>(halfWidth) * 2,
									  static_cast<float>(fullHeight), 0.0f, -1.0f, 1.0f);

	shader.bind();
	shader.set_uniform("uProjection", proj);
	shader.set_uniform("uView", glm::mat4(1.0f));
	shader.set_uniform("uModel", glm::mat4(1.0f));

	const float projectPlaneDistance = window.get_width() / (2 * tan(FOV / 2));

	for (int col = 0; col < rays.size(); col++)
	{
		auto& ray = rays[col];

		// Ignore rays that never hit a wall
		if (!ray.Hit)
		{
			continue;
		}

		// Calculate distance from start to end of ray.
		// Then define wall height based on the distance
		const float distance = glm::length(ray.EndPosition - ray.Position);

		const float normDistance = distance *
			cos(glm::radians(FOV / 2 - col * rate)); // Remove fisheye effect

		const float wallHeight = (WallTileSize * projectPlaneDistance) / normDistance;

		// Calculate wall top and bottom relative to OpenGL's coordinate system
		const float yTop = (static_cast<float>(fullHeight) / 2) - (wallHeight / 2);
		const float yBot = yTop + wallHeight;

		// Map column to screen-space within right viewport
		const float colX = (static_cast<float>(col) / static_cast<float>(rays.size())) *
			static_cast<float>(halfWidth) * 2;

		ray.draw_vertical_line(static_cast<int>(colX), yTop, static_cast<int>(colX), yBot);
	}
}
