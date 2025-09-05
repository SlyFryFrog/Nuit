#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/glm.hpp>
#include <filesystem>

import nuit;
import camera;

using namespace Nuit;


void process_input();
void reload_shaders();

Window window("Meshes", 600, 600);
MeshLoader mesh{};
MeshLoader mesh2{};
GLShaderProgram shader;
Camera camera;

int main()
{
	std::filesystem::current_path(WorkingDirectory);
	window.init();
	GLRenderer::init();	// Before calling GL-related code, we need to initiate the renderer

	GLRenderer::enable_depth_testing(true);

	reload_shaders();
	mesh.load("assets/banjo_frog/banjo_frog.obj");
	mesh2.load("assets/low_poly_tree/Lowpoly_tree_sample.obj");

	Timer timer;
	timer.start();

	float angle = 0.0;

	while (!window.is_done())
	{
		Window::clear();	// Must first clear the screen
		process_input();
		camera.set_aspect_ratio(window.get_frame_buffer_width(), window.get_frame_buffer_height());

		const double delta = timer.delta();
		angle += 15 * delta;

		camera._process(delta);

		// Move the frog backwards
		glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -15.0f));

		// First, rotate the model to make it sit upright
		// Second, make the model spin
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(angle), glm::vec3(0.0f, 0.0f, 1.0f));

		shader.bind();
		shader.set_uniform("uModel", model);
		shader.set_uniform("uLightPos", glm::vec3(15.0f, 15.0f, -15.0f));
		camera._draw(shader);

		mesh.draw(shader);

		// Note: we can change uniforms between draws - i.e. the light position
		model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -45.0f));
		shader.set_uniform("uModel", model);
		shader.set_uniform("uLightPos", glm::vec3(-15.0f, 15.0f, -15.0f));
		mesh2.draw(shader);

		window.process();
	}

	return 0;
}

void process_input()
{
	// Close window
	if (InputManager::is_ordered_pressed({KEY_LEFT_CONTROL, KEY_Q}))
	{
		window.set_done(true);
	}

	if (InputManager::is_ordered_pressed({KEY_LEFT_CONTROL, KEY_R}))
	{
		reload_shaders();
	}

	// Change polygon mode
	if (InputManager::is_ordered_pressed({KEY_LEFT_CONTROL, KEY_1}))
	{
		GLRenderer::set_polygon_mode(FILL);
	}
	else if (InputManager::is_ordered_pressed({KEY_LEFT_CONTROL, KEY_2}))
	{
		GLRenderer::set_polygon_mode(LINE);
	}
	else if (InputManager::is_ordered_pressed({KEY_LEFT_CONTROL, KEY_3}))
	{
		GLRenderer::set_polygon_mode(POINT);
	}
}

void reload_shaders()
{
	shader.create();
	shader.compile_and_attach("shaders/basic_tex.vert", VERTEX);
	shader.compile_and_attach("shaders/basic_tex.frag", FRAGMENT);
	shader.link();
}