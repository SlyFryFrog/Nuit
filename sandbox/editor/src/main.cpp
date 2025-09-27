#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <iostream>
#include <print>

import editor;
import nuit;

using namespace Nuit;

GLFramebuffer framebuff;
GLuint fbo, texture, rbo;
int viewportWidth = 600, viewportHeight = 600;

void init_framebuffer()
{
	glGenFramebuffers(1, &fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);

	// Create a texture to render to
	texture = create_texture(viewportWidth, viewportHeight);

	// Create a renderbuffer object for depth and stencil attachment
	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, viewportWidth, viewportHeight);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		std::println(std::cerr, "Framebuffer not complete!");
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

int main()
{
	Window window("Editor", 600, 600);
	window.init();
	GLRenderer::init();

	GLShaderProgram shader{};
	shader.create();
	shader.compile_and_attach("shaders/basic_vert.vert", VERTEX);
	shader.compile_and_attach("shaders/basic_frag.frag", FRAGMENT);
	shader.link();

	Circle circle;
	circle.Radius = 0.5f;
	circle.Color = {1.0f, 1.0f, 0.0f, 1.0f};

	framebuff.create();
	init_imgui(window.get_glfw_window());

	while (!window.is_done())
	{
		Window::clear();
		// Bind the framebuffer and render the scene to it
		framebuff.bind();
		{
			Window::set_viewport(0, 0, viewportWidth, viewportHeight);
			circle._draw(shader);
		}
		framebuff.unbind();

		// Start ImGui frame
		begin_imgui_frame();

		// Create an ImGui window for the viewport
		ImGui::Begin("Viewport");
		{
			ImVec2 size = ImGui::GetContentRegionAvail();

			// Draw image from framebuffer texture when given a valid size
			if (size.x > 0 && size.y > 0)
			{
				// Update framebuffer size if the ImGui window size changes
				if (size.x != viewportWidth || size.y != viewportHeight)
				{
					viewportWidth = size.x;
					viewportHeight = size.y;
					framebuff.update(viewportWidth, viewportHeight);
				}
				// Display the framebuffer texture in the ImGui window
				ImGui::Image(framebuff.get_texture(), size, ImVec2(0, 1), ImVec2(1, 0));
			}
		}
		ImGui::End();

		render_imgui();
		window.process();
	}

	shutdown_imgui();
	return 0;
}
