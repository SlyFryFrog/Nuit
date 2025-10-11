#include <imgui.h>
#include <iostream>

import editor;
import nuit;
import nuit.extensions.imgui;

using namespace Nuit;

Window window("Editor", 600, 600);
GLFramebuffer framebuff;
int viewportWidth = 600, viewportHeight = 600;

int main()
{
	window.init();
	GLRenderer::init();

	framebuff.create();
	init_imgui(window.get_glfw_window());

	GLShaderProgram shader{};
	shader.create();
	shader.compile_and_attach("shaders/basic_vert.vert", VERTEX);
	shader.compile_and_attach("shaders/basic_frag.frag", FRAGMENT);
	shader.link();

	Circle circle{};
	circle.Radius = 0.5f;
	circle.Color = {0.0f, 1.0f, 0.0f, 1.0f};

	while (!window.is_done())
	{
		Window::clear();
		// Bind the framebuffer and render the scene to it
		framebuff.bind();
		{
			Window::set_viewport(0, 0, viewportWidth, viewportHeight);
			circle._draw(shader);
		}
		GLFramebuffer::unbind();

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
