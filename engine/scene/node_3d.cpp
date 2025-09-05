module;
#include <glm/detail/type_quat.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/glm.hpp>
module nuit;

namespace Nuit
{
	const glm::mat4& Node3D::local_transform() const
	{
		return m_localTransform;
	}

	const glm::mat4& Node3D::world_transform() const
	{
		return m_worldTransform;
	}

	void Node3D::update_local_transform()
	{
		const glm::mat4 translation = glm::translate(glm::mat4(1.0f), Position);
		auto rotation = glm::mat4(1.0f);
		rotation = glm::rotate(rotation, glm::radians(Rotation.x), glm::vec3(1, 0, 0)); // pitch
		rotation = glm::rotate(rotation, glm::radians(Rotation.y), glm::vec3(0, 1, 0)); // yaw
		rotation = glm::rotate(rotation, glm::radians(Rotation.z), glm::vec3(0, 0, 1)); // roll
		const glm::mat4 scale = glm::scale(glm::mat4(1.0f), Scale);

		m_localTransform = translation * rotation * scale;
	}

	void Node3D::update_world_transform()
	{
		if (const auto* parent = dynamic_cast<Node3D*>(m_parent))
		{
			m_worldTransform = parent->world_transform() * m_localTransform;
		}
		else
		{
			m_worldTransform = m_localTransform;
		}
	}

	void Node3D::update_transform()
	{
		update_local_transform();
		update_world_transform();
	}
} // namespace Nuit
