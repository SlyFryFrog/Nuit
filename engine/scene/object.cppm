module;
#include <vector>
export module nuit:object;

import :gl_shader;

namespace Nuit
{
	/**
	 * @brief The most basic Scene type available. All Scene-related Nodes inherit from this object.
	 */
	export class Object
	{
	protected:
		std::vector<Object*> m_children;

	public:
		virtual ~Object() = default;

		virtual void _init()
		{
		}

		virtual void _process(double delta)
		{
		}

		virtual void _draw(GLShaderProgram& shader)
		{
		}
	};
} // namespace Nuit
