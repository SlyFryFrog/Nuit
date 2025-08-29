module;
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <print>
module nuit;

namespace Nuit
{
	void Window::init()
	{
		if (!glfwInit())
		{
			std::println(std::cerr, "Failed to initialize GLFW.");
			return;
		}

		// Support latest version available on macOS
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_VISIBLE, GLFW_TRUE);

		// Required by macOS
#ifdef __APPLE__
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

		m_window = glfwCreateWindow(m_width, m_height, "Hello World!", nullptr, nullptr);
		if (!m_window)
		{
			glfwTerminate();
			std::println(std::cerr, "Failed to create GLFW window.");
			return;
		}

		set_title(m_title);

		glfwGetWindowSize(m_window, &m_width, &m_height);
		glfwGetFramebufferSize(m_window, &m_fbWidth, &m_fbHeight);
		glfwSetFramebufferSizeCallback(m_window, _frame_buffer_size_callback);
		glfwSetKeyCallback(m_window, InputManager::_process_input_callback);
		glfwSetCursorPosCallback(m_window, InputManager::_process_mouse_callback);
		glfwSetWindowUserPointer(m_window, this);

		glfwMakeContextCurrent(m_window);
		glViewport(0, 0, m_fbWidth, m_fbHeight); // use framebuffer size for OpenGL
	}

	void Window::process() const
	{
		InputManager::_process();
		glfwSwapBuffers(m_window);
		glfwPollEvents();
	}

	void Window::clear()
	{
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	bool Window::is_done() const
	{
		return glfwWindowShouldClose(m_window);
	}

	GLFWwindow* Window::get_glfw_window() const
	{
		return m_window;
	}

	void Window::set_title(const std::string& title)
	{
		m_title = title;
		glfwSetWindowTitle(m_window, title.c_str());
	}

	std::string Window::get_title() const
	{
		return m_title;
	}

	void Window::set_size(const int width, const int height)
	{
		m_width = width;
		m_height = height;

		glfwSetWindowSize(m_window, m_width, m_height);
	}

	int Window::get_width() const
	{
		return m_width;
	}

	int Window::get_height() const
	{
		return m_height;
	}

	int Window::get_frame_buffer_width() const
	{
		return m_fbWidth;
	}

	int Window::get_frame_buffer_height() const
	{
		return m_fbHeight;
	}

	void Window::set_viewport(const int x, const int y, const int width, const int height)
	{
		glViewport(x, y, width, height);
	}

	void Window::reset_viewport_to_window() const
	{
		glViewport(0, 0, m_width, m_height);
	}

	void Window::_frame_buffer_size_callback(GLFWwindow* window, const int width,
													const int height)
	{
		const auto impl = static_cast<Window*>(glfwGetWindowUserPointer(window));
		impl->m_fbWidth = width;
		impl->m_fbHeight = height;

		glfwGetWindowSize(window, &impl->m_width, &impl->m_height);
		glViewport(0, 0, width, height);
	}
} // namespace Nuit
