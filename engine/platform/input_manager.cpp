module;
#include <GLFW/glfw3.h>
#include <initializer_list>
#include <map>
#include <memory>
#include <queue>
module nuit;

namespace Nuit
{
	void InputManager::_process_input_callback(GLFWwindow* window, const int key,
											   const int scancode, const int action, const int mods)
	{
		// Ignore repeated events as they are updated in _process() when called externally
		if (action == GLFW_REPEAT)
		{
			return;
		}

		const Key lysKey = static_cast<Key>(key);
		InputState state;

		if (action == GLFW_PRESS && !m_events.contains(lysKey))
		{
			state = JUST_PRESSED;
		}
		else if (action == GLFW_RELEASE)
		{
			state = JUST_RELEASED;
		}

		const auto event = std::make_shared<InputEvent>(lysKey, state);

		// Add to recent queue if just pressed
		// Used for handling combinations of keys being pressed
		m_recentQueue.push(event);

		m_eventQueue.push(event);
		m_events[lysKey] = event;
	}

	void InputManager::_process_mouse_callback(GLFWwindow* window, const double xposIn,
											   const double yposIn)
	{
		if (m_firstMouse)
		{
			m_prevMouseX = xposIn;
			m_prevMouseY = yposIn;
			m_firstMouse = false;
		}

		double deltaX = xposIn - m_prevMouseX;
		double deltaY = m_prevMouseY - yposIn;

		m_prevMouseX = xposIn;
		m_prevMouseY = yposIn;

		m_deltaX = deltaX;
		m_deltaY = deltaY;
	}

	/**
	 * @brief Processes and updates Input states and events.
	 */
	void InputManager::_process()
	{
		// Handle all events in the queue by iterating through the queue and popping all events.
		// After popping an event from the eventQueue, the state of the key in the map is
		// either:
		// 1. Updated to reflect it being "PRESSED"
		// 2. Erased to reflect it being "RELEASED"
		while (!m_eventQueue.empty())
		{
			// Extract event from the eventQueue
			const std::shared_ptr<InputEvent> event = m_eventQueue.front();
			m_eventQueue.pop();

			Key key = event->get_key();
			const InputState state = event->get_state();

			// Update the map depending on the event state
			if (state == JUST_PRESSED)
			{
				m_events[key]->set_state(PRESSED);
			}
			else if (state == JUST_RELEASED)
			{
				m_events.erase(key); // Key is released, remove from active map
			}
		}

		clear_recent_queue();
	}

	bool InputManager::is_pressed(const Key key)
	{
		if (!m_events.contains(key))
		{
			return false;
		}

		return m_events[key]->is_pressed();
	}

	bool InputManager::is_just_pressed(const Key key)
	{
		if (!m_events.contains(key))
		{
			return false;
		}

		return m_events[key]->is_just_pressed();
	}

	bool InputManager::is_just_released(const Key key)
	{
		if (!m_events.contains(key))
		{
			return false;
		}

		return m_events[key]->is_just_released();
	}

	bool InputManager::is_released(const Key key)
	{
		return !m_events.contains(key);
	}

	bool InputManager::is_ordered_pressed(const std::initializer_list<Key>& keys)
	{
		return is_ordered_pressed(std::vector<Key>(keys));
	}

	bool InputManager::is_ordered_pressed(const std::vector<Key>& keys)
	{
		std::queue<std::shared_ptr<InputEvent>> tempRecentQueue = m_recentQueue;
		auto it = keys.begin();

		while (it != keys.end() && !tempRecentQueue.empty())
		{
			std::shared_ptr<InputEvent> currentEvent = tempRecentQueue.front();
			if (currentEvent->get_key() == *it &&
				(currentEvent->is_pressed() || currentEvent->is_just_pressed()))
			{
				tempRecentQueue.pop();
				it++;
			}
			else if (!currentEvent->is_just_pressed())
			{
				tempRecentQueue.pop(); // Skip non-JUST_PRESSED events
			}
			else
			{
				return false; // Sequence doesn't match
			}
		}

		// If we've gone through all the keys in the desired sequence,
		// now check if all those keys are currently being held down.
		if (it == keys.end())
		{
			for (const Key& key : keys)
			{
				if (!m_events.contains(key) ||
					!(m_events[key]->is_pressed() || m_events[key]->is_just_pressed()))
				{
					return false; // Not all keys are held
				}
			}

			// Iterate through each Key, InputEvent
			for (const auto& [key, event] : m_events)
			{
				bool inCombo = false;
				for (const Key& comboKey : keys)
				{
					if (key == comboKey)
					{
						inCombo = true;
						break;
					}
				}

				if (!inCombo && event->is_pressed())
				{
					return false; // Another key is pressed
				}
			}

			return true; // Sequence matched and all keys are held
		}

		return false; // The recent queue didn't contain the full sequence of presses
	}


	void InputManager::clear_recent_queue()
	{
		// Clear recentQueue of all !is_pressed elements
		std::queue<std::shared_ptr<InputEvent>> tempQueue;
		while (!m_recentQueue.empty())
		{
			std::shared_ptr<InputEvent> event = m_recentQueue.front();
			m_recentQueue.pop();

			// Copy element to other queue
			if (is_pressed(event->get_key())) // Checks if event contains key and is pressed
			{
				tempQueue.emplace(event);
			}
		}

		// Update queue
		m_recentQueue = tempQueue;
	}
} // namespace Nuit
