module;
export module nuit:math;

namespace Nuit
{
	export constexpr double PI = 3.141592653589793;

	export template <typename T = double>
	T dot(T x1, T x2, T y1, T y2)
	{
		return x1 * x2 + y1 * y2;
	}
} // namespace Nuit
