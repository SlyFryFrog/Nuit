import nuit;

using namespace Nuit;

int main()
{
	Window window("Shapes", 600, 600);
	window._init();

	GLRenderer::_init();	// Before calling GL-related code, we need to initiate the renderer


	GLShaderProgram shader{};
	shader.create();
	shader.compile_and_attach("shaders/basic_vert.vert", VERTEX);
	shader.compile_and_attach("shaders/basic_frag.frag", FRAGMENT);
	shader.link();

	Circle circle{};

	while (!window.is_done())
	{
		circle._draw(shader);

		window._process();
		Window::clear();
	}

	return 0;
}