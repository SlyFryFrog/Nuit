module;
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <print>
export module nuit:window;

import :input_manager;

namespace Nuit
{
	export class Window
	{
		GLFWwindow* m_window{};
	public:
		int Width{600}, Height{600};

	public:
		Window() = default;

		Window(const int width, const int height) : Width(width), Height(height)
		{
		}

		~Window() = default;

		void _init()
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

			m_window = glfwCreateWindow(Width, Height, "Hello World!", nullptr, nullptr);
			if (!m_window)
			{
				glfwTerminate();
				std::println(std::cerr, "Failed to create GLFW window.");
				return;
			}

			glfwGetFramebufferSize(m_window, &Width, &Height);
			glfwSetFramebufferSizeCallback(m_window, _frame_buffer_size_callback);
			glfwSetKeyCallback(m_window, InputManager::_process_input_callback);
			glfwSetCursorPosCallback(m_window, InputManager::_process_mouse_callback);
			glfwSetWindowUserPointer(m_window, this);

			glfwMakeContextCurrent(m_window);
		}

		void _process() const
		{
			glfwSwapBuffers(m_window);
			glfwPollEvents();
			InputManager::_process();
		}

		static void clear()
		{
			glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		}

		[[nodiscard]] bool is_done() const
		{
			return glfwWindowShouldClose(m_window);
		}

		[[nodiscard]] GLFWwindow* get_glfw_window() const
		{
			return m_window;
		}

		static void set_viewport(const int x, const int y, const int width, const int height)
		{
			glViewport(x, y, width, height);
		}

		void reset_viewport_to_window() const
		{
			glViewport(0, 0, Width, Height);
		}

		static void _frame_buffer_size_callback(GLFWwindow* window, const int width, const int height)
		{
			auto impl = (Window*)glfwGetWindowUserPointer(window);
			impl->Width = width;
			impl->Height = height;

			glViewport(0, 0, width, height);
		}
	};
}
