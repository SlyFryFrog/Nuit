module;
export module nuit:object;

namespace Nuit
{
	export class Object
	{
	public:
		virtual ~Object() = default;

		virtual void _init()
		{
		}

		virtual void _process()
		{
		}

		virtual void _draw() const
		{
		}
	};
} // namespace Nuit
