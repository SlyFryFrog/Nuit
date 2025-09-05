module;
#include <glm/ext/matrix_transform.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
module nuit;

namespace Nuit
{
	void Camera3D::look_at(const glm::vec3 &point)
	{
		const glm::mat4 look = glm::lookAt(Position, point, m_up);

		// Update relevant data
		Rotation = glm::eulerAngles(glm::quat_cast(glm::inverse(look)));
		m_worldTransform = glm::inverse(look);
	}

	glm::mat4 Camera3D::get_view() const
	{
		return glm::inverse(m_worldTransform);
	}

	glm::mat4 Camera3D::get_projection() const
	{
		return glm::perspective(glm::radians(FOV), m_aspectRatio, Near, Far);
	}

	void Camera3D::set_aspect_ratio(const int width, const int height)
	{
		m_aspectRatio = static_cast<float>(width) / static_cast<float>(height);
	}

	glm::vec3 Camera3D::get_forward() const
	{
		// -Z is forward (into screen)
		return glm::normalize(glm::vec3(m_worldTransform[2])) * -1.0f;
	}

	glm::vec3 Camera3D::get_right() const
	{
		return glm::normalize(glm::vec3(m_worldTransform[0]));
	}

	glm::vec3 Camera3D::get_up() const
	{
		return glm::normalize(glm::vec3(m_worldTransform[1]));
	}
} // namespace Nuit
