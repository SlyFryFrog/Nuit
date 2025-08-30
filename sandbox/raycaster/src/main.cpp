#include <filesystem>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_projection.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/glm.hpp>
#include <iostream>
#include <print>
#include <thread>

import nuit;
import player;
import map;

using namespace Nuit;

auto window = std::make_shared<Window>("Raycaster", 600, 600);
auto rays = std::make_shared<std::vector<Ray>>();

GLShaderProgram shader_2d;
GLShaderProgram shader_3d;
Timer timer;
Grid grid;
Player player;
Map map;
constexpr auto CellSize = glm::vec2{20.0f, 20.0f};
float angle;


void draw_left(const GLShaderProgram& shader);
void draw_right(const GLShaderProgram& shader);

glm::ivec2 get_mouse_grid_position(glm::vec4 worldPos);

int main()
{
	std::filesystem::current_path(WorkingDirectory);
	window->init();
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

	grid = Grid(glm::vec2{0, 0}, CellSize);
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

		if (InputManager::is_ordered_pressed({KEY_LEFT_CONTROL, KEY_L}))
		{
			map.load_map("data/map");
		}

		if (InputManager::is_ordered_pressed({KEY_LEFT_CONTROL, KEY_S}))
		{
			map.save_map("data/map");
		}

		if (InputManager::is_pressed(KEY_E))
		{
			angle -= rotationSpeed * static_cast<float>(delta);
		}
		if (InputManager::is_pressed(KEY_Q))
		{
			angle += rotationSpeed * static_cast<float>(delta);
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

		rate = FOV / (static_cast<float>(window->get_frame_buffer_width() / 2)); // Size of viewport
		rays->clear();
		rays->resize(window->get_frame_buffer_width() / 2);

		draw_left(shader_2d);
		draw_right(shader_3d);

		window->process();
	}

	return 0;
}

void draw_left(const GLShaderProgram& shader)
{
	// Left: top-down ortho
	constexpr float zoom = 10.0f;

	const float vpWidth = static_cast<float>(window->get_frame_buffer_width()) / 2.0f;

	const float vpHeight = static_cast<float>(window->get_frame_buffer_height());
	const float aspect = vpWidth / vpHeight;

	Window::set_viewport(0, 0, static_cast<int>(vpWidth), static_cast<int>(vpHeight));
	const glm::mat4 proj = glm::ortho(-zoom * aspect, zoom * aspect, -zoom, zoom, -1.0f, 1.0f);

	const glm::mat4 view = glm::translate(glm::mat4(1.0f),
										  -glm::vec3{player.Position.x, player.Position.z, 0.0f});

	shader.bind();
	shader.set_uniform("uProjection", proj);
	shader.set_uniform("uView", view);
	shader.set_uniform("uModel", glm::mat4(1.0f));
	shader.set_uniform("uColor", glm::vec4{1.0f, 1.0f, 1.0f, 1.0f});

	grid.draw_filled(shader, generatedMap);
	player._draw(shader);


	// Split path for casting rays onto different threads
	std::vector<std::thread> threads;
	const unsigned int numThreads = std::thread::hardware_concurrency();
	threads.reserve(numThreads);

	const int raysPerThread = rays->size() / numThreads;

	for (int i = 0; i < numThreads; ++i)
	{
		threads.emplace_back(
			[&, i]()
			{
				const int start = i * raysPerThread;
				const int end = (i == numThreads - 1) ? rays->size() : start + raysPerThread;

				for (int j = start; j < end; ++j)
				{
					const float a = angle + glm::radians(j * -rate); // step x degrees
					rays->at(j).Position = glm::vec2{player.Position.x, player.Position.z};
					rays->at(j).Direction = {cos(a), sin(a)};
					rays->at(j).cast(generatedMap);
				}
			});
	}

	// Wait for all threads to finish before proceeding
	for (auto& thread : threads)
	{
		thread.join();
	}

	// Draw each ray, can't call to OpenGL on multiple threads
	for (int i = 0; i < rays->size(); i++)
	{
		rays->at(i)._draw(shader);
	}


	glm::ivec2 gridPos{-1, -1};	// Invalid grid coordinate by default
	if (const glm::vec2 mousePos = InputManager::get_mouse_position();
		mousePos.x < window->get_width() / 2)
	{
		// Convert mouse coordinate to grid position
		const glm::vec4 viewport(0.0f, 0.0f, window->get_width() / 2.0f, window->get_height());
		const glm::vec3 screenPos(mousePos.x, window->get_height() - mousePos.y,
								  0.0f); // Y needs reversed
		const glm::vec3 worldPos = glm::unProject(screenPos, view, proj, viewport);

		gridPos = {static_cast<int>(std::floor(worldPos.x)),
				   static_cast<int>(std::floor(worldPos.y))};
	}

	// Since the map is a fixed size, must check if the gridPos is in-bounds
	// Based on the key pressed, update the integer at the gridPos
	if (gridPos.y >= 0 && gridPos.y < generatedMap.size())
	{
		if (gridPos.x >= 0 && gridPos.x < generatedMap.at(gridPos.y).size())
		{
			if (InputManager::is_pressed(KEY_SPACE))
			{
				generatedMap.at(gridPos.y).at(gridPos.x) = 0;
			}
			else if (InputManager::is_pressed(KEY_1))
			{
				generatedMap.at(gridPos.y).at(gridPos.x) = 1;
			}
			else if (InputManager::is_pressed(KEY_2))
			{
				generatedMap.at(gridPos.y).at(gridPos.x) = 2;
			}
			else if (InputManager::is_pressed(KEY_3))
			{
				generatedMap.at(gridPos.y).at(gridPos.x) = 3;
			}
		}
	}
}

void draw_right(const GLShaderProgram& shader)
{
	map.draw_perspective_view(shader);
}
