module;
export module nuit:object;

import :gl_shader;

namespace Nuit
{
	export class Object
	{
	public:
		virtual ~Object() = default;

		virtual void _init()
		{
		}

		virtual void _process(double delta)
		{
		}

		virtual void _draw(const GLShaderProgram& shader)
		{
		}
	};
} // namespace Nuit
