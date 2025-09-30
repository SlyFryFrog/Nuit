module;
#include <glm/glm.hpp>
#include <ostream>
export module camera;

import nuit;

using namespace Nuit;

export class Camera final : public Camera3D
{
public:
	float Speed{10.0f};

public:
	void _init() override
	{
	}

	void _process(const double delta) override
	{
		glm::vec3 movement{};

		if (InputManager::is_pressed(KEY_W))
		{
			movement += get_forward();
		}
		if (InputManager::is_pressed(KEY_S))
		{
			movement -= get_forward();
		}
		if (InputManager::is_pressed(KEY_A))
		{
			movement -= get_right();
		}
		if (InputManager::is_pressed(KEY_D))
		{
			movement += get_right();
		}
		if (InputManager::is_pressed(KEY_SPACE))
		{
			movement += m_up;
		}
		if (InputManager::is_pressed(KEY_LEFT_SHIFT))
		{
			movement -= m_up;
		}


		if (InputManager::is_pressed(KEY_E))
		{
			rotate_yaw(-50.0f * static_cast<float>(delta));
		}
		if (InputManager::is_pressed(KEY_Q))
		{
			rotate_yaw(50.0f * static_cast<float>(delta));
		}

		Position += movement * Speed * static_cast<float>(delta);

		update_transform();
	}

	void _draw(GLShaderProgram& shader) override
	{
		shader.set_uniform("uView", get_view());
		shader.set_uniform("uProjection", get_projection());

		for (auto* child : m_children)
		{
			if (auto* nodeChild = dynamic_cast<Node3D*>(child))
			{
				nodeChild->update_transform();
			}
		}
	}
};
