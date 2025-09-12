module;
#include <GLFW/glfw3.h>
#include <string>
export module nuit:window;

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

		/**
		 * @brief Initialize window with the specified GL version for GLFW to use.
		 *
		 * @param majorVersion Target OpenGL major version. Default is 4.
		 * @param minorVersion Target OpenGL minor version. Default is 1.
		 */
		void init(int majorVersion = 4, int minorVersion = 1);

		void process() const;

		/**
		 * @brief Clears the window and buffer bits.
		 */
		static void clear();

		void set_done(bool done);

		[[nodiscard]] bool is_done() const;

		[[nodiscard]] GLFWwindow* get_glfw_window() const;

		void set_title(const std::string& title);

		[[nodiscard]] std::string get_title() const;

		/**
		 * @brief Sets the dimensions of the window.
		 *
		 * @param width Width of window.
		 * @param height Height of window.
		 */
		void set_size(int width, int height);

		[[nodiscard]] int get_width() const;
		[[nodiscard]] int get_height() const;

		[[nodiscard]] int get_frame_buffer_width() const;
		[[nodiscard]] int get_frame_buffer_height() const;

		static void set_viewport(int x, int y, int width, int height);

		void reset_viewport_to_window() const;

		void set_windowed(int width, int height);
		void set_fullscreen();

		void set_glfw_window_defaults();

		static void _frame_buffer_size_callback(GLFWwindow* window, int width, int height);
	};
}
