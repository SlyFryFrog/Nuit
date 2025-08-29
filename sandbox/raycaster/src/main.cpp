#include <filesystem>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/glm.hpp>
#include <print>

import nuit;
import player;
import map;

using namespace Nuit;

std::shared_ptr<Window> window = std::make_shared<Window>("Raycaster", 600, 600);
GLShaderProgram shader_2d;
GLShaderProgram shader_3d;
Timer timer;
Grid grid;
Player player;
Map map;

float angle;

std::shared_ptr<std::vector<Ray>> rays = std::make_shared<std::vector<Ray>>();

void draw_left(const GLShaderProgram& shader);
void draw_right(const GLShaderProgram& shader);

int main()
{
	std::filesystem::current_path(WorkingDirectory);
	window->_init();
	GLRenderer::_init();

	map = Map(20, 20, window, rays);

	// Before linking, we must create a new shader instance and then attach our desired shaders
	shader_2d.create();
	shader_2d.compile_and_attach("shaders/2d/vertex.vert", VERTEX);
	shader_2d.compile_and_attach("shaders/2d/fragment.frag", FRAGMENT);
	shader_2d.link();

	shader_3d.create();
	shader_3d.compile_and_attach("shaders/3d/vertex.vert", VERTEX);
	shader_3d.compile_and_attach("shaders/3d/fragment.frag", FRAGMENT);
	shader_3d.link();

	// Grid from -1,-1 to 1,1 (full normalized ortho space)
	grid = Grid(glm::vec2{0, 0}, glm::vec2{20.0f, 20.0f});
	grid.generate(20, 20);
	player._init();
	player.Position = glm::vec3{10, 0, 10};

	timer.start();
	while (!window->is_done())
	{
		Window::clear();
		constexpr float rotationSpeed = 2.5f;
		const double delta = timer.delta();

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


		player._process(delta);

		rate = FOV / (static_cast<float>(window->get_width() / 2));		// Size of viewport
		rays->clear();
		rays->resize(window->get_width() / 2);

		draw_left(shader_2d);
		draw_right(shader_3d);

		window->_process();
	}

	return 0;
}

void draw_left(const GLShaderProgram& shader)
{
	// Left: top-down ortho
	constexpr float zoom = 10.0f;

	const float vpWidth = static_cast<float>(window->get_width()) / 2.0f;
	const float vpHeight = static_cast<float>(window->get_height());
	const float aspect = vpWidth / vpHeight;

	Window::set_viewport(0, 0, static_cast<int>(vpWidth), static_cast<int>(vpHeight));
	const glm::mat4 proj = glm::ortho(-zoom * aspect, zoom * aspect, -zoom, zoom, -1.0f, 1.0f);

	const glm::mat4 view = glm::translate(glm::mat4(1.0f),
										  -glm::vec3{player.Position.x, player.Position.z, 0.0f});
	constexpr glm::mat4 model = glm::mat4(1.0f);

	shader.bind();
	shader.set_uniform("uProjection", proj);
	shader.set_uniform("uView", view);
	shader.set_uniform("uModel", model);
	shader.set_uniform("uColor", glm::vec4{1.0f, 1.0f, 1.0f, 1.0f});
	grid.draw_filled(shader, generatedMap);
	player._draw(shader);

	for (int i = 0; i < rays->size(); i++)
	{
		const float a = angle + glm::radians(i * -rate); // step x degrees
		rays->at(i).Position = glm::vec2{player.Position.x, player.Position.z};
		rays->at(i).Direction = {cos(a), sin(a)};
		rays->at(i).cast(generatedMap);
		rays->at(i)._draw(shader);
	}
}

void draw_right(const GLShaderProgram& shader)
{
	map._draw(shader);
}
