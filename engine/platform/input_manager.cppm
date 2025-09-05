module;
#include <GLFW/glfw3.h>
#include <glm/vec2.hpp>
#include <initializer_list>
#include <map>
#include <memory>
#include <queue>
export module nuit:input_manager;

export import :input_event;
export import :input_map;

namespace Nuit
{
	/**
	 * @note On macOS, there is a limitation in the way key presses are handled. When capturing a
	 * recording, it acts as if a key is being held down. This causes issues with checking the order
	 * and just_pressed statuses.
	 */
	export class InputManager
	{
		static inline std::queue<std::shared_ptr<InputEvent>> m_eventQueue; // Put events in order
		static inline std::queue<std::shared_ptr<InputEvent>> m_recentQueue;
		static inline std::map<Key, std::shared_ptr<InputEvent>> m_events; // Register events for quick
																	// access

		static inline double m_prevMouseX;
		static inline double m_prevMouseY;
		static inline bool m_firstMouse = true;
		static inline double m_deltaX;
		static inline double m_deltaY;

	public:
		/**
		 * @return std::map<Key, std::shared_ptr<InputEvent>> Map of Key, InputEvent pairs.
		 */
		static std::map<Key, std::shared_ptr<InputEvent>> get_events()
		{
			return m_events;
		}

		static void _process_input_callback(GLFWwindow* window, int key, int scancode, int action,
											int mods);

		/**
		 * @brief Internally called callback function for when mouse movement occurs.
		 *
		 * @param window Window instance calling callback.
		 * @param xposIn Mouse in position X.
		 * @param yposIn Mouse in position Y.
		 */
		static void _process_mouse_callback(GLFWwindow* window, double xposIn, double yposIn);

		/**
		 * @brief Processes and updates Input states and events. Is called internally by Window.
		 */
		static void _process();

		static bool is_pressed(Key key);

		static bool is_just_pressed(Key key);

		static bool is_just_released(Key key);

		static bool is_released(Key key);

		/**
		 * @brief Checks if keys are pressed in a specific order.
		 *
		 * @param keys Order of keys being pressed.
		 * @return true Keys are pressed in the specific order.
		 * @return false Either not all keys are pressed or some key is pressed that is not in the
		 * keys.
		 */
		static bool is_ordered_pressed(const std::initializer_list<Key>& keys);
		static bool is_ordered_pressed(const std::vector<Key>& keys);

		static bool is_ordered_just_pressed(const std::initializer_list<Key>& keys);
		static bool is_ordered_just_pressed(const std::vector<Key>& keys);

		static glm::vec2 get_mouse_position()
		{
			return {m_prevMouseX, m_prevMouseY};
		}

		static glm::vec2 get_mouse_delta()
		{
			return {m_deltaX, m_deltaY};
		}

	private:
		/**
		 * @brief Clear all elements that are not currently pressed from recent queue.
		 */
		static void clear_recent_queue();
	};
} // namespace Lys