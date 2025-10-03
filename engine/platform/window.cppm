module;
#include <GLFW/glfw3.h>
#include <string>
export module nuit:window;
namespace Nuit
{
	/**
	 * @brief Manages a GLFW window and its OpenGL context.
	 *
	 * The `Window` class encapsulates the creation, configuration, and management of a GLFW window,
	 * including OpenGL context initialization, window resizing, viewport management, and event
	 * processing. It is designed to be the main interface for window and OpenGL context management
	 * in a rendering application.
	 */
	export class Window
	{
		GLFWwindow* m_window{};					///< GLFW window handle.
		std::string m_title{"Untitled Window"}; ///< Title of the window.
		int m_width{600}, m_height{600};		///< Dimensions of the window in pixels.
		int m_fbWidth{}, m_fbHeight{};			///< Dimensions of the framebuffer in pixels.

	public:
		/**
		 * @brief Constructs a `Window` with default values.
		 */
		Window() = default;

		/**
		 * @brief Constructs a `Window` with the specified title and dimensions.
		 *
		 * @param title The title of the window.
		 * @param width The width of the window in pixels.
		 * @param height The height of the window in pixels.
		 */
		Window(std::string title, const int width, const int height) :
			m_title(std::move(title)), m_width(width), m_height(height)
		{
		}

		/**
		 * @brief Constructs a `Window` with the specified dimensions.
		 *
		 * @param width The width of the window in pixels.
		 * @param height The height of the window in pixels.
		 */
		Window(const int width, const int height) : m_width(width), m_height(height)
		{
		}

		/**
		 * @brief Default destructor.
		 */
		~Window() = default;

		/**
		 * @brief Initializes the window and OpenGL context with the specified GL version.
		 *
		 * @param majorVersion Target OpenGL major version. Default is 4.
		 * @param minorVersion Target OpenGL minor version. Default is 1.
		 */
		void init(int majorVersion = 4, int minorVersion = 1);

		/**
		 * @brief Processes window events and swaps buffers.
		 *
		 * This method should be called once per frame to handle input events and update the window.
		 */
		void process() const;

		/**
		 * @brief Clears the window and buffer bits.
		 *
		 * Sets the clear color and clears the color and depth buffers.
		 */
		static void clear();

		/**
		 * @brief Sets the window's close flag.
		 *
		 * @param done If true, the window will close on the next event loop iteration.
		 */
		void set_done(bool done);

		/**
		 * @brief Checks if the window is marked for closure.
		 *
		 * @return true If the window should close.
		 * @return false If the window should remain open.
		 */
		[[nodiscard]] bool is_done() const;

		/**
		 * @brief Gets the GLFW window handle.
		 *
		 * @return GLFWwindow* The GLFW window handle.
		 */
		[[nodiscard]] GLFWwindow* get_glfw_window() const;

		/**
		 * @brief Sets the title of the window.
		 *
		 * @param title The new title of the window.
		 */
		void set_title(const std::string& title);

		/**
		 * @brief Gets the title of the window.
		 *
		 * @return std::string The current title of the window.
		 */
		[[nodiscard]] std::string get_title() const;

		/**
		 * @brief Sets the dimensions of the window.
		 *
		 * @param width The new width of the window in pixels.
		 * @param height The new height of the window in pixels.
		 */
		void set_size(int width, int height);

		/**
		 * @brief Gets the width of the window.
		 *
		 * @return int The width of the window in pixels.
		 */
		[[nodiscard]] int get_width() const;

		/**
		 * @brief Gets the height of the window.
		 *
		 * @return int The height of the window in pixels.
		 */
		[[nodiscard]] int get_height() const;

		/**
		 * @brief Gets the width of the framebuffer.
		 *
		 * @return int The width of the framebuffer in pixels.
		 */
		[[nodiscard]] int get_frame_buffer_width() const;

		/**
		 * @brief Gets the height of the framebuffer.
		 *
		 * @return int The height of the framebuffer in pixels.
		 */
		[[nodiscard]] int get_frame_buffer_height() const;

		/**
		 * @brief Sets the OpenGL viewport.
		 *
		 * @param x The lower left corner of the viewport.
		 * @param y The lower left corner of the viewport.
		 * @param width The width of the viewport.
		 * @param height The height of the viewport.
		 */
		static void set_viewport(int x, int y, int width, int height);

		/**
		 * @brief Resets the OpenGL viewport to match the window dimensions.
		 */
		void reset_viewport_to_window() const;

		/**
		 * @brief Switches the window to windowed mode with the specified dimensions.
		 *
		 * @param width The width of the window in pixels.
		 * @param height The height of the window in pixels.
		 */
		void set_windowed(int width, int height);

		/**
		 * @brief Switches the window to fullscreen mode.
		 */
		void set_fullscreen();

		/**
		 * @brief Configures default GLFW window callbacks and settings.
		 */
		void set_glfw_window_defaults();

		/**
		 * @brief Internal callback for framebuffer resize events.
		 *
		 * @param window The GLFW window instance.
		 * @param width The new width of the framebuffer.
		 * @param height The new height of the framebuffer.
		 */
		static void _frame_buffer_size_callback(GLFWwindow* window, int width, int height);
	};
} // namespace Nuit
