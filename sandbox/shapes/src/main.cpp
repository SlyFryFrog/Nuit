import nuit;

using namespace Nuit;

void _process(double delta);

Circle circle;
GLShaderProgram shader;

int main()
{
	Window window("Shapes", 600, 600);
	window.init();
	GLRenderer::init();	// Before calling GL-related code, we need to initiate the renderer

	shader.create();
	shader.compile_and_attach("shaders/basic_vert.vert", VERTEX);
	shader.compile_and_attach("shaders/basic_frag.frag", FRAGMENT);
	shader.link();

	Timer timer;

	circle.Radius = 0.5f;
	circle.Color = {1.0f, 1.0f, 0.0f, 1.0f};

	timer.start();
	while (!window.is_done())
	{
		Window::clear();

		const double delta = timer.delta();
		_process(delta);

		window.process();
	}

	return 0;
}

void _process(const double delta)
{
	circle._draw(shader);
}