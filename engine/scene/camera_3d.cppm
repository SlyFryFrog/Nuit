module;
#include <glm/glm.hpp>
export module nuit:camera_3d;

import :node_3d;

namespace Nuit
{
	export class Camera3D : public Node3D
	{
	protected:
		float m_aspectRatio{1};
		bool m_firstMouse = true;
		glm::vec2 m_lastMousePos{};
		glm::vec3 m_up = glm::vec3(0.0f, 1.0f, 0.0f);

	public:
		float FOV{45.0f};
		float Near{0.1f};
		float Far{1000.0f};

	public:
		Camera3D() = default;
		~Camera3D() override = default;

		void look_at(const glm::vec3& point);

		[[nodiscard]] glm::mat4 get_view() const;
		[[nodiscard]] glm::mat4 get_projection() const;

		/**
		 * @brief This should be updated every frame so that when the ratio changes, it reflects the
		 * new ratio.
		 *
		 * @param width Width of the viewport.
		 * @param height Height of the viewport.
		 */
		void set_aspect_ratio(int width, int height);

		[[nodiscard]] glm::vec3 get_forward() const;
		[[nodiscard]] glm::vec3 get_right() const;
		[[nodiscard]] glm::vec3 get_up() const;

		void rotate_pitch(float degrees);
		void rotate_yaw(float degrees);
		void rotate_roll(float degrees);
	};
} // namespace Nuit
