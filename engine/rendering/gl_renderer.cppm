module;
export module nuit:gl_renderer;

namespace Nuit
{
	export enum PolygonMode
	{
		FILL,
		LINE,
		POINT
	};

	export class GLRenderer
	{
	public:
		/**
		 * @brief Before calling and GL-related functions, the GLRenderer MUST first be called.
		 */
		static void init();

		static void set_polygon_mode(PolygonMode mode);

		static void enable_depth_testing(bool enable);
	};
}