module;
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <print>
#include <utility>
export module nuit:window;

import :input_manager;

namespace Nuit
{
	export class Window
	{
		GLFWwindow* m_window{};
		std::string m_title{"Untitled Window"};
		int m_width{600}, m_height{600};

	public:
		Window() = default;

		Window(std::string title, const int width, const int height) : m_title(std::move(title)), m_width(width), m_height(height)
		{
		}

		Window(const int width, const int height) : m_width(width), m_height(height)
		{
		}

		~Window() = default;

		void init()
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

			glfwGetFramebufferSize(m_window, &m_width, &m_height);
			glfwSetFramebufferSizeCallback(m_window, _frame_buffer_size_callback);
			glfwSetKeyCallback(m_window, InputManager::_process_input_callback);
			glfwSetCursorPosCallback(m_window, InputManager::_process_mouse_callback);
			glfwSetWindowUserPointer(m_window, this);

			glfwMakeContextCurrent(m_window);
		}

		void process() const
		{
			InputManager::_process();
			glfwSwapBuffers(m_window);
			glfwPollEvents();
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

		void set_title(const std::string& title)
		{
			m_title = title;
			glfwSetWindowTitle(m_window, title.c_str());
		}

		[[nodiscard]] std::string get_title() const
		{
			return m_title;
		}

		void set_size(const int width, const int height)
		{
			m_width = width;
			m_height = height;

			glfwSetWindowSize(m_window, m_width, m_height);
		}

		[[nodiscard]] int get_width() const
		{
			return m_width;
		}

		[[nodiscard]] int get_height() const
		{
			return m_height;
		}

		static void set_viewport(const int x, const int y, const int width, const int height)
		{
			glViewport(x, y, width, height);
		}

		void reset_viewport_to_window() const
		{
			glViewport(0, 0, m_width, m_height);
		}

		static void _frame_buffer_size_callback(GLFWwindow* window, const int width, const int height)
		{
			const auto impl = static_cast<Window*>(glfwGetWindowUserPointer(window));
			impl->m_width = width;
			impl->m_height = height;

			glViewport(0, 0, width, height);
		}
	};
}
