module;
#include <GLFW/glfw3.h>
#include <iostream>
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
		int m_fbWidth{}, m_fbHeight{};

	public:
		Window() = default;

		Window(std::string title, const int width, const int height) : m_title(std::move(title)), m_width(width), m_height(height)
		{
		}

		Window(const int width, const int height) : m_width(width), m_height(height)
		{
		}

		~Window() = default;

		void init();

		void process() const;

		static void clear();

		[[nodiscard]] bool is_done() const;

		[[nodiscard]] GLFWwindow* get_glfw_window() const;

		void set_title(const std::string& title);

		[[nodiscard]] std::string get_title() const;

		void set_size(int width, int height);

		[[nodiscard]] int get_width() const;
		[[nodiscard]] int get_height() const;

		[[nodiscard]] int get_frame_buffer_width() const;
		[[nodiscard]] int get_frame_buffer_height() const;

		static void set_viewport(int x, int y, int width, int height);

		void reset_viewport_to_window() const;

		static void _frame_buffer_size_callback(GLFWwindow* window, int width, int height);
	};
}
