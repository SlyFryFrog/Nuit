#include <filesystem>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_projection.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/glm.hpp>
#include <print>
#include <thread>

import nuit;
import nuit.extensions.imgui;
import player;
import map;
import ray_2d;

using namespace Nuit;

auto window = std::make_shared<Window>("Raycaster", 600, 600);
auto rays = std::make_shared<std::vector<Ray>>();

GLShaderProgram shader_2d;
GLShaderProgram shader_3d;
Timer timer;
Grid grid;
Player player;
Map map;

void init();
void run();
void shutdown();
void reload_shaders();
void draw_left(GLShaderProgram& shader);
void draw_right(GLShaderProgram& shader);

int main()
{
	init();
	run();
	shutdown();

	return 0;
}

void init()
{
	// Window setup and set pwd for correct pathing when accessing relative files
	std::filesystem::current_path(WorkingDirectory);
	window->init();
	init_imgui(window->get_glfw_window());

	// Renderer setup
	GLRenderer::init();
	reload_shaders();

	// Setup map and grid overlay
	map = Map(200, 200, window, rays);
	map.load_map("data/map");
	grid = Grid(glm::vec2{0, 0});
	grid.generate(generatedMap);

	player._init();
	player.Position = glm::vec3{10, 0, 10};
}

void run()
{
	timer.start();
	while (!window->is_done())
	{
		const double delta = timer.delta();

		Window::clear();
		begin_imgui_frame();

		if (InputManager::is_ordered_pressed({KEY_LEFT_CONTROL, KEY_Q}))
			window->set_done(true);
		if (InputManager::is_just_pressed(KEY_R))
			reload_shaders();
		if (InputManager::is_ordered_pressed({KEY_LEFT_CONTROL, KEY_S}))
			map.save_map("data/map");
		if (InputManager::is_ordered_pressed({KEY_LEFT_CONTROL, KEY_L}))
		{
			map.load_map("data/map");

			grid = Grid(glm::vec2{0, 0});
			grid.generate(generatedMap);
		}


		player._process(delta);

		rate = FOV / (static_cast<float>(window->get_frame_buffer_width()) / 2) *
			Step; // Size of viewport
		rays->clear();
		rays->resize(window->get_frame_buffer_width() / 2 / Step);

		draw_left(shader_2d);
		draw_right(shader_3d);

		debug_window();
		render_imgui();

		window->process();
	}
}

void shutdown()
{
	shutdown_imgui();
}

void reload_shaders()
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

void draw_left(GLShaderProgram& shader)
{
	const float vpWidth = static_cast<float>(window->get_frame_buffer_width()) / 2.0f;

	const float vpHeight = static_cast<float>(window->get_frame_buffer_height());
	const float aspect = vpWidth / vpHeight;

	Window::set_viewport(0, 0, static_cast<int>(vpWidth), static_cast<int>(vpHeight));
	const glm::mat4 proj = glm::ortho(-Zoom * aspect, Zoom * aspect, -Zoom, Zoom, -1.0f, 1.0f);

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
	const float rayStart = player.Rotation + glm::radians(FOV / 2); // We start drawing from left

	for (int i = 0; i < numThreads; ++i)
	{
		threads.emplace_back(
			[&, i]()
			{
				const int start = i * raysPerThread;
				const int end = (i == numThreads - 1) ? rays->size() : start + raysPerThread;

				for (int j = start; j < end; ++j)
				{
					// We multiply degrees by -1 to make POV turn correctly
					const float angle = rayStart + glm::radians(-j * rate); // step x degrees
					rays->at(j).Position = glm::vec2{player.Position.x, player.Position.z};
					rays->at(j).Direction = {cos(angle), sin(angle)};
					rays->at(j).cast(generatedMap);
				}
			});
	}

	// Wait for all threads to finish before proceeding
	for (auto& thread : threads)
		thread.join();

	// Draw each ray, can't call to OpenGL on multiple threads
	for (auto& i : *rays)
		i._draw(shader);


	glm::ivec2 gridPos{-1, -1}; // Invalid grid coordinate by default
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
				generatedMap.at(gridPos.y).at(gridPos.x) = 0;
			else if (InputManager::is_pressed(KEY_1))
				generatedMap.at(gridPos.y).at(gridPos.x) = 1;
			else if (InputManager::is_pressed(KEY_2))
				generatedMap.at(gridPos.y).at(gridPos.x) = 2;
			else if (InputManager::is_pressed(KEY_3))
				generatedMap.at(gridPos.y).at(gridPos.x) = 3;
		}
	}
}

void draw_right(GLShaderProgram& shader)
{
	if (InputManager::is_pressed(KEY_8))
		GLRenderer::set_polygon_mode(FILL);
	else if (InputManager::is_pressed(KEY_9))
		GLRenderer::set_polygon_mode(LINE);
	else if (InputManager::is_pressed(KEY_0))
		GLRenderer::set_polygon_mode(POINT);

	map.draw_perspective_view(shader);

	GLRenderer::set_polygon_mode(FILL);
}
