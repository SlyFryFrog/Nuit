#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/glm.hpp>
#include <filesystem>

import nuit;

using namespace Nuit;

void process_input();

Window window("Meshes", 600, 600);
MeshLoader mesh{};
MeshLoader mesh2{};

int main()
{
	std::filesystem::current_path(WorkingDirectory);
	window.init();
	GLRenderer::init();	// Before calling GL-related code, we need to initiate the renderer

	GLRenderer::enable_depth_testing(true);

	GLShaderProgram shader{};
	shader.create();
	shader.compile_and_attach("shaders/basic_tex.vert", VERTEX);
	shader.compile_and_attach("shaders/basic_tex.frag", FRAGMENT);
	shader.link();

	mesh.load("assets/banjo_frog/banjo_frog.obj");
	mesh2.load("assets/low_poly_tree/Lowpoly_tree_sample.obj");

	Timer timer;
	timer.start();

	float angle = 0.0;

	while (!window.is_done())
	{
		Window::clear();	// Must first clear the screen
		process_input();

		const double delta = timer.delta();
		angle += 15 * delta;

		// Move the frog backwards
		glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -5.0f, -15.0f));

		// First, rotate the model to make it sit upright
		// Second, make the model spin
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(angle), glm::vec3(0.0f, 0.0f, 1.0f));

		glm::mat4 view = glm::lookAt(glm::vec3(0,0,3), glm::vec3(0,0,0), glm::vec3(0,1,0));
		glm::mat4 projection = glm::perspective(glm::radians(45.0f), static_cast<float>(window.get_width()) /window.get_height(), 0.1f, 100.0f);

		shader.bind();
		shader.set_uniform("uModel", model);
		shader.set_uniform("uView", view);
		shader.set_uniform("uProjection", projection);
		shader.set_uniform("uLightPos", glm::vec3(15.0f, 15.0f, -15.0f));

		mesh.draw(shader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -5.0f, -45.0f));
		shader.set_uniform("uModel", model);
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