module;
#include <glm/glm.hpp>
export module nuit:node_3d;

import :object;

namespace Nuit
{
	export class Node3D : public Object
	{
	protected:
		Node3D* m_parent{nullptr};
		glm::mat4 m_worldTransform{1.0f};
		glm::mat4 m_localTransform{1.0f};

	public:
		glm::vec3 Position{0.0};
		glm::vec3 Rotation{0.0};
		glm::vec3 Scale{1.0f};

	public:
		[[nodiscard]] const glm::mat4& local_transform() const;
		[[nodiscard]] const glm::mat4& world_transform() const;

		void update_local_transform();
		void update_world_transform();

		void update_transform();
	};
} // namespace Nuit
